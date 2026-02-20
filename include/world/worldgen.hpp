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
				WorldGen(u64 seed, int octaves = 4, double scale = 50.0,
						 double persistance = 0.5, double lacunarity = 2.0);
				void chunkGen(const std::shared_ptr<Chunk> &playerChunk);

			private:
				u64 seed;
				int octaves;
				double scale, persistance, lacunarity;
				std::vector<std::array<int, 2>> octaveOffset;
		};
	} // namespace World
} // namespace Minecraft