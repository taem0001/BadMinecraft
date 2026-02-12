#include "../../include/world/worldgen.hpp"

namespace Minecraft {
    namespace World {
        void WorldGen::chunkGen(int playerX, int playerZ, SetQuery query,
                                u64 seed) {
            std::srand(seed);

            const double scale = 59.0, persistance = 0.2, lacunarity = 1.7;
            const int octaves = 4;

            std::array<std::array<double, 2>, octaves> octaveOffsets;
            for (int i = 0; i < octaves; i++) {
                double offsetX =
                    -100000 + (std::rand() % (100000 - (-100000) + 1));
                double offsetZ =
                    -100000 + (std::rand() % (100000 - (-100000) + 1));
                octaveOffsets[i][0] = offsetX;
                octaveOffsets[i][1] = offsetZ;
            }

            int cx = floorDiv(playerX, CHUNK_MAX_X);
            int cz = floorDiv(playerZ, CHUNK_MAX_Z);
            
            for (int lx = 0; lx < CHUNK_MAX_X; lx++) {
                for (int lz = 0; lz < CHUNK_MAX_Z; lz++) {
                    int wx = cx * CHUNK_MAX_X + lx;
                    int wz = cz * CHUNK_MAX_Z + lz;

                    double freq = 1;
                    double amp = 1;
                    double noiseH = 0;

                    for (int i = 0; i < octaves; i++) {
                        double sampleX = wx / scale * freq + octaveOffsets[i][0];
                        double sampleZ = wz / scale * freq + octaveOffsets[i][1];
                        double perlinVal = Perlin::perlin(sampleX, 0.0, sampleZ);
                        noiseH += perlinVal * amp;
                        amp *= persistance;
                        freq *= lacunarity;
                    }

                    noiseH = (noiseH < 0) ? std::fmod(std::abs(noiseH), 1.0) : std::fmod(noiseH, 1.0);
                    noiseH *= CHUNK_MAX_Y;
                    int height = std::round(noiseH);
                    if (height == 0) height = 1; // TODO: This is a temporary fix until negative y coordinates are allowed

                    for (int y = 0; y < height; y++) {
                        if (y == height - 1) {
                            query(wx, y, wz, Block::GRASS);
                        } else if (y > height - 3) {
                            query(wx, y, wz, Block::DIRT);
                        } else {
                            query(wx, y, wz, Block::STONE);
                        }
                    }
                }
            }
        }
    } // namespace World
} // namespace Minecraft