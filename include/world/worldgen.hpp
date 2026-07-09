#pragma once

#include "../util/includes.hpp"
#include "../util/perlin.hpp"
#include "../util/types.hpp"
#include "chunk.hpp"

struct Chunk;

namespace Minecraft {
	namespace World {
		class WorldGen {
			public:
				WorldGen(u64 seed, int o_height = 4, double s_height = 50.0, double p_height = 0.5,
						 double l_height = 2.0, int o_biome = 4, double s_biome = 50.0, double p_biome = 0.5,
						 double l_biome = 2.0);
				void chunkGen(const std::shared_ptr<Chunk> &playerChunk);

			private:
				u64 seed;
				int octaves_height, octaves_biome;
				double scale_height, persistance_height, lacunarity_height, scale_biome, persistance_biome,
					lacunarity_biome;
				std::vector<std::array<int, 2>> octaveOffsetHeight, octaveOffsetBiome;
		};
	} // namespace World
} // namespace Minecraft