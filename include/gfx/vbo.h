#pragma once

#include "gfx.h"

namespace Minecraft {
	namespace GFX {
		struct VBO {
				GLuint handle;
                GLint type;
		};

		struct VBO createVBO(GLint);
        void bindVBO(struct VBO);
        void bufferVBO(struct VBO, void *, size_t);
        void destroyVBO(struct VBO);
	} // namespace GFX
} // namespace Minecraft