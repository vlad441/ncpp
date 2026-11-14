const net = require('net'); const tls = require('tls'); const dgram = require("dgram"); const Stream = require("stream");
const fs = require('fs'); const CBOR = require("./myCBOR.js"); let clSock; let usetls=true;
let runnerSrv, ctrlSrv; let port=8210, cport=8211, host="::"; let token="";

let runners={};
  
// ========= CBOR Packets =========
function CBORSplit(socket, chunk, callback){ const sdata=socket.data; sdata.rawbuf = Buffer.concat([sdata.rawbuf, chunk]);
	let cbst = {offset: 0}, dsz = sdata.datasz; //if(!sdata.datasz){ sdata.rawbuf=Buffer.alloc(0); sdata.datasz=[0,0]; }
	while(sdata.rawbuf.length>0){ let bf = sdata.rawbuf;
		if(dsz[1]===0){ cbst.offset=0; let len = CBOR.parseLen(bf, cbst); if(len>0){ dsz[0]=cbst.offset; dsz[1]=len; }else{ break; } }
		if(bf.length < dsz[0]+dsz[1]) break; const body = bf.slice(dsz[0], dsz[0]+dsz[1]); sdata.rawbuf = bf.slice(dsz[0]+dsz[1]); dsz[0]=0; dsz[1]=0;
		try{ callback(socket, CBOR.parse(body)); }catch(e){ console.error("CBORSplit err:", e); } } }
		
function CBORPacket(obj={}){ let packet = CBOR.serialize(obj); let lenBuf=Buffer.alloc(CBOR._calcLenSz(packet.length));
	CBOR.writeLen(lenBuf, packet.length); return Buffer.concat([lenBuf, packet]); }
	
function sendMsg(socket, obj={}){ socket.write(CBORPacket(obj)); }
function sendResp(socket, txt=""){ socket.write(CBORPacket({type:"srvresp", txt})); }
function sendOk(socket){ socket.write(CBORPacket({srvresp:"Ok", ok:true})); }
function sendErr(socket, txt="Unknown error"){ socket.write(CBORPacket({type:"srverr", txt})); }

// ========= Сервер для Runners =========
function onConnectionMgr(socket, onPacket, name="Socket"){ socket.data={auth: false, rawbuf: Buffer.alloc(0), datasz: [0,0], };
  console.log("["+name+"] New Connection: "+socket.remoteAddress+":"+socket.remotePort);
  socket.on("data", (chunk) => CBORSplit(socket, chunk, onPacket));
  socket.on("error", (e) => console.error("["+name+"] Socket Error ("+socket.remoteAddress+":"+socket.remotePort+"):", e));
  socket.on("close", () => { console.log("["+name+"] Closed Connection: "+socket.remoteAddress+":"+socket.remotePort); });
}
function onExecConnection(socket){ onConnectionMgr(socket, onExecPacket, "Runner"); 
	socket.on("close", () => { if(runners[socket.data.name]===socket) delete runners[socket.data.name]; }); }

async function onExecPacket(socket, packet){ if(!packet || typeof packet !== "object") return; const type = packet.type;
  console.log("[ExecPacket]", packet);
  if(type=="auth"){ if(packet.token!==token){ sendErr(socket, txt="Auth Error"); return; } 
	runners[packet.runner]=socket; socket.data.name=packet.runner; socket.data.auth=true; return; }
  if(!socket.data.auth){ socket.destroy(); return; }
  
  if(socket.data.rspSock) sendMsg(socket.data.rspSock, packet);
}

// ========= Сервер для Clients =========
function onCtrlConnection(socket){ onConnectionMgr(socket, onCtrlPacket, "Client"); }

async function onCtrlPacket(socket, packet){ if(!packet || typeof packet !== "object") return; const cmd = packet.cmd;
  console.log("[CtrlPacket]", packet);
  if(cmd=="auth"){ if(packet.token!==token){ sendErr(socket, txt="Auth Error"); return; }
	socket.data.auth=true; sendOk(socket); return; }
  if(!socket.data.auth){ socket.destroy(); return; } 
  
  if(packet.srvcmd){ switch(packet.srvcmd){
	case "list": sendResp(socket, "["+Object.keys(runners).join(", ")+"]"); return;
    default: sendErr(socket, txt="Unknown cmd");
  } }
  
  let rsock=runners[packet.runner]; if(!rsock){ sendErr(socket, "Runner \""+packet.runner+"\" not registred."); return; }
  //if(rsock.data.rspSock && rsock.data.rspSock !== socket){ sendErr(socket, "Runner busy"); return; }
  rsock.data.rspSock=socket; sendMsg(runners[packet.runner], packet);
}

function createSrv(name="runnerSrv", port, host="::", onConnectionCb){
  let server = usetls
    ? tls.createServer({ key: fs.readFileSync("key.pem"), cert: fs.readFileSync("cert.crt") }, onConnectionCb)
    : net.createServer(onConnectionCb);
  server.listen(port, host, () => { console.log(`${usetls?"TLS":"TCP"} ${name} starts on: ${host}:${port}`); });
  server.on("error", (e) => console.error("server err:", e)); return server; }

// =========  =========
function main(){ 
	runnerSrv = createSrv("runnerSrv", port, host, onExecConnection); 
	ctrlSrv = createSrv("ctrlSrv", cport, host, onCtrlConnection); }

CLIparse(); main();

// =================== console argv ===================
function CLIparse(){ for(let i=2;i<process.argv.length;i++){ switch(process.argv[i]){
  	case "--help": { console.log("Syntax: node ci-executor.js\nArgs:\n"
			+" --host(-h) <host>\n --port <port>\n --token(-t) <token>\n --tls <true/false/1/0>"); process.exit(0); }
	case "--host": case "-h": { host=process.argv[i+1]; break; }
	case "--port": { port=Number(process.argv[i+1]); break; }
	case "--token": case "-t": { let p=process.argv[i+1]; if(p[0]=="\""&&p.endsWith("\"")) p=p.slice(1,-1); token=p; break; }
	case "--tls": { let istls=process.argv[i+1]; usetls=(istls==="true"||istls==="1")?true:false; break; } } } }
// =================== CLI commands ===================