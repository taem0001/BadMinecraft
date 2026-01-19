#pragma once

#include "gfx.hpp"
#include "vbo.hpp"

namespace Minecraft {
	namespace GFX {
		struct VAO {
				GLuint handle;
		};

		struct VAO createVAO();
		void bindVAO(struct VAO);
		void attrVAO(struct VAO vao, struct VBO vbo, struct VBO ebo,
					 GLuint index, GLint size, GLenum type, GLsizei offset);
		void destroyVAO(struct VAO);
	} // namespace GFX
} // namespace Minecraft