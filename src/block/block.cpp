#include "../../include/block/block.hpp"

namespace Minecraft {
	namespace Block {
		TexCoord getTexCoord(BlockType type, FaceDir dir) {
			switch (type) {
			case DIRT:
				return {0, 1};
			case STONE:
				return {1, 1};
			case GRASS:
				if (dir == FaceDir::PY)
					return {1, 0};
				else if (dir == FaceDir::NY)
					return {0, 1};
				else
					return {0, 0};
			default:
				return {0, 0};
			}
		}
	} // namespace Block
} // namespace Minecraft