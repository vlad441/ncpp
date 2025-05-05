const net = require('net'); let server = net.createServer();
const http = require('http');

var config = { tcpport:3001 };

if(config.tcpport>0){ server.listen(config.tcpport, function(){ console.log('TCP cервер запущен: ', server.address()); }); }

server.on('error', function(err){ console.log(funcs.ShowLogDate()+"Server error: "+err); });

server.on('connection', function(socket){
	console.log(funcs.ShowLogDate()+"TCP подключение: "+socket.remoteAddress+":"+socket.remotePort); OnSocketConnection(socket); });

function OnSocketConnection(socket){
	socket.on("data", function(data){ OnMessage(socket, data); });
    socket.once("error", function(err){ console.log("У клиента "+socket.remoteAddress+":"+socket.remotePort+" произошла ошибка сокета: "+err); socket.destroy(); });
    socket.once("close", function(){ console.log(funcs.ShowLogDate()+"Клиент "+socket.remoteAddress+":"+socket.remotePort+" отключился."); });
    socket.write("test passed."); 
}
// =====
function OnMessage(socket, data){ console.log("Socket data:", data.toString()); console.log(" -- buffer: ", data); }

// =====
let funcs={
	ShowDate(date=new Date()){ let resp=""; if(date.getDate()<10){ resp+="0"; } resp+=date.getDate()+"."; if(date.getMonth()+1<10){ resp+="0" }
		resp+=Number(date.getMonth()+1)+"."+date.getFullYear()+" "+date.getHours()+":"; if(date.getMinutes()<10){ resp+="0" } resp+=date.getMinutes(); return resp; },
	ShowLogDate(){ return "["+funcs.ShowDate()+"] "; },
	ShowReverseDate(date=new Date()){ let resp=date.getFullYear()+"-";
		if(date.getMonth()+1<10){ resp+="0" } resp+=Number(date.getMonth()+1)+"-"; if(date.getDate()<10){ resp+="0"; } resp+=date.getDate()
		resp+=" "+date.getHours()+":"; if(date.getMinutes()<10){ resp+="0" } resp+=date.getMinutes(); return resp; },};
