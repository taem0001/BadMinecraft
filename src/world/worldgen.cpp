#include "../../include/world/worldgen.hpp"

namespace Minecraft {
	namespace World {
        u64 hashChunk(u64 seed, int cx, int cz) {
            u64 h = seed;
            h ^= (u64)cx * 0x9E3779B97F4A7C15ULL;
            h ^= (u64)cz * 0xC2B2AE3D27D4EB4FULL;
            h ^= (h >> 30);
            h *= 0xBF58476D1CE4E5B9ULL;
            h ^= (h >> 27);
            h *= 0x94D049BB133111EBULL;
            h ^= (h >> 31);
            return h;
        }
        
        WorldGen::WorldGen(u64 sd, int o, double s, double p, double l) : seed(sd), octaves(o), scale(s), persistance(p), lacunarity(l) {
            std::mt19937_64 worldRng(sd);
			std::uniform_int_distribution<int> offsetDist(-100000, 100000);

            for (int i = 0; i < o; i++) {
                std::array<int, 2> offsets = {offsetDist(worldRng), offsetDist(worldRng)}; 
                octaveOffset.push_back(offsets);
            }
        }

		void WorldGen::chunkGen(const ChunkCoord &playerChunk, SetQuery query) {
			u64 chunkSeed = hashChunk(seed, playerChunk.x, playerChunk.z);
			std::mt19937_64 rng(chunkSeed);
			std::uniform_int_distribution<int> dirtDist(3, 5);

			std::cout << "[INFO] Generating chunk at: (" << playerChunk.x
					  << "; " << playerChunk.z << ")" << std::endl;

			for (int lx = 0; lx < CHUNK_MAX_X; lx++) {
				for (int lz = 0; lz < CHUNK_MAX_Z; lz++) {
					int wx = playerChunk.x * CHUNK_MAX_X + lx;
					int wz = playerChunk.z * CHUNK_MAX_Z + lz;

					double freq = 1;
					double amp = 1;
					double noiseH = 0;
					double maxAmp = 0;

					for (int i = 0; i < octaves; i++) {
						double sampleX =
							wx / scale * freq + octaveOffset[i][0];
						double sampleZ =
							wz / scale * freq + octaveOffset[i][1];
						double perlinVal =
							Perlin::perlin(sampleX, 0.0, sampleZ);
						noiseH += perlinVal * amp;

						maxAmp += amp;
						amp *= persistance;
						freq *= lacunarity;
					}

					double h01 = (noiseH / maxAmp + 1.0) * 0.5;
					h01 = std::clamp(h01, 0.0, 1.0);

					double h = h01 * (CHUNK_MAX_Y - 1);
					int height = (int)std::floor(h);
					height = std::max(height, 1);

					int dirtOffset = dirtDist(rng);

					for (int y = 0; y < height; y++) {
						if (y == height - 1) {
							query(wx, y, wz, Block::GRASS);
						} else if (y > height - dirtOffset) {
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