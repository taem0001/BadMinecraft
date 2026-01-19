#include "../../include/gfx/renderer.hpp"
#include <iostream>
#include <stdlib.h>

namespace Minecraft {
	namespace GFX {
		Renderer::Renderer()
			: cam(Entity::Camera(glm::vec3(0.0f, 0.0f, 3.0f))),
			  vbo(createVBO(GL_ARRAY_BUFFER)),
			  ebo(createVBO(GL_ELEMENT_ARRAY_BUFFER)), vao(createVAO()) {
			for (int i = 0; i < SHADERNUM; i++) {
				this->shader[i].init("res/shaders/block.vert",
									 "res/shaders/block.frag");
			}

			bufferVBO(this->vbo, World::vertices, sizeof(World::vertices));
			bufferVBO(this->ebo, World::indices, sizeof(World::indices));
			attrVAO(this->vao, this->vbo, this->ebo, 0, 3, GL_FLOAT, 0);
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

			bindVAO(vao);

			for (int i = 0; i < 25; i++) {
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, w.blocks[i].pos);
				shader[0].setMat4("model", model);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
		}
	} // namespace GFX
} // namespace Minecraft
