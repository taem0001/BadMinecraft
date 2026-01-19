#pragma once

#include "../entity/camera.hpp"
#include "../util/macros.hpp"
#include "../world/world.hpp"
#include "gfx.hpp"
#include "shader.hpp"
#include "vao.hpp"
#include "vbo.hpp"

namespace Minecraft {
	namespace GFX {
		enum ShaderType { TRIANGLE = 0 };
#define SHADERNUM (TRIANGLE + 1)
#define WIDTH	  1280
#define HEIGHT	  720

		struct Renderer {
				~Renderer();
				void init();
				void renderWorld(World::World &world);

				struct Shader shader[SHADERNUM];
				struct VBO vbo, ebo;
				struct VAO vao;

				Entity::Camera cam;
		};
	} // namespace GFX
} // namespace Minecraft
