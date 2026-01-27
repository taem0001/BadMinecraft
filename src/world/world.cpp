#include "../../include/world/world.hpp"

namespace Minecraft {
	namespace World {
		World::World() {
			for (int z = 0; z < CHUNK_MAX_Z; z++) {
				for (int y = 0; y < CHUNK_MAX_Y; y++) {
					for (int x = 0; x < CHUNK_MAX_X; x++) {
						chunk.setBlockID(x, y, z, DIRT);
					}
				}
			}
		}

		struct Chunk World::getChunk() { return chunk; }
	} // namespace World
} // namespace Minecraft