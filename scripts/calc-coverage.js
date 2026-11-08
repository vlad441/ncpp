const fs = require("fs");
const content = fs.readFileSync("gcov-coverage.txt", "utf8");
let covs=[];

function calcCov(regex, name, unit){
    let total = 0, exec = 0, match;
    while ((match = regex.exec(content)) !== null){
        const pct = parseFloat(match[1]), count = parseInt(match[2], 10);
        exec += count * (pct / 100); total += count; }
    if(total > 0){ let p = Math.floor(((exec/total)*100)*100)/100;
		console.log(`== ${name}:`, p, "% (", Math.floor(exec), "/", total, `) ${unit}.`); }
    else{ console.log(`== ${name}: No ${unit}`); }
}

calcCov(/Lines executed:([0-9.]+)% of ([0-9]+)/g, "Lines Coverage", "lines");
calcCov(/Branches executed:([0-9.]+)% of ([0-9]+)/g, "Branches Coverage", "branches");
calcCov(/Taken at least once:([0-9.]+)% of ([0-9]+)/g, "Taken at least once", "branches");
calcCov(/Calls executed:([0-9.]+)% of ([0-9]+)/g, "Calls Coverage", "calls");