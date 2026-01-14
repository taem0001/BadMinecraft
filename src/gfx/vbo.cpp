#include "../../include/gfx/vbo.h"

namespace Minecraft {
	namespace GFX {
		struct VBO createVBO(GLint type) {
			struct VBO self = {.type = type};
			glGenBuffers(1, &self.handle);
			return self;
		}

		void bindVBO(struct VBO self) { glBindBuffer(self.type, self.handle); }

		void bufferVBO(struct VBO self, void *data, size_t size) {
			bindVBO(self);
			glBufferData(self.type, size, data, GL_STATIC_DRAW);
		}

		void destroyVBO(struct VBO self) { glDeleteBuffers(1, &self.handle); }
	} // namespace GFX
} // namespace Minecraft