import Cubie from "./Cubie";
import { createSolvedCube } from "./cubeState";
import { SPACING } from "./constants";
import { useEffect, useState } from "react";
import { rotateR } from "./moves";

export default function Cube() {

    const [cube, setCube] = useState(createSolvedCube());

    useEffect(() => {

        function handleKeyDown(event) {

            if (event.key === "r" || event.key === "R") {

                setCube(previousCube => rotateR(previousCube));

            }

        }

        window.addEventListener("keydown", handleKeyDown);

        return () => {
            window.removeEventListener("keydown", handleKeyDown);
        };

    }, []);

    return (

        <group>

            {cube.map(cubie => (

                <Cubie
                    key={cubie.id}
                    cubie={{
                        ...cubie,
                        position: [
                            cubie.position[0] * SPACING,
                            cubie.position[1] * SPACING,
                            cubie.position[2] * SPACING
                        ]
                    }}
                />

            ))}

        </group>

    );

}