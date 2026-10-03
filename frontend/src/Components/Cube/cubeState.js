import { COLORS } from "./constants.js";

const NORMALS = {
  left: [-1, 0, 0],
  right: [1, 0, 0],
  bottom: [0, -1, 0],
  top: [0, 1, 0],
  back: [0, 0, -1],
  front: [0, 0, 1],
};
const NORMAL_KEYS = Object.fromEntries(
  Object.entries(NORMALS).map(([key, normal]) => [normal.join(","), key]),
);
const FACE_COLORS = {
  left: COLORS.BLUE,
  right: COLORS.GREEN,
  bottom: COLORS.ORANGE,
  top: COLORS.RED,
  back: COLORS.YELLOW,
  front: COLORS.WHITE,
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
  R: [0, 1, -1],
  L: [0, -1, 1],
  U: [1, 1, -1],
  D: [1, -1, 1],
  F: [2, 1, -1],
  B: [2, -1, 1],
};

const BACKEND_FACE = {
  front: { face: 0, cell: ([x, y]) => [1 - y, x + 1] },
  top: { face: 1, cell: ([x, , z]) => [z + 1, x + 1] },
  left: { face: 2, cell: ([, y, z]) => [1 - y, z + 1] },
  right: { face: 3, cell: ([, y, z]) => [1 - y, 1 - z] },
  bottom: { face: 4, cell: ([x, , z]) => [1 - z, x + 1] },
  back: { face: 5, cell: ([x, y]) => [1 - y, 1 - x] },
};
const COLOR_ID = Object.fromEntries(
  Object.entries(FACE_COLORS).map(([face, color]) => [
    color.toLowerCase(),
    {
      left: 2,
      right: 3,
      bottom: 4,
      top: 1,
      back: 5,
      front: 0,
    }[face],
  ]),
);

export function serializeCubeState(cubies) {
  const faces = Array.from({ length: 6 }, () =>
    Array.from({ length: 3 }, () => Array(3).fill(-1)),
  );
  for (const cubie of cubies) {
    for (const [side, color] of Object.entries(cubie.stickers)) {
      const mapping = BACKEND_FACE[side];
      const colorId = COLOR_ID[color.toLowerCase()];
      if (!mapping || colorId === undefined)
        throw new Error("Cube contains an unsupported sticker color or face.");
      const [row, col] = mapping.cell(cubie.position);
      if (
        row < 0 ||
        row > 2 ||
        col < 0 ||
        col > 2 ||
        faces[mapping.face][row][col] !== -1
      )
        throw new Error("Cube sticker positions are inconsistent.");
      faces[mapping.face][row][col] = colorId;
    }
  }
  if (faces.some((face) => face.some((row) => row.some((color) => color < 0))))
    throw new Error("Cube state is incomplete.");
  return faces;
}

export function getMoveAnimation(token) {
  const [axis, layer, clockwise] = MOVE[token[0]] || [];
  if (axis === undefined) throw new Error(`Invalid move notation: ${token}`);
  const turns = token.endsWith("2") ? 2 : 1;
  const direction = token.endsWith("'") ? -clockwise : clockwise;
  return { axis, layer, angle: (direction * turns * Math.PI) / 2 };
}

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
