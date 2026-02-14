#pragma once

#include "../util/includes.hpp"
#include "../util/types.hpp"
#include "chunk.hpp"
#include "../util/perlin.hpp"

namespace Minecraft {
	namespace World {
		using SetQuery = std::function<void(int, int, int, Block::BlockID)>;

		class WorldGen {
			public:
				static void chunkGen(const ChunkCoord &playerChunk, SetQuery query, u64 seed);

			private:
		};
	} // namespace World
} // namespace Minecraft