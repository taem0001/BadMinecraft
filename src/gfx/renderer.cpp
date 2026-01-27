#include "../../include/gfx/renderer.hpp"
#include <iostream>
#include <stdlib.h>

namespace Minecraft {
	namespace GFX {
		Renderer::Renderer()
			: cam(Entity::Camera(glm::vec3(0.0f, 3.0f, 0.0f))),
			  vbo(createVBO(GL_ARRAY_BUFFER)),
			  ebo(createVBO(GL_ELEMENT_ARRAY_BUFFER)), vao(createVAO()) {
			for (int i = 0; i < SHADERNUM; i++) {
				this->shader[i].init("res/shaders/block.vert",
									 "res/shaders/block.frag");
			}
			texture.init("res/textures/dirt.png");
		}

		Renderer::~Renderer() {
			destroyVBO(this->vbo);
			destroyVBO(this->ebo);
			destroyVAO(this->vao);
		}

		void Renderer::renderWorld(World::World &w) {
			shader[0].use();

			glm::mat4 proj =
				glm::perspective(glm::radians(CASTTOFLOAT(cam.fovy)),
								 (float)WIDTH / (float)HEIGHT,
								 CASTTOFLOAT(cam.near), CASTTOFLOAT(cam.far));
			shader[0].setMat4("projection", proj);

			glm::mat4 view = cam.getViewMat();
			shader[0].setMat4("view", view);

			struct World::Chunk c = w.getChunk();
			c.generateMesh();
			renderChunk(c);
		}

		void Renderer::renderChunk(struct World::Chunk &c) {
			bufferVBO(this->vbo, c.vertices.data(),
					  static_cast<GLsizeiptr>(c.vertices.size() *
											  sizeof(World::Vertex)));
			bindVBO(this->vbo);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
								  sizeof(World::Vertex), (void *)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
								  sizeof(World::Vertex),
								  (void *)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			bindVAO(this->vao);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, c.offset);
			shader[0].setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, c.vertices.size());
		}
	} // namespace GFX
} // namespace Minecraft
