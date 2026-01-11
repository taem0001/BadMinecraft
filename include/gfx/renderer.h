#pragma once

#include "gfx.h"
#include "vao.h"
#include "vbo.h"

namespace Minecraft {
	namespace GFX {
		class Renderer {
			public:
				Renderer();
				~Renderer();

			private:
				struct VBO vbo;
				struct VAO vao;
		};
	} // namespace GFX
} // namespace Minecraft