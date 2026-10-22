const fs = require('fs');

function cleanAndJoinHex(txt=""){ const lines = txt.split(/\r?\n/); let buffParts = [], hexParts = [];
    for(let line of lines){ let cleanLine = line; commIdx = line.indexOf('#'); 
		if(commIdx==-1) commIdx = line.indexOf(';'); if(commIdx==-1) commIdx = line.indexOf("//");

        if(commIdx !== -1) cleanLine = line.substring(0, commIdx); cleanLine = cleanLine.trim(); if (cleanLine=="") continue;
		cleanLine = cleanLine.replace(/\s+/g, ""); //cleanLine.split(/\s+/).join(); //Чистим пробелы
		buffParts.push(Buffer.from(cleanLine, "hex"));
		
        // for (let token of tokens){
            // if(/^[0-9A-Fa-f]{1,2}$/.test(token)){ // Проверяем, является ли токен валидным HEX-байтом (1 или 2 символа: 0-9, A-F)
                // hexParts.push(token.padStart(2, '0').toUpperCase()); // Приводим к верхнему регистру и дополняем до 2 знаков (на всякий случай)
            // } }
    } return Buffer.concat(buffParts); }
	
function compileHEX(fInp="input.txt", fOut="output.exe"){ const txt = fs.readFileSync(fInp, "utf8");
	console.log(" 1) Input:", fInp); console.log(" -- Размер: "+Buffer.byteLength(txt)+" байт | Текст: "+txt.length+" символов");
	const buff = cleanAndJoinHex(txt); fs.writeFileSync(fOut, buff);
	console.log(" 2) Output:", fOut); console.log(" -- Размер: "+buff.length+" байт"); }

//const result = cleanAndJoinHex(rawData); console.log("Результат склейки:"); console.log(result);
compileHEX("PE32-min.txt");