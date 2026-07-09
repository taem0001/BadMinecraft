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

		WorldGen::WorldGen(u64 sd, int o_height, double s_height, double p_height, double l_height, int o_biome,
						   double s_biome, double p_biome, double l_biome)
			: seed(sd), octaves_height(o_height), scale_height(s_height), persistance_height(p_height),
			  lacunarity_height(l_height), octaves_biome(o_biome), scale_biome(s_biome), persistance_biome(p_biome),
			  lacunarity_biome(l_biome) {
			std::mt19937_64 worldRng(sd);
			std::uniform_int_distribution<int> offsetDist(-100000, 100000);

			for (int i = 0; i < o_height; i++) {
				std::array<int, 2> offsets = {offsetDist(worldRng), offsetDist(worldRng)};
				octaveOffsetHeight.push_back(offsets);
			}
			for (int i = 0; i < o_biome; i++) {
				std::array<int, 2> offsets = {offsetDist(worldRng), offsetDist(worldRng)};
				octaveOffsetBiome.push_back(offsets);
			}
		}

		void WorldGen::chunkGen(const std::shared_ptr<Chunk> &playerChunk) {
			u64 chunkSeed = hashChunk(seed, playerChunk->coord.x, playerChunk->coord.z);
			std::mt19937_64 rng(chunkSeed);
			std::uniform_int_distribution<int> dirtDist(3, 5);

			std::cout << INFO << " Generating chunk at: (" << playerChunk->coord.x << "; " << playerChunk->coord.z
					  << ")" << std::endl;

			for (int lx = 0; lx < CHUNK_MAX_X; lx++) {
				for (int lz = 0; lz < CHUNK_MAX_Z; lz++) {
					int wx = playerChunk->coord.x * CHUNK_MAX_X + lx;
					int wz = playerChunk->coord.z * CHUNK_MAX_Z + lz;

					double freqH = 1;
					double ampH = 1;
					double maxAmpH = 0;
					double noiseH = 0;

					for (int i = 0; i < octaves_height; i++) {
						double sampleX = wx / scale_height * freqH + octaveOffsetHeight[i][0];
						double sampleZ = wz / scale_height * freqH + octaveOffsetHeight[i][1];
						double perlinVal = Perlin::perlin(sampleX, 0.0, sampleZ);
						noiseH += perlinVal * ampH;

						maxAmpH += ampH;
						ampH *= persistance_height;
						freqH *= lacunarity_height;
					}

					double freqB = 1;
					double ampB = 1;
					double maxAmpB = 0;
					double noiseB = 0;

					for (int i = 0; i < octaves_biome; i++) {
						double sampleX = wx / scale_biome * freqB + octaveOffsetBiome[i][0];
						double sampleZ = wz / scale_biome * freqB + octaveOffsetBiome[i][1];
						double perlinVal = Perlin::perlin(sampleX, 0.0, sampleZ);
						noiseB += perlinVal * ampB;

						maxAmpB += ampB;
						ampB *= persistance_biome;
						freqB *= lacunarity_biome;
					}

					double h01 = (noiseH / maxAmpH + 1.0) * 0.5;
					h01 = std::clamp(h01, 0.0, 1.0);

					double b01 = (noiseB / maxAmpB + 1.0) * 0.5;
					b01 = std::clamp(b01, 0.0, 1.0);

					double h = h01 * (64 - 1);
					int height = (int)std::floor(h);
					height = std::max(height, 1);

					int dirtOffset = dirtDist(rng);

					for (int y = height - 1; y >= 0; y--) {
						if (y == height - 1) {
							if (b01 >= 0 && b01 < 0.5)
								playerChunk->setLocalBlock(lx, y, lz, Block::GRASS);
							else
								playerChunk->setLocalBlock(lx, y, lz, Block::SAND);
						} else if (y > height - dirtOffset) {
							if (playerChunk->getLocalBlock(lx, height - 1, lz) == Block::GRASS)
								playerChunk->setLocalBlock(lx, y, lz, Block::DIRT);
							else
								playerChunk->setLocalBlock(lx, y, lz, Block::SAND);
						} else {
							playerChunk->setLocalBlock(lx, y, lz, Block::STONE);
						}
					}
				}
			}
		}
	} // namespace World
} // namespace Minecraft