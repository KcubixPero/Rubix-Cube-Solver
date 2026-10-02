import test from "node:test";
import assert from "node:assert/strict";
import { applyMove, applyMoves, createSolvedCube, tokenizeMoves } from "./cubeState.js";

const stateKey = (cube) => JSON.stringify(cube.map(({ position, stickers }) => [position, Object.entries(stickers).sort()]));

test("each face turn followed by its inverse restores the solved cube", () => {
  const solved = createSolvedCube();
  for (const face of ["R", "L", "U", "D", "F", "B"]) {
    const inverse = `${face}${String.fromCharCode(39)}`;
    assert.equal(stateKey(applyMove(applyMove(solved, face), inverse)), stateKey(solved), face);
  }
});

test("four quarter turns and two half turns restore the cube", () => {
  const solved = createSolvedCube();
  for (const face of ["R", "L", "U", "D", "F", "B"]) {
    let four = solved;
    for (let i = 0; i < 4; i += 1) four = applyMove(four, face);
    assert.equal(stateKey(four), stateKey(solved), face);
    assert.equal(stateKey(applyMove(applyMove(solved, `${face}2`), `${face}2`)), stateKey(solved), `${face}2`);
  }
});

test("move notation accepts only the existing 18 moves", () => {
  assert.deepEqual(tokenizeMoves("R U' F2 L D' B"), ["R", "U'", "F2", "L", "D'", "B"]);
  assert.throws(() => tokenizeMoves("x R"), /Invalid move notation/);
  assert.equal(stateKey(applyMoves(createSolvedCube(), "R R'")), stateKey(createSolvedCube()));
});
