const net = require('net'); const tls = require('tls'); const dgram = require("dgram"); const Stream = require("stream");
const fs = require('fs'); const CBOR = require("./myCBOR.js"); let clSock; let usetls=true;
let host="ncpp.art", port=8211; let runner="winxp", branch="dev", token="";

async function Sleep(msec){ return new Promise(function(resolve){ setTimeout(()=>resolve(), msec); }); }
  
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
function sendCmd(socket, cmd=""){ socket.write(CBORPacket({cmd, runner})); }
function sendSrvCmd(socket, srvcmd=""){ socket.write(CBORPacket({srvcmd})); }
function sendErr(socket, txt="Unknown error"){ socket.write(CBORPacket({type:"err", txt})); }

// ========= Соединение =========
let retryMs=2000; let nextPacket=null; let onecmd=false;
function onConnect(socket){ socket.data={auth: false, rawbuf: Buffer.alloc(0), datasz: [0,0], }; clSock=socket;
  console.log("[Socket] "+(usetls?"TLS":"TCP")+" connected to "+host+":"+port);
  socket.on("data", (chunk) => CBORSplit(socket, chunk, onPacket));
  socket.on("error", (e) => console.error("[Socket] Socket Error:", e));
  socket.on("close", () => { console.log("[Socket] Connection Closed."); }); //reConnect();
  sendMsg(socket, {cmd:"auth", token}); retryMs=2000;
  if(nextPacket!=null){ sendMsg(socket, nextPacket); } }

// ========= Обработчик пакетов =========
async function onPacket(socket, packet){ if(!packet || typeof packet !== "object") return; const type = packet.type;
  //console.log("[Packet]", packet);
  switch(type){
	 case "stdout": process.stdout.write(packet.txt); break;
	 case "cmdEnd": if(onecmd){ socket.destroy(); console.log("[Complete] Process complete code:", packet.code); process.exit(packet.code); } break;
	 case "err": { console.log("[Runner Err]", packet.txt, "(code:", packet.code, "| signal:", packet.signal, ")");
		if(onecmd){ socket.destroy(); process.exit(1); } break; }
	 case "srvresp": console.log("Server> ", packet.txt);    if(onecmd){ socket.destroy(); process.exit(1); } break;
	 case "srverr": console.log("[Server Err]", packet.txt); if(onecmd){ socket.destroy(); process.exit(1); } break;
  }
}
	
// ========= Соединиться с сервером команд =========
async function reConnect(){ console.error("-- retryMs:", retryMs); await Sleep(retryMs); if(retryMs<60000){ retryMs*=2; } connect(); }
function connect(){ const opts = { host, port }; let sock;
	if(usetls){ sock = tls.connect({ ...opts, rejectUnauthorized: false }, () => { onConnect(sock); }); } 
	else{ sock = net.connect(opts, () => onConnect(sock)); }
	sock.on("error", (e) => { console.error((usetls?"TLS":"TCP"), "err:", e.message); });
}

CLIparse(); connect();

// =================== console argv ===================
function CLIparse(){ for(let i=2;i<process.argv.length;i++){ switch(process.argv[i]){
  	case "--help": { console.log("Syntax: node ci-executor.js\nArgs:\n"
		+" --host(-h) <host>\n --port <port>\n --token(-t) <token>\n --tls <true/false/1/0>\n --runner <runner>\n --branch <branch>\n --cmd <cmd>"
		+"\nRunner Commands:\n getCode\n compile <flags>\n tests"
		+"\nSrv Commands:\n list"); process.exit(0); }
	case "--host": case "-h": { host=process.argv[i+1]; break; }
	case "--port": { port=Number(process.argv[i+1]); break; }
	case "--token": case "-t": { let p=process.argv[i+1]; if(p[0]=="\""&&p.endsWith("\"")) p=p.slice(1,-1); token=p; break; }
	case "--tls": { let istls=process.argv[i+1]; usetls=(istls==="true"||istls==="1")?true:false; break; }
	case "--runner": { runner=process.argv[i+1]; break; }
	case "--branch": { branch=process.argv[i+1]; break; }
    case "--cmd": { onecmd=true; nextPacket={cmd:process.argv[i+1], runner}; break; }
	case "getCode": { onecmd=true; nextPacket={cmd:process.argv[i], runner, branch}; console.log("branch:", branch); break; }
	case "compile": { onecmd=true; nextPacket={cmd:process.argv[i], runner, flags: process.argv.slice(i+1).join(" ")}; return; }
	case "tests": case "testsNoOut": { onecmd=true; nextPacket={cmd:process.argv[i], runner}; break; }
	case "list": { onecmd=true; nextPacket={srvcmd:process.argv[i]}; break; } } } }
// =================== CLI commands ===================
// const rl = require("readline").createInterface({input: process.stdin}); rl.prompt(); rl.on("line", (input) => { onInput(input); });

// function onInput(msg){ let rcmd=msg.split(" ");
	// if(rcmd[0]==="help"){ console.log("CLI commands:\n");
		// console.log(" No Commands\n "); return; } }