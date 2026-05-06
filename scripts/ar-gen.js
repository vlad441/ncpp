let fs = require("fs");

function PrepareNames(fname="../src/ncpp.cpp", incl_all=false){ let lines = fs.readFileSync(fname).toString(); let endl="\n";
	let pattern="#include \""; lines=lines.split(endl); let names=[]; //pattern="#include <";
	for(let line of lines){ if(line==""||!line.trim().startsWith(pattern)) continue; if(!incl_all&&line.startsWith("//")) continue; 
		line=line.split(pattern)[1]; if(line==undefined) continue; line=line.replace(/\//g,"_"); names.push(line.split(".")[0]+".o"); } return names; }
	
function argen_line(){
	let base1 = PrepareNames("../src/ncpp-base.cpp"); if(base1[base1.length-1]=="base_ostream.o") base1.pop();
	let main1 = PrepareNames("../src/ncpp.cpp"); if(main1[0]=="ncpp.o") main1.shift(); if(main1[0]=="ncpp-base.o") main1.shift(); 
	//console.log("base1: ", base1, "main1: ", main1); 
	console.log("ar rcs ../libncpp.a "+base1.join(" ")+" "+main1.join(" ")); 
} argen_line();

