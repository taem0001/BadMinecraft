#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Minecraft {
	namespace World {
		enum BlockType { DIRT };

		static float vertices[] = {
			0.5f,  0.5f,  0.0f, // top right
			0.5f,  -0.5f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f, // bottom left
			-0.5f, 0.5f,  0.0f, // top left
			0.5f,  0.5f,  1.0f, // top right - behind
			0.5f,  -0.5f, 1.0f, // bottom right - behind
			-0.5f, -0.5f, 1.0f, // bottom left - behind
			-0.5f, 0.5f,  1.0f	// top left - behind
		};

		static float texcoords[] = {
			0.0f, 0.0f, // Lower left corner
			0.0f, 1.0f, // Upper left corner
			1.0f, 0.0f, // Lower right corner
			1.0f, 1.0f	// Upper right corner
		};

		static unsigned int indices[] = {
			0, 1, 3, 1, 2, 3, // Facing -z (north)
			4, 5, 7, 5, 6, 7, // Facing +z (south)
			5, 1, 0, 0, 4, 5, // Facing +x (east)
			1, 2, 5, 5, 2, 6, // Facing -y (down)
			0, 3, 4, 4, 3, 7, // Facing +y (up)
			6, 2, 3, 3, 7, 6  // Facing -x (west)
		};

		struct Block {
				BlockType type;
				glm::vec3 pos;
		};
	} // namespace World
} // namespace Minecraft