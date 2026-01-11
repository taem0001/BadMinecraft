#include "../../include/gfx/vao.h"

namespace Minecraft {
	namespace GFX {
		struct VAO createVAO() {
			struct VAO self;
			glGenVertexArrays(1, &self.handle);
			return self;
		}

		void bindVAO(struct VAO self) { glBindVertexArray(self.handle); }

		void attrVAO(struct VAO vao, struct VBO vbo, GLuint index, GLint size,
					 GLenum type, GLsizei offset) {
			bindVAO(vao);
			bindVBO(vbo);

			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, size, type, GL_FALSE, offset, NULL);
		}

		void destroyVAO(struct VAO self) {
			glDeleteVertexArrays(1, &self.handle);
		}
	} // namespace GFX
} // namespace Minecraft