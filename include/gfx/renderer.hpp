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
        };
    } // namespace GFX
} // namespace Minecraft
