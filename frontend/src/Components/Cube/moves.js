export function rotateR(cube) {

    return cube.map(cubie => {

        // Leave every other cubie untouched
        if (cubie.position[0] !== 1)
            return cubie;

        // Copy the cubie
        const newCubie = {
            ...cubie,
            position: [...cubie.position],
            stickers: { ...cubie.stickers }
        };

        const [x, y, z] = newCubie.position;

        // Rotate around X-axis
        newCubie.position = [x, z, -y];
        console.log(newCubie.position);
        
        return newCubie;

    });

}