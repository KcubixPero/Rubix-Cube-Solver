import { useState } from "react";
import Cubie from "./Cubie";
import { createSolvedCube } from "./cubeState";
import { SPACING } from "./constants";

export default function Cube() {

    const [cube, setCube] = useState(createSolvedCube());

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