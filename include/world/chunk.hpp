#pragma once

#include "../util/types.hpp"
#include "block.hpp"

namespace Minecraft {
	namespace World {
		static constexpr int CHUNK_MAX_X = 16;
		static constexpr int CHUNK_MAX_Y = 16;
		static constexpr int CHUNK_MAX_Z = 16;

		struct Chunk {
                glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f);
				std::array<u8, CHUNK_MAX_X * CHUNK_MAX_Y * CHUNK_MAX_Z> blocks{};
                std::vector<struct Vertex> vertices;

				static constexpr int index(int x, int y, int z);
				u8 getBlockID(int x, int y, int z);
				void setBlockID(int x, int y, int z, u8 blockID);
                void generateMesh();
                void pushFace(int x, int y, int z, FaceDir dir);
                bool isAir(int x, int y, int z);
		};
	} // namespace World
} // namespace Minecraft