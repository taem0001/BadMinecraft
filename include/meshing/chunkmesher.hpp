#pragma once

#include "../block/block.hpp"
#include "../world/chunk.hpp"
#include "../world/world.hpp"
#include "meshdata.hpp"

namespace Minecraft {
	namespace Meshing {
		enum class FaceDir { PZ, NZ, PX, NX, PY, NY };

		using BlockQuery = std::function<Block::BlockID(int, int, int)>;

		class ChunkMesher {
			public:
				static MeshData build(const World::Chunk &chunk, const BlockQuery &query);

			private:
				static bool isAir(const BlockQuery &query, int x, int y,
								  int z);
				static void emitFace(MeshData &out, int bx, int by, int bz,
									 FaceDir dir, Block::BlockID texid);
		};
	} // namespace Meshing
} // namespace Minecraft