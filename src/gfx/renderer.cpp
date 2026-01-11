#include "../../include/gfx/renderer.h"

namespace Minecraft {
	namespace GFX {
		Renderer::Renderer() {
			this->vbo = createVBO(GL_ARRAY_BUFFER);
			this->vao = createVAO();

			float points[] = {
				-0.5f, -0.5f, 0.0f, // left
				0.5f,  -0.5f, 0.0f, // right
				0.0f,  0.5f,  0.0f	// top
			};

			attrVAO(this->vao, this->vbo, 0, 3, GL_FLOAT, 0);
			bufferVBO(this->vbo, points, sizeof(points));
		}

		Renderer::~Renderer() {
			destroyVBO(this->vbo);
			destroyVAO(this->vao);
		}
	} // namespace GFX
} // namespace Minecraft