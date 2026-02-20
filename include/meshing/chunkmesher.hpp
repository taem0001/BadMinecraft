#pragma once

#include "../block/block.hpp"
#include "../util/includes.hpp"
#include "../world/chunk.hpp"
#include "../world/world.hpp"
#include "meshdata.hpp"

namespace Minecraft {
	namespace Meshing {
		using BlockQuery = std::function<Block::BlockID(int, int, int)>;

		class ChunkMesher {
			public:
				static MeshData
				build(const World::NeighborSnapshot &snapshot);

			private:
				static bool isAir(const World::NeighborSnapshot &snapshot,
								  int x, int y, int z);
				static void emitFace(MeshData &out, int bx, int by, int bz,
									 FaceDir dir, Block::BlockType texid);
		};
	} // namespace Meshing
} // namespace Minecraft
