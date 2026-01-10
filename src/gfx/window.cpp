#include "../../include/gfx/window.h"
#include <iostream>

namespace Minecraft {
	namespace GFX {
		// GLFW callback functions
		static void errorCallback(int error, const char *description) {
			fprintf(stderr, "Error: %s\n", description);
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

		// Window class functions implemented
		Window::Window(int width, int height) {
			this->width = width;
			this->height = height;

			glfwSetErrorCallback(errorCallback);

			if (!glfwInit()) {
				exit(EXIT_FAILURE);
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			this->handle =
				glfwCreateWindow(width, height, "Minecraft", NULL, NULL);

			if (!handle) {
				glfwTerminate();
				exit(EXIT_FAILURE);
			}

			glfwSetKeyCallback(handle, keyCallback);

			glfwMakeContextCurrent(handle);
			gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			glfwSwapInterval(1);
		}

		Window::~Window() {
			glfwDestroyWindow(handle);
			glfwTerminate();
		}

		void Window::windowLoop() {
			glfwSetFramebufferSizeCallback(handle, framebufferSizeCallback);

			while (!glfwWindowShouldClose(handle)) {
				glClearColor(0.3, 0.7, 0.9, 1);
				glClear(GL_COLOR_BUFFER_BIT);

				glfwSwapBuffers(handle);
				glfwPollEvents();
			}
		}
	} // namespace GFX
} // namespace Minecraft