#include "../../include/gfx/window.h"
#include "../../include/gfx/renderer.h"
#include <chrono>
#include <iostream>

namespace Minecraft {
	namespace GFX {
		using namespace std::chrono;

		void Window::errorCallback(int error, const char *description) {
			fprintf(stderr, "[ERROR] %s\n", description);
		}

		void Window::keyCallback(GLFWwindow *window, int key, int scancode,
								 int action, int mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		void Window::framebufferSizeCallback(GLFWwindow *window, int width,
											 int height) {
			glViewport(0, 0, width, height);
		}

		Window::Window(int w, int h)
			: renderer(), width(w), height(h), deltatime(0), lastframe(0) {

			glfwSetErrorCallback(errorCallback);

			if (!glfwInit()) {
				exit(EXIT_FAILURE);
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			this->handle =
				glfwCreateWindow(width, height, "BadMinecraft", NULL, NULL);

			if (!handle) {
				glfwTerminate();
				exit(EXIT_FAILURE);
			}

			glfwSetKeyCallback(handle, keyCallback);
			glfwSetFramebufferSizeCallback(handle, framebufferSizeCallback);

			glfwMakeContextCurrent(handle);
			gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			glfwSwapInterval(1);

			renderer.init();

			// Log info
			printf("[INFO] Renderer: %s.\n", glGetString(GL_RENDERER));
			printf("[INFO] OpenGL version supported %s.\n",
				   glGetString(GL_VERSION));
		}

		void Window::windowLoop() {
			auto start = steady_clock::now();

			while (!glfwWindowShouldClose(handle)) {
				auto now = steady_clock::now();
				u64 currentframe =
					duration_cast<milliseconds>(now - start).count();
				deltatime = currentframe - lastframe;
				lastframe = currentframe;

				glfwPollEvents();
				processInput();

				glClearColor(0.3, 0.7, 0.9, 1);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				render();
				glfwSwapBuffers(handle);
			}
			printf("[INFO] Shutting down.\n");
		}

		void Window::render() {
			renderer.renderTriangle();
			renderer.shader[0].use();

			glm::mat4 proj = glm::perspective(
				glm::radians(renderer.cam.fovy), (float)width / (float)height,
				renderer.cam.near, renderer.cam.far);
			renderer.shader[0].setMat4("projection", proj);

			glm::mat4 view = renderer.cam.getViewMat();
			renderer.shader[0].setMat4("view", view);

			glm::mat4 model = glm::mat4(1.0f);
			renderer.shader[0].setMat4("model", model);

			bindVAO(renderer.vao);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		Window::~Window() {
			glfwDestroyWindow(handle);
			glfwTerminate();
		}

		void Window::processInput() {
			if (glfwGetKey(handle, GLFW_KEY_W) == GLFW_PRESS)
				renderer.cam.processKey(Entity::FORWARD, deltatime);
			if (glfwGetKey(handle, GLFW_KEY_S) == GLFW_PRESS)
				renderer.cam.processKey(Entity::BACKWARD, deltatime);
			if (glfwGetKey(handle, GLFW_KEY_A) == GLFW_PRESS)
				renderer.cam.processKey(Entity::LEFT, deltatime);
			if (glfwGetKey(handle, GLFW_KEY_D) == GLFW_PRESS)
				renderer.cam.processKey(Entity::RIGHT, deltatime);
		}
	} // namespace GFX
} // namespace Minecraft
