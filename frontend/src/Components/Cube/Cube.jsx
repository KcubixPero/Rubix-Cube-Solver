import { useFrame } from "@react-three/fiber";
import { useEffect, useRef } from "react";
import Cubie from "./Cubie";
import { SPACING } from "./constants";
import { getMoveAnimation } from "./cubeState";

const DURATION = 420;
const AXES = ["x", "y", "z"];

export default function Cube({ cubies, animation, paused, onAnimationComplete }) {
  const layer = useRef();
  const progressTime = useRef(0);
  const completed = useRef(false);
  const move = animation ? getMoveAnimation(animation.token) : null;

  useEffect(() => {
    progressTime.current = 0;
    completed.current = false;
    if (layer.current) layer.current.rotation.set(0, 0, 0);
  }, [animation?.id]);

  useFrame((state, delta) => {
    if (!move || completed.current || !layer.current) return;
    const axis = AXES[move.axis];
    if (!paused) progressTime.current += delta;
    const progress = Math.min(progressTime.current / (DURATION / 1000), 1);
    const eased = progress * progress * (3 - 2 * progress);
    layer.current.rotation[axis] = move.angle * eased;
    if (progress === 1) {
      completed.current = true;
      onAnimationComplete(animation.id);
    }
  });

  const affected = move ? cubies.filter((cubie) => cubie.position[move.axis] === move.layer) : [];
  const unaffected = move ? cubies.filter((cubie) => cubie.position[move.axis] !== move.layer) : cubies;
  const renderCubie = (cubie) => <Cubie key={cubie.id} cubie={{ ...cubie, position: cubie.position.map((value) => value * SPACING) }} />;

  return (
    <group>
      {unaffected.map(renderCubie)}
      {move && <group ref={layer}>{affected.map(renderCubie)}</group>}
    </group>
  );
}
