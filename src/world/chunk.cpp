#include "../../include/world/chunk.hpp"

namespace Minecraft {
	namespace World {
		constexpr int Chunk::index(int x, int y, int z) {
			return x + CHUNK_MAX_X * (y + CHUNK_MAX_Y * z);
		}

		void Chunk::setLocalBlock(int lx, int ly, int lz, Block::BlockID id) {
			blocks[index(lx, ly, lz)] = id;
		}

		Block::BlockID Chunk::getLocalBlock(int lx, int ly, int lz) const {
			return blocks[index(lx, ly, lz)];
		}
	} // namespace World
} // namespace Minecraft