const fs = require('fs');

//const defines = ["_WIN32", ["__cplusplus", "201103L"]];
const defines = [["__cplusplus", "201103L"]];

const declareRegex = /^\s*(template|typedef|using)\s+/;
const structRegex = /^\s*(struct|class|namespace|static struct|static class)\s+/;
const funcRegex = /^\s*([\w\s\*&:<>,~]+)\s+([\w:]+)\s*\([^\)]*\)\s*(const)?\s*(override|final)?\s*(noexcept)?\s*(\{|\;)\s*$/;

function isDefined(macro){ return defines.includes(macro) || defines.some(d => Array.isArray(d) && d[0] === macro); }

function evalExpr(expr){ expr = expr.trim();
	if(expr.startsWith("defined(")){ const macro = expr.match(/defined\(([^)]+)\)/)[1]; return isDefined(macro); }
	if(expr.startsWith("!defined(")){ const macro = expr.match(/!defined\(([^)]+)\)/)[1]; return !isDefined(macro); }
	if(expr.includes("&&")){ const [a, b] = expr.split("&&"); return evalExpr(a) && evalExpr(b); }
	if(expr.includes("||")){ const [a, b] = expr.split("||"); return evalExpr(a) || evalExpr(b); }
	
	const ops = [">=", ">", "<=", "<", "==", "!="];
	for (const op of ops){
		if (expr.includes(op)){
			const [lhs, rhs] = expr.split(op).map(s => s.trim());
			const def = defines.find(d => Array.isArray(d) && d[0] === lhs);
			if (!def) return false; const lval = parseInt(def[1]); const rval = parseInt(rhs);
			switch (op){
				case "<=": return lval <= rval;
				case ">=": return lval >= rval;
				case "==": return lval == rval;
				case "!=": return lval != rval;
				case "<":  return lval < rval;
				case ">":  return lval > rval; } } }
	return isDefined(expr); }

function Preprocessing(inpstr = ""){ const lines = inpstr.split("\n");
	const result = []; const stack = []; let active = true;
	for(let line of lines){ const tLine = line.trim(); let expr;
		if(tLine.startsWith("#")){ expr = tLine.split(" ").slice(1).join(" "); }
	
		if(tLine.startsWith("#if")){ const res = evalExpr(expr);
			stack.push({ wasActive: active, thisActive: res }); active = active && res; }
		else if(tLine.startsWith("#ifdef")){ const res = isDefined(expr);
			stack.push({ wasActive: active, thisActive: res }); active = active && res; } 
		else if(tLine.startsWith("#ifndef")){ const res = !isDefined(expr);
			stack.push({ wasActive: active, thisActive: res }); active = active && res; }
		else if(tLine.startsWith("#else")){ const top = stack.pop(); const res = !top.thisActive;
			stack.push({ wasActive: top.wasActive, thisActive: res });
			active = top.wasActive && res; }
		else if(tLine.startsWith("#elif")){ const top = stack.pop(); const res = evalExpr(expr);
			stack.push({ wasActive: top.wasActive, thisActive: res });
			active = top.wasActive && res; } 
		else if(tLine.startsWith("#endif")){ const top = stack.pop(); active = top.wasActive; }
		else{ if(active) result.push(line); } } return result.join("\n"); }

function get_funcname(signature=""){ let brindx=-1; if((brindx=line.indexOf("("))>=0){ signature=signature.slice(0, brindx); }else{ return ""; }
	return signature.split(" ").slice(-1); }
	
function findClosingBrace(str="", start){ let depth = 0;
    for(let i = start; i < str.length; i++){
        if(str[i] === '{') depth++;
        else if(str[i] === '}') depth--;
        if(depth===0) return i; } return -1; }
        
function cutBlocksInLine(str){ let nindx=-1;
	while((nindx=str.indexOf("{", nindx))>=0){ let indx = findClosingBrace(str, nindx); if(str[nindx-1]===' ') nindx--;
		if(indx>=0){ let substr=str.slice(nindx, indx+1); str=str.replace(substr, ";"); }else{ break; } } return str; }

function header_gen(inpstr = ""){ const lines = inpstr.split("\n"); let output = ""; let i = 0;
	while (i < lines.length){ let line = lines[i]; line=cutBlocksInLine(line);
		// Поиск функции: наличие ( ) перед { и не быть =0; или ;
		const isFunc = /\w[\w\s:*&<>]*\([^\)]*\)\s*(const)?\s*(noexcept)?\s*(\{|$)/.test(line) && line.includes("{");
		
		if (isFunc&&!line.match(structRegex)){ const start = inpstr.indexOf(line);
			const end = findClosingBrace(inpstr, start + line.indexOf("{"));
			if (end > start){
				const header = line.split("{")[0].replace(/\s+$/, "");
				output += header.endsWith(";") ? header : header + ";\n";
				i += inpstr.slice(start, end).split("\n").length; continue; } }
		i++; const tLine=line.trim(); //if(tLine.startsWith("private:")||tLine.startsWith("protected:")){} 
		if(tLine==""||tLine.startsWith("#")||tLine.startsWith("//")) continue; output += line + "\n"; } 
	output=output.replace(/ {}/g, ";"); output=output.replace(/;;/g, ";"); return output; }



function header_save(inpath="input.cpp", outpath="output.h"){
	fs.readFile(inpath, 'utf8', (err, data) => {
		if (err){ console.error('Ошибка чтения файла:', err); return; }
		const output = header_gen(Preprocessing(data.toString()));

		fs.writeFile(outpath, output, 'utf8', (err) => {
			if(err){ console.error("Ошибка записи файла:", err); } 
			else{ console.log("Заголовочный файл сгенерирован: "+outpath); } });
	}); }
//header_save("../src/ncpp-base.cpp", "output.h");


