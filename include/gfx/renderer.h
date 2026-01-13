#pragma once

#include "gfx.h"
#include "shader.h"
#include "vao.h"
#include "vbo.h"

namespace Minecraft {
	namespace GFX {
		class Renderer {
			public:
				Renderer();
				~Renderer();
				void renderTriangle(); // TODO: This is only for testing shaders
				
				// TODO: Maybe just make this a struct
				struct Shader *shader;
				struct VBO vbo;
				struct VAO vao;
		};
	} // namespace GFX
} // namespace Minecraft