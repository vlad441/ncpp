const fs = require('fs'); const path = require('path');

function processFile(filePath, stdInclude=false, processedFiles = new Set(), baseDir = null){
  if(processedFiles.has(filePath)){ return ''; } processedFiles.add(filePath); let content; 
  try{ content = fs.readFileSync(filePath, 'utf-8'); console.log(`Included: ${filePath}`); }
  catch(e){ console.error(`Error reading file: ${filePath}`); return ""; }
  if(baseDir === null){ baseDir = path.dirname(filePath); }

  // Регулярное выражение для поиска #include ""
  let includePattern; if(stdInclude){ includePattern = /#include\s+"([^"]+)"/g; }else{ includePattern = /#include\s+"([^"]+)"/g; }
  let match; let result = ""; let lastIndex = 0;

  while((match = includePattern.exec(content)) !== null){ const includedPath = match[1]; const absPath = path.resolve(baseDir, includedPath);
    // Добавляем часть файла до текущего #include
    result += content.substring(lastIndex, match.index);
	
    if(match.index>=2&&content[match.index-1]=='/'&&content[match.index-2]=='/'){ console.log([match[0]]);
		result += match[0]+"(Тут не включилось название иклуда)"; lastIndex = includePattern.lastIndex; console.log(`NOT included (commented): ${match[0]}`); continue; }

    // Рекурсивно обрабатываем включенный файл
    const includedContent = processFile(absPath, stdInclude, processedFiles, path.dirname(absPath));
    result += includedContent; lastIndex = includePattern.lastIndex; }

  result += content.substring(lastIndex); // Добавляем оставшуюся часть файла
  return result.replace(includePattern, ''); //снос всех #include ""
}

function HInclude(sourceFile, outputFile, stdInclude=false){
  const absPath = path.resolve(sourceFile); const content = processFile(absPath, stdInclude);

  try{ fs.writeFileSync(outputFile, content, 'utf-8'); console.log(`\n(#) Successfully concatenated in file: ${outputFile}`); }
  catch(e){ console.error(`(!) Error writing to file: ${outputFile}`); } }

HInclude("../src/ncpp.cpp", "../tests/ncpp-unity-bundle.cpp");