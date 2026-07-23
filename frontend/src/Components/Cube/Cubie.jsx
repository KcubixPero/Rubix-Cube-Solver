import Sticker from "./Sticker";

export default function Cubie({ cubie }) {

    const { position, stickers } = cubie;

    return (

        <group position={position}>

            {/* Plastic Body */}

            <mesh>
                <boxGeometry args={[0.95, 0.95, 0.95]} />
                <meshStandardMaterial
                    color="#111111"
                    roughness={0.8}
                />
            </mesh>

            {/* Front */}

            {stickers.front && (
                <Sticker
                    color={stickers.front}
                    position={[0, 0, 0.48]}
                    rotation={[0, 0, 0]}
                />
            )}

            {/* Back */}

            {stickers.back && (
                <Sticker
                    color={stickers.back}
                    position={[0, 0, -0.48]}
                    rotation={[0, Math.PI, 0]}
                />
            )}

            {/* Right */}

            {stickers.right && (
                <Sticker
                    color={stickers.right}
                    position={[0.48, 0, 0]}
                    rotation={[0, Math.PI / 2, 0]}
                />
            )}

            {/* Left */}

            {stickers.left && (
                <Sticker
                    color={stickers.left}
                    position={[-0.48, 0, 0]}
                    rotation={[0, -Math.PI / 2, 0]}
                />
            )}

            {/* Top */}

            {stickers.top && (
                <Sticker
                    color={stickers.top}
                    position={[0, 0.48, 0]}
                    rotation={[-Math.PI / 2, 0, 0]}
                />
            )}

            {/* Bottom */}

            {stickers.bottom && (
                <Sticker
                    color={stickers.bottom}
                    position={[0, -0.48, 0]}
                    rotation={[Math.PI / 2, 0, 0]}
                />
            )}

        </group>

    );

}