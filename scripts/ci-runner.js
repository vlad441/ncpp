const net = require('net'); const tls = require('tls'); const dgram = require("dgram"); const Stream = require("stream");
const fs = require('fs'); const CBOR = require("./myCBOR.js"); let clSock; let usetls=true;
let host="ncpp.art", port=8210; let runner="winxp", token="";

// For WinXP: 
// -- Node v8.6.0: https://github.com/hlizard/node8-xp
// -- Git 2.10.0: https://github.com/git-for-windows/git/releases/tag/v2.10.0.windows.1
// execShell(): pipe redirect error: https://www.reddit.com/r/rust/comments/sv1m48/executing_external_exe_with_stdprocesscommand/

async function Sleep(msec){ return new Promise(function(resolve){ setTimeout(()=>resolve(), msec); }); }

const { spawn } = require("child_process"); //execSync?
let execShell=function(cmd, stream=null, detach=false, stdio=["ignore", "pipe", "pipe"], ){ //stdio: ["ignore", "inherit", "inherit"]
  return new Promise((resolve, reject) => { //const child = spawn(cmd, args);
	const child = spawn(cmd, { shell: true, stdio, detached: detach }); let output="";
	if(child.stdout){ child.stdout.on("data", d => stream ? stream.write(d) : output += d.toString()); }
    if(child.stderr){ child.stderr.on("data", d => stream ? stream.write(d) : output += d.toString()); }
    child.once("error", (err) => reject(err));
    //child.once("close", (code, signal) => code === 0 ? resolve(output) : reject({ code, signal, output }));
	child.once("exit",  (code, signal) => code === 0 ? resolve(output) : reject({ code, signal, output }));
  }); }
  
// ========= CBOR Packets =========
function CBORSplit(socket, chunk, callback){ const sdata=socket.data, mgr=this; sdata.rawbuf = Buffer.concat([sdata.rawbuf, chunk]); 
	let cbst = {offset: 0}, dsz = sdata.datasz; //if(!sdata.datasz){ sdata.rawbuf=Buffer.alloc(0); sdata.datasz=[0,0]; }
	while(sdata.rawbuf.length>0){ let bf = sdata.rawbuf;
		if(dsz[1]===0){ cbst.offset=0; let len = CBOR.parseLen(bf, cbst); if(len>0){ dsz[0]=cbst.offset; dsz[1]=len; }else{ break; } }
		if(bf.length < dsz[0]+dsz[1]) break; const body = bf.slice(dsz[0], dsz[0]+dsz[1]); sdata.rawbuf = bf.slice(dsz[0]+dsz[1]); dsz[0]=0; dsz[1]=0;
		try{ callback(socket, CBOR.parse(body)); }catch(e){ console.error("CBORSplit err:", e); } } }
		
function CBORPacket(obj={}){ let packet = CBOR.serialize(obj); let lenBuf=Buffer.alloc(CBOR._calcLenSz(packet.length));
	CBOR.writeLen(lenBuf, packet.length); return Buffer.concat([lenBuf, packet]); }
	
function sendMsg(socket, obj={}){ socket.write(CBORPacket(obj)); }
function sendErr(socket, txt="Unknown error", err={}){ socket.write(CBORPacket({type:"err", txt, ...err})); }
function sendOk(socket, code=0){ socket.write(CBORPacket({type:"cmdEnd", code})); }
function sendStdOut(socket, txt=""){ socket.write(CBORPacket({type:"stdout", txt})); }

// ========= Соединение =========
let retryMs=2000;
function onConnect(socket){ socket.data={auth: false, rawbuf: Buffer.alloc(0), datasz: [0,0], }; clSock=socket; 
  CmdStream = new Stream.PassThrough(); CmdStream.on("data", (chunk) => sendStdOut(clSock, chunk.toString()));
  console.log("[Socket] "+(usetls?"TLS":"TCP")+" connected to "+host+":"+port);
  socket.on("data", (chunk) => CBORSplit(socket, chunk, onPacket));
  socket.on("error", (e) => console.error("[Socket] Socket Error:", e));
  socket.on("close", () => { console.log("[Socket] Connection Closed."); reConnect(); });
  sendMsg(socket, {type:"auth", runner, token}); retryMs=2000; }

// ========= Обработчик пакетов =========
let CmdStream = new Stream.PassThrough(); 

function isAllowedFlags(flags=""){ let allowFlags=["", "GCC3", "GCC", "CLANG", "x32", "x64", "C++98", "C++11"]; if(typeof flags !== "string") return false; 
	let list=flags.split(" "); for(let value of list){ if(allowFlags.indexOf(value)===-1) return false; } return true; }

async function onPacket(socket, packet){ if(!packet || typeof packet !== "object") return; const cmd = packet.cmd;
  console.log("[Packet]", packet);
  try{ switch(cmd){
    //case "shell":   await execShell(packet.txt); sendOk(socket); break;
	case "getCode": { let branch="main"; if(packet.branch){ branch=packet.branch; }
		await execShell("chcp 1251 & rd /s /q ncpp & git clone --single-branch --branch "+branch+" --depth 1 https://github.com/vlad441/ncpp.git", CmdStream); sendOk(socket); break; }
	case "compile": { let flags=packet.flags?packet.flags:""; if(!isAllowedFlags(flags)){ sendErr(socket, "This flags set not allowed."); }
		try{ fs.unlinkSync("ncpp/tests/tests.exe"); }catch(e){}
		await execShell("chcp 1251 & cd ncpp\\tests && make_test.bat "+flags, CmdStream); sendOk(socket); break; }
	case "tests": { CmdStream.write("[Runner] Running Tests...\n\n");
		await execShell("cd ncpp\\tests && tests.exe --log", CmdStream, true).catch(err => TestsFailCb(err, socket));
		CmdStream.write(fs.readFileSync("ncpp\\tests\\tests.log", "utf8")); sendOk(socket); break; } //Kostyl: Stupid file pipe
	
	//case "tests": await execShell("cd ncpp\\tests && tests.exe", CmdStream).catch(err => TestsFailCb(err, socket)); sendOk(socket); break;
	//case "tests": await execShell("cd ncpp\\tests && tests.exe > tests.log 2>&1", CmdStream, true); sendOk(socket); break;
	//case "tests": await execShell("cd ncpp\\tests && start \"\" tests.exe > tests.log 2>&1", CmdStream); sendOk(socket); break;
	case "testsNoOut": { await execShell("cd ncpp\\tests && tests.exe", CmdStream, true, ["ignore", "inherit", "inherit"])
		.catch(err => TestsFailCb(err, socket)); sendOk(socket); break; }
    default: sendErr(socket, "Unknown cmd");
  } }catch(err){ console.log("[Execution error]", err); sendErr(socket, "Execution error", err); }
}

function TestsFailCb(err, socket){ sendErr(socket, "Tests Fail", err); }
	
// ========= Соединиться с сервером команд =========
async function reConnect(){ console.error("-- retryMs:", retryMs); await Sleep(retryMs); if(retryMs<60000){ retryMs*=2; } connect(); }
function connect(){ const opts = { host, port }; let sock;
	if(usetls){ sock = tls.connect({ ...opts, rejectUnauthorized: false }, () => { onConnect(sock); }); } 
	else{ sock = net.connect(opts, () => onConnect(sock)); }
	sock.once("error", (e) => { console.error((usetls?"TLS":"TCP"), "err:", e.message); reConnect(); });
}

CLIparse(); connect();

// =================== console argv ===================
function CLIparse(){ for(let i=2;i<process.argv.length;i++){ switch(process.argv[i]){
  	case "--help": { console.log("Syntax: node ci-runner.js\nArgs:\n"
		+" --host(-h) <host>\n --port <port>\n --token(-t) <token>\n --tls <true/false/1/0>"); process.exit(0); }
	case "--host": case "-h": { host=process.argv[i+1]; break; }
	case "--port": { port=Number(process.argv[i+1]); break; }
	case "--token": case "-t": { let p=process.argv[i+1]; if(p[0]=="\""&&p.endsWith("\"")) p=p.slice(1,-1); token=p; break; }
	case "--tls": { let istls=process.argv[i+1]; usetls=(istls==="true"||istls==="1")?true:false; break; } } } }
// =================== CLI commands ===================
// const rl = require("readline").createInterface({input: process.stdin}); rl.prompt(); rl.on("line", (input) => { onInput(input); });

// function onInput(msg){ let rcmd=msg.split(" ");
	// if(rcmd[0]==="help"){ console.log("CLI commands:\n");
		// console.log(" No Commands\n "); return; } }