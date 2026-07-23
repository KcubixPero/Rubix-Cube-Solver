import { Canvas } from "@react-three/fiber";
import { OrbitControls } from "@react-three/drei";
import Cube from "./components/Cube/Cube";

export default function App() {
  return (
    <div
      style={{
        width: "100vw",
        height: "100vh",
        background: "#1b1b1b"
      }}
    >
      <Canvas camera={{ position: [7, 6, 7], fov: 45 }}>

        <ambientLight intensity={2} />

        <directionalLight
          position={[6, 8, 5]}
          intensity={3}
        />

        <directionalLight
          position={[-5, 5, -5]}
          intensity={2}
        />

        <Cube />

        <OrbitControls
          enableDamping={false}
          rotateSpeed={1.8}
          zoomSpeed={1.5}
          panSpeed={1.5}
        />

      </Canvas>
    </div>
  );
}