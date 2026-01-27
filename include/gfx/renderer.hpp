#pragma once

#include "../entity/camera.hpp"
#include "../util/macros.hpp"
#include "../world/world.hpp"
#include "../world/chunk.hpp"
#include "../world/block.hpp"
#include "gfx.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vao.hpp"
#include "vbo.hpp"

namespace Minecraft {
	namespace GFX {
		enum ShaderType { TRIANGLE = 0 };
#define SHADERNUM (TRIANGLE + 1)
#define WIDTH	  1280
#define HEIGHT	  720

		class Renderer {
			public:
				Renderer();
				~Renderer();
				void renderWorld(World::World &world);
				void renderChunk(World::Chunk &chunk);

				Entity::Camera &getCam() { return cam; }
				Shader *getShaders() { return shader; }
				struct VBO getVBO() { return vbo; }
				struct VBO getEBO() { return ebo; }
				struct VAO getVAO() { return vao; }

			private:
				Shader shader[SHADERNUM];
				Texture texture;
				Entity::Camera cam;
				struct VBO vbo, ebo;
				struct VAO vao;
		};
	} // namespace GFX
} // namespace Minecraft
