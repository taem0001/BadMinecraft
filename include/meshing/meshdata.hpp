#pragma once

#include "../block/block.hpp"
#include "../util/includes.hpp"

namespace Minecraft {
	namespace Meshing {
		struct Vertex {
				glm::vec3 position;
				glm::vec2 uv;
                Block::BlockID texid;
		};

		struct MeshData {
				std::vector<struct Vertex> vertices;
				std::vector<unsigned int> indices;
		};
	} // namespace Meshing
} // namespace Minecraft