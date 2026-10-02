import { COLORS } from "./constants.js";

const NORMALS = {
  left: [-1, 0, 0], right: [1, 0, 0],
  bottom: [0, -1, 0], top: [0, 1, 0],
  back: [0, 0, -1], front: [0, 0, 1],
};
const NORMAL_KEYS = Object.fromEntries(Object.entries(NORMALS).map(([key, normal]) => [normal.join(","), key]));
const FACE_COLORS = {
  left: COLORS.BLUE, right: COLORS.GREEN,
  bottom: COLORS.ORANGE, top: COLORS.RED,
  back: COLORS.YELLOW, front: COLORS.WHITE,
};

export function createSolvedCube() {
  const cubies = [];
  let id = 0;
  for (let x = -1; x <= 1; x += 1) {
    for (let y = -1; y <= 1; y += 1) {
      for (let z = -1; z <= 1; z += 1) {
        if (x === 0 && y === 0 && z === 0) continue;
        const stickers = {};
        for (const [face, normal] of Object.entries(NORMALS)) {
          if (normal[0] * x + normal[1] * y + normal[2] * z === 1) {
            stickers[face] = FACE_COLORS[face];
          }
        }
        cubies.push({ id: id++, position: [x, y, z], stickers });
      }
    }
  }
  return cubies;
}

function rotateVector([x, y, z], axis, quarter) {
  if (axis === 0) return quarter > 0 ? [x, -z, y] : [x, z, -y];
  if (axis === 1) return quarter > 0 ? [z, y, -x] : [-z, y, x];
  return quarter > 0 ? [-y, x, z] : [y, -x, z];
}

const MOVE = {
  R: [0, 1, -1], L: [0, -1, 1],
  U: [1, 1, -1], D: [1, -1, 1],
  F: [2, 1, -1], B: [2, -1, 1],
};

export function tokenizeMoves(sequence) {
  const tokens = sequence.trim() ? sequence.trim().split(/\s+/) : [];
  for (const token of tokens) {
    if (!/^[RLUDFB](?:2|')?$/.test(token)) {
      throw new Error(`Invalid move notation: ${token}`);
    }
  }
  return tokens;
}

export function applyMove(cube, token) {
  const base = token[0];
  const [axis, layer, clockwise] = MOVE[base];
  const turns = token.endsWith("2") ? 2 : 1;
  const direction = token.endsWith("'") ? -clockwise : clockwise;
  let next = cube;
  for (let turn = 0; turn < turns; turn += 1) {
    next = next.map((cubie) => {
      if (cubie.position[axis] !== layer) return cubie;
      const position = rotateVector(cubie.position, axis, direction);
      const stickers = {};
      for (const [face, color] of Object.entries(cubie.stickers)) {
        const normal = rotateVector(NORMALS[face], axis, direction);
        stickers[NORMAL_KEYS[normal.join(",")]] = color;
      }
      return { ...cubie, position, stickers };
    });
  }
  return next;
}

export function applyMoves(cube, sequence) {
  return tokenizeMoves(sequence).reduce(applyMove, cube);
}
