#include "../../include/world/chunk.hpp"

namespace Minecraft {
	namespace World {
		constexpr int Chunk::index(int x, int y, int z) {
			return x + CHUNK_MAX_X * (y + CHUNK_MAX_Y * z);
		}

		u8 Chunk::getBlockID(int x, int y, int z) {
			return blocks[index(x, y, z)];
		}

		void Chunk::setBlockID(int x, int y, int z, u8 blockID) {
			blocks[index(x, y, z)] = blockID;
		}

		void Chunk::generateMesh() {
			for (int z = 0; z < CHUNK_MAX_Z; z++) {
				for (int y = 0; y < CHUNK_MAX_Y; y++) {
					for (int x = 0; x < CHUNK_MAX_X; x++) {
						u8 id = getBlockID(x, y, z);
						if (id == AIR) {
							continue;
						}

						if (isAir(x, y, z + 1))
							pushFace(x, y, z, PZ);
						if (isAir(x, y, z - 1))
							pushFace(x, y, z, NZ);
						if (isAir(x + 1, y, z))
							pushFace(x, y, z, PX);
						if (isAir(x - 1, y, z))
							pushFace(x, y, z, NX);
						if (isAir(x, y + 1, z))
							pushFace(x, y, z, PY);
						if (isAir(x, y - 1, z))
							pushFace(x, y, z, NY);
					}
				}
			}
		}

		void Chunk::pushFace(int x, int y, int z, FaceDir dir) {
			const float *face = face_verts[dir - LASTBLOCK];

			static int tri[] = {0, 1, 2, 0, 2, 3};

			for (int i = 0; i < 6; i++) {
				int v = tri[i];

				struct Vertex vertex;
				vertex.px = face[3 * v + 0] + x + 0.5f + offset[0];
				vertex.py = face[3 * v + 1] + y + 0.5f + offset[1];
				vertex.pz = face[3 * v + 2] + z + 0.5f + offset[2];

				switch (v) {
				case 0:
					vertex.tx = 1;
					vertex.ty = 1;
					break;
				case 1:
					vertex.tx = 1;
					vertex.ty = 0;
					break;
				case 2:
					vertex.tx = 0;
					vertex.ty = 0;
					break;
				case 3:
					vertex.tx = 0;
					vertex.ty = 1;
				default:
					break;
				}

				vertices.push_back(vertex);
			}
		}

		// TODO: Handle chunk borders
		bool Chunk::isAir(int x, int y, int z) {
			if (x < 0 || x >= CHUNK_MAX_X)
				return true;
			if (y < 0 || y >= CHUNK_MAX_Y)
				return true;
			if (z < 0 || z >= CHUNK_MAX_Z)
				return true;

			return blocks[index(x, y, z)] == AIR;
		}
	} // namespace World
} // namespace Minecraft