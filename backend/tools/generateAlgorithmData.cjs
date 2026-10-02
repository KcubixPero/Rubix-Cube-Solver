const fs = require("node:fs");
const vm = require("node:vm");

const [inputPath, outputPath, symbol] = process.argv.slice(2);
const scope = {};
vm.runInNewContext(`${fs.readFileSync(inputPath, "utf8")}\nglobalThis.exportedAlgSet = algSet;`, scope);
const cases = scope.exportedAlgSet.cases.filter((item) => item.id !== "Skip");
const rows = [];
for (const item of cases) {
  const options = item.algs.flatMap((entry) => [entry, ...(entry.vars || [])].map((variant) => variant.alg));
  const algorithm = options.find((candidate) => candidate.split(/\s+/).every((token) => {
    const move = token.replace(/[()[\]]/g, "").replace(/2'$/, "2");
    return /^[RLUDFBrludfbMESxyz](?:2|')?$/.test(move);
  }));
  if (!algorithm) throw new Error(`No supported ${symbol} algorithm found for ${item.id}`);
  const normalized = algorithm.replace(/([RLUDFBrludfbMESxyz])2'/g, "$12");
  rows.push(`    {${JSON.stringify(item.id)}, ${JSON.stringify(normalized)}},`);
}
const body = `// Algorithm notation adapted from Logiqx cubing-algs (GPL-3.0):\n// https://github.com/Logiqx/cubing-algs/tree/master/data\nstatic constexpr RawAlgorithm ${symbol}[] = {\n${rows.join("\n")}\n};\n`;
fs.writeFileSync(outputPath, body);
console.log(`Wrote ${rows.length} ${symbol} entries to ${outputPath}`);
