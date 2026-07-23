import { COLORS } from "./constants";

export function createSolvedCube() {

    const cubies = [];

    let id = 0;

    for (let x = -1; x <= 1; x++) {

        for (let y = -1; y <= 1; y++) {

            for (let z = -1; z <= 1; z++) {

                // Skip hidden core
                if (x === 0 && y === 0 && z === 0)
                    continue;

                const stickers = {};

                if (x === -1) stickers.left = COLORS.BLUE;
                if (x === 1) stickers.right = COLORS.GREEN;

                if (y === 1) stickers.top = COLORS.RED;
                if (y === -1) stickers.bottom = COLORS.ORANGE;

                if (z === 1) stickers.front = COLORS.WHITE;
                if (z === -1) stickers.back = COLORS.YELLOW;

                cubies.push({
                    id: id++,
                    position: [x, y, z],
                    stickers
                });

            }

        }

    }

    return cubies;

}