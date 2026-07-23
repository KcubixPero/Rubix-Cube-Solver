export default function Sticker({ position, rotation, color }) {

    return (

        <mesh
            position={position}
            rotation={rotation}
        >

            <boxGeometry args={[0.82,0.82,0.03]} />

            <meshBasicMaterial color={color} />

        </mesh>

    );

}