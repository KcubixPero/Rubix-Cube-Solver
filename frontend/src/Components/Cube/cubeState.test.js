import test from "node:test";
import assert from "node:assert/strict";
import { applyMove, applyMoves, createSolvedCube, serializeCubeState, tokenizeMoves } from "./cubeState.js";

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

test("cube serialization matches the backend face order and fixed colors", () => {
  const solved = serializeCubeState(createSolvedCube());
  assert.deepEqual(solved.map((face) => face[1][1]), [0, 1, 2, 3, 4, 5]);
  assert.deepEqual(solved.map((face) => face.flat().every((color) => color === face[1][1])), [true, true, true, true, true, true]);

  const scrambled = serializeCubeState(applyMoves(createSolvedCube(), "R U2 F' L D B2"));
  assert.deepEqual(scrambled.map((face) => face[1][1]), [0, 1, 2, 3, 4, 5]);
  assert.deepEqual(Array.from({ length: 6 }, (_, color) => scrambled.flat(2).filter((value) => value === color).length), [9, 9, 9, 9, 9, 9]);
});
