#pragma once

#include "../meshing/chunkmesher.hpp"
#include "../util/includes.hpp"
#include "../util/macros.hpp"
#include "../world/world.hpp"
#include "../world/worldgen.hpp"
#include "camera.hpp"
#include "chunkmesh.hpp"
#include "gfx.hpp"
#include "shader.hpp"
#include "texture.hpp"

namespace Minecraft {
	namespace GFX {
		struct Plane {
				glm::vec3 normal = {0, 1, 0};
				float distance = 0.0;
		};

		struct Frustum {
				Plane p[6];
		};

		struct aabb {
				glm::vec3 mn;
				glm::vec3 mx;
		};

		class Renderer {
			public:
				Renderer();

				void updateChunks(World::World &world);
				void renderWorld();

				Camera &getCam() { return cam; }
				const Camera &getCam() const { return cam; }

				void setViewPortSize(int w, int h) {
					this->width = w;
					this->height = h;
				}

			private:
				std::unordered_map<World::ChunkCoord, ChunkMesh> meshes;
				Camera cam;

				Shader shader;
				Texture texture;

				int width, height;
				double const RENDER_RADIUS;

				Plane normalizePlane(const glm::vec4 &v);
				Frustum getFrustum();
				aabb getAABB(const World::ChunkCoord &coord);

				bool aabbOutsidePlane(const aabb &b, const Plane& p);
				bool aabbInFrustum(const aabb &b, const Frustum &f);
		};
	} // namespace GFX
} // namespace Minecraft
