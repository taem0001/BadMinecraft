#include "../../include/gfx/window.h"
#include "../../include/gfx/renderer.h"
#include <iostream>

namespace Minecraft {
	namespace GFX {
		using namespace std::chrono;

		static void errorCallback(int error, const char *description) {
			fprintf(stderr, "[ERROR] %s\n", description);
		}

		static void keyCallback(GLFWwindow *window, int key, int scancode,
								int action, int mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		static void framebufferSizeCallback(GLFWwindow *window, int width,
											int height) {
			glViewport(0, 0, width, height);
		}

		Window::Window(int w, int h) : renderer(), width(w), height(h) {
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
			const int TICKS_PER_SECOND = 20;
			const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
			const int MAX_FRAMESKIP = 5;

			u64 nextgametick = getTickCount();
			int loops;
			float interpolation;

			while (!glfwWindowShouldClose(handle)) {
				glfwPollEvents();

				loops = 0;
				while (getTickCount() > nextgametick && loops < MAX_FRAMESKIP) {
					// Update
					nextgametick += SKIP_TICKS;
					loops++;
				}

				glClearColor(0.3, 0.7, 0.9, 1);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				render(&renderer);
				glfwSwapBuffers(handle);
			}
			printf("[INFO] Shutting down.\n");
		}

		void Window::render(Renderer *renderer) {
			renderer->renderTriangle();
			renderer->shader->use();
			bindVAO(renderer->vao);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		Window::~Window() {
			glfwDestroyWindow(handle);
			glfwTerminate();
		}

		u64 Window::getTickCount() {
			u64 ms = duration_cast<milliseconds>(
						 system_clock::now().time_since_epoch())
						 .count();
			return ms / 50;
		}
	} // namespace GFX
} // namespace Minecraft