#include "../../include/gfx/renderer.hpp"

namespace Minecraft {
    namespace GFX {
        Renderer::Renderer()
            : cam(Entity::Camera(glm::vec3(40.0f, 10.0f, 40.0f))), width(WIDTH),
              height(HEIGHT), RENDER_RADIUS(SQUARE(2.5)) {
            shader.init("res/shaders/block.vert", "res/shaders/block.frag");
            texture.init("res/textures/blockatlas.png");
        }

        void Renderer::updateChunks(World::World &w) {
            auto blockQuery = [&](int x, int y, int z) {
                return w.getBlockWorld(x, y, z);
            };

            auto setQuery = [&](int x, int y, int z, Block::BlockID id) {
                return w.setBlockWorld(x, y, z, id);
            };

            World::ChunkCoord playerChunk = {floorDiv(cam.pos[0], CHUNK_MAX_X),
                                             floorDiv(cam.pos[1], CHUNK_MAX_Y),
                                             floorDiv(cam.pos[2], CHUNK_MAX_Z)};

            World::WorldGen::chunkGen(cam.pos[0], cam.pos[2], setQuery, w.getSeed());

            auto &chunks = w.getChunks();
            for (auto it = chunks.begin(); it != chunks.end(); it++) {
                double dist = EUCLDIST(playerChunk, it->first);
                auto &chunk = it->second;
                if (!chunk.dirty || dist >= RENDER_RADIUS) continue;

                Meshing::MeshData cpu =
                    Meshing::ChunkMesher::build(chunk, blockQuery);

                auto [mit, inserted] = meshes.try_emplace(it->first);
                mit->second.upload(cpu);

                chunk.dirty = false;
            }
        }

        void Renderer::renderWorld() {
            shader.use();
            texture.bind();

            glm::mat4 proj = glm::perspective(glm::radians((float)cam.fovy),
                                              (float)width / (float)height,
                                              (float)cam.near, (float)cam.far);
            shader.setMat4("projection", proj);

            glm::mat4 view = cam.getViewMat();
            shader.setMat4("view", view);

            World::ChunkCoord playerChunk = {floorDiv(cam.pos[0], CHUNK_MAX_X),
                                             floorDiv(cam.pos[1], CHUNK_MAX_Y),
                                             floorDiv(cam.pos[2], CHUNK_MAX_Z)};

            for (auto &[coord, mesh] : meshes) {
                if (mesh.empty()) continue;

                double dist = EUCLDIST(playerChunk, coord);
                if (dist >= RENDER_RADIUS) continue;

                glm::mat4 model =
                    glm::translate(glm::mat4(1.0f), coord.worldOrigin());
                shader.setMat4("model", model);

                mesh.draw();
            }
        }
    } // namespace GFX
} // namespace Minecraft
