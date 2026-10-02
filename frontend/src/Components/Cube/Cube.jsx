import Cubie from "./Cubie";
import { SPACING } from "./constants";

export default function Cube({ cubies }) {
  return (
    <group>
      {cubies.map((cubie) => (
        <Cubie
          key={cubie.id}
          cubie={{
            ...cubie,
            position: cubie.position.map((value) => value * SPACING),
          }}
        />
      ))}
    </group>
  );
}
