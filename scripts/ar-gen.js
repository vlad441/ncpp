let fname="../src/ncpp.cpp";
let fs = require("fs");

function argen_line(incl_all=false){ let lines = fs.readFileSync(fname).toString(); let endl="\n";
	lines=lines.slice(lines.indexOf("#include \"ncpp-base.cpp\"")).split(endl);
	let pattern="#include \""; if(lines[0].indexOf("<")>0){ pattern="#include <"; }
	let linearg=""; for(let line of lines){ if(line=="") continue; if(!incl_all&&line.startsWith("//")) continue; line=line.split(pattern)[1]; 
		if(line==undefined) continue; line=line.replace(/\//g,"_"); linearg+=line.split(".")[0]+".o "; }
	console.log("ar rcs ../libncpp.a "+linearg); }
argen_line();

