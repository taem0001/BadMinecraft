#include "../../include/meshing/chunkmesher.hpp"

namespace Minecraft {
	namespace Meshing {
		static constexpr int SX = CHUNK_MAX_X;
		static constexpr int SY = CHUNK_MAX_Y;
		static constexpr int SZ = CHUNK_MAX_Z;
		static constexpr int ATLAS_W = 32;
		static constexpr int ATLAS_H = 32;
		static constexpr int TILE_W = 16;
		static constexpr int TILE_H = 16;

		static constexpr unsigned int INDICES[] = {0, 1, 2, 0, 2, 3};

		static constexpr float FACE_PZ[] = {
			// +Z
			+0.5f, +0.5f, +0.5f, +0.5f, -0.5f, +0.5f,
			-0.5f, -0.5f, +0.5f, -0.5f, +0.5f, +0.5f,
		};
		static constexpr float FACE_NZ[] = {
			// -Z
			+0.5f, +0.5f, -0.5f, -0.5f, +0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f, +0.5f, -0.5f, -0.5f,
		};
		static constexpr float FACE_PY[] = {
			// +Y
			+0.5f, +0.5f, +0.5f, -0.5f, +0.5f, +0.5f,
			-0.5f, +0.5f, -0.5f, +0.5f, +0.5f, -0.5f,
		};
		static constexpr float FACE_NY[] = {
			// -Y
			+0.5f, -0.5f, +0.5f, +0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, +0.5f,
		};
		static constexpr float FACE_PX[] = {
			// +X
			+0.5f, +0.5f, +0.5f, +0.5f, +0.5f, -0.5f,
			+0.5f, -0.5f, -0.5f, +0.5f, -0.5f, +0.5f,
		};
		static constexpr float FACE_NX[] = {
			// -X
			-0.5f, +0.5f, +0.5f, -0.5f, -0.5f, +0.5f,
			-0.5f, -0.5f, -0.5f, -0.5f, +0.5f, -0.5f,
		};

		static const float *faceVerts(FaceDir dir) {
			switch (dir) {
			case FaceDir::PZ:
				return FACE_PZ;
			case FaceDir::NZ:
				return FACE_NZ;
			case FaceDir::PX:
				return FACE_PX;
			case FaceDir::NX:
				return FACE_NX;
			case FaceDir::PY:
				return FACE_PY;
			case FaceDir::NY:
				return FACE_NY;
			}
			return FACE_PZ;
		}

		bool ChunkMesher::isAir(const BlockQuery &query, int x, int y, int z) {
			return query(x, y, z) == Block::AIR;
		}

		MeshData ChunkMesher::build(const World::Chunk &chunk,
									const BlockQuery &query) {
			MeshData out;
			out.vertices.clear();
			out.indices.clear();

			const int baseX = chunk.coord.x * SX;
			const int baseY = chunk.coord.y * SY;
			const int baseZ = chunk.coord.z * SZ;

			for (int z = 0; z < SZ; z++) {
				for (int y = 0; y < SY; y++) {
					for (int x = 0; x < SX; x++) {
						Block::BlockID id = chunk.getLocalBlock(x, y, z);
						if (id == Block::AIR) continue;

						int wx = x + baseX;
						int wy = y + baseY;
						int wz = z + baseZ;

						if (isAir(query, wx, wy, wz + 1))
							emitFace(out, x, y, z, FaceDir::PZ, id);
						if (isAir(query, wx, wy, wz - 1))
							emitFace(out, x, y, z, FaceDir::NZ, id);
						if (isAir(query, wx, wy + 1, wz))
							emitFace(out, x, y, z, FaceDir::PY, id);
						if (isAir(query, wx, wy - 1, wz))
							emitFace(out, x, y, z, FaceDir::NY, id);
						if (isAir(query, wx + 1, wy, wz))
							emitFace(out, x, y, z, FaceDir::PX, id);
						if (isAir(query, wx - 1, wy, wz))
							emitFace(out, x, y, z, FaceDir::NX, id);
					}
				}
			}

			return out;
		}

		void ChunkMesher::emitFace(MeshData &out, int bx, int by, int bz,
								   FaceDir dir, Block::BlockID texid) {
			const float *f = faceVerts(dir);

			// Center the templates
			const float ox = (float)bx + 0.5f;
			const float oy = (float)by + 0.5f;
			const float oz = (float)bz + 0.5f;

			unsigned int base = (unsigned int)out.vertices.size();

			Block::TexCoord tc = Block::getTexCoord((Block::BlockType)texid);

			float u0 = (float)tc.u * TILE_W / ATLAS_W;
			float u1 = ((float)tc.u + 1.0f) * TILE_W / ATLAS_W;
			float v0 = 1.0f - ((float)tc.v * TILE_H / ATLAS_H);
			float v1 = 1.0f - (((float)tc.v + 1.0f) * TILE_H / ATLAS_H);

			float uv[]{
				u1, v1, // Top right
				u1, v0, // Bottom right
				u0, v0, // Bottom left
				u0, v1	// Top left
			};

			for (int i = 0; i < 4; i++) {
				Vertex v;
				v.position = {f[3 * i + 0] + ox, f[3 * i + 1] + oy,
							  f[3 * i + 2] + oz};
				v.uv = {uv[2 * i + 0], uv[2 * i + 1]};
				v.texid = texid;

				out.vertices.push_back(v);
			}

			for (unsigned int idx : INDICES) {
				out.indices.push_back(base + idx);
			}
		}
	} // namespace Meshing
} // namespace Minecraft