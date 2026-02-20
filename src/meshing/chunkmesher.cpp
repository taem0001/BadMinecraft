#include "../../include/meshing/chunkmesher.hpp"

namespace Minecraft {
	namespace Meshing {
		static constexpr int SX = CHUNK_MAX_X;
		static constexpr int SZ = CHUNK_MAX_Z;
		static constexpr int SY = CHUNK_MAX_Y;
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

		bool ChunkMesher::isAir(const World::NeighborSnapshot &snapshot, int x,
								int y, int z) {
			if (y < 0 || y >= SY) return true;
			if (x == -1) {
				if (!snapshot.nx) return true;
				return snapshot.nx->getLocalBlock(SX - 1, y, z) == Block::AIR;
			}
			if (x == SX) {
				if (!snapshot.px) return true;
				return snapshot.px->getLocalBlock(0, y, z) == Block::AIR;
			}
			if (z == -1) {
				if (!snapshot.nz) return true;
				return snapshot.nz->getLocalBlock(x, y, SZ - 1) == Block::AIR;
			}
			if (z == SZ) {
				if (!snapshot.pz) return true;
				return snapshot.pz->getLocalBlock(x, y, 0) == Block::AIR;
			}
			return snapshot.center->getLocalBlock(x, y, z) == Block::AIR;
		}

		MeshData ChunkMesher::build(const World::NeighborSnapshot &snapshot) {
			MeshData out;
			out.vertices.clear();
			out.indices.clear();

			for (int z = 0; z < SZ; z++) {
				for (int y = 0; y < SY; y++) {
					for (int x = 0; x < SX; x++) {
						Block::BlockID id =
							snapshot.center->getLocalBlock(x, y, z);
						if (id == Block::AIR) continue;

						if (isAir(snapshot, x, y, z + 1))
							emitFace(out, x, y, z, FaceDir::PZ,
									 (Block::BlockType)id);
						if (isAir(snapshot, x, y, z - 1))
							emitFace(out, x, y, z, FaceDir::NZ,
									 (Block::BlockType)id);
						if (isAir(snapshot, x, y + 1, z))
							emitFace(out, x, y, z, FaceDir::PY,
									 (Block::BlockType)id);
						if (isAir(snapshot, x, y - 1, z))
							emitFace(out, x, y, z, FaceDir::NY,
									 (Block::BlockType)id);
						if (isAir(snapshot, x + 1, y, z))
							emitFace(out, x, y, z, FaceDir::PX,
									 (Block::BlockType)id);
						if (isAir(snapshot, x - 1, y, z))
							emitFace(out, x, y, z, FaceDir::NX,
									 (Block::BlockType)id);
					}
				}
			}

			return out;
		}

		void ChunkMesher::emitFace(MeshData &out, int bx, int by, int bz,
								   FaceDir dir, Block::BlockType texid) {
			const float *f = faceVerts(dir);

			// Center the templates
			const float ox = (float)bx + 0.5f;
			const float oy = (float)by + 0.5f;
			const float oz = (float)bz + 0.5f;

			const unsigned int base = (unsigned int)out.vertices.size();

			Block::TexCoord tc =
				Block::getTexCoord((Block::BlockType)texid, dir);

			const float u0 = (float)tc.u * TILE_W / ATLAS_W;
			const float u1 = ((float)tc.u + 1.0f) * TILE_W / ATLAS_W;
			const float v0 = 1.0f - ((float)tc.v * TILE_H / ATLAS_H);
			const float v1 = 1.0f - (((float)tc.v + 1.0f) * TILE_H / ATLAS_H);

			float uv[] = {
				u1, v1, // Top right
				u1, v0, // Bottom right
				u0, v0, // Bottom left
				u0, v1	// Top left
			};

			for (int i = 0; i < 4; i++) {
				Vertex v;
				v.position = {f[3 * i] + ox, f[3 * i + 1] + oy,
							  f[3 * i + 2] + oz};
				if (dir == FaceDir::NZ || dir == FaceDir::PX ||
					dir == FaceDir::PY) {
					int idx = (4 - i) % 4;
					v.uv = {uv[2 * idx], uv[2 * idx + 1]};
				} else {
					v.uv = {uv[2 * i], uv[2 * i + 1]};
				}

				v.texid = texid;

				out.vertices.push_back(v);
			}

			for (unsigned int idx : INDICES) {
				out.indices.push_back(base + idx);
			}
		}
	} // namespace Meshing
} // namespace Minecraft
