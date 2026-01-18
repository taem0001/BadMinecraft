#include "../../include/gfx/window.hpp"
#include "../../include/gfx/renderer.hpp"
#include <iostream>

namespace Minecraft {
	namespace GFX {
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

		Window::Window()
			: renderer(), width(WIDTH), height(HEIGHT), deltatime(0.0f),
			  lastframe(0.0f) {

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

			glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			if (glfwRawMouseMotionSupported())
				glfwSetInputMode(handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

			glfwMakeContextCurrent(handle);
			gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			glfwSwapInterval(1);

			glEnable(GL_DEPTH_TEST);

			renderer.init();

			// Log info
			printf("[INFO] Renderer: %s.\n", glGetString(GL_RENDERER));
			printf("[INFO] OpenGL version supported %s.\n",
				   glGetString(GL_VERSION));
		}

		void Window::windowLoop() {
			renderer.prepareRect();

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			while (!glfwWindowShouldClose(handle)) {
				double currentframe = glfwGetTime();
				deltatime = currentframe - lastframe;
				lastframe = currentframe;

				glfwPollEvents();
				processInput();
				processMouse();

				glClearColor(0.3, 0.7, 0.9, 1);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				render();
				glfwSwapBuffers(handle);
			}
			printf("[INFO] Shutting down.\n");
		}

		void Window::render() {
			renderer.shader[0].use();

			glm::mat4 proj = glm::perspective(
				glm::radians(CASTTOFLOAT(renderer.cam.fovy)),
				(float)width / (float)height, CASTTOFLOAT(renderer.cam.near),
				CASTTOFLOAT(renderer.cam.far));
			renderer.shader[0].setMat4("projection", proj);

			glm::mat4 view = renderer.cam.getViewMat();
			renderer.shader[0].setMat4("view", view);

			glm::mat4 model = glm::mat4(1.0f);
			renderer.shader[0].setMat4("model", model);

			bindVAO(renderer.vao);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

		Window::~Window() {
			glfwDestroyWindow(handle);
			glfwTerminate();
		}

		void Window::processMouse() {
			static double lastx = WIDTH / 2.0f;
			static double lasty = HEIGHT / 2.0f;
			static bool firstmouse = true;
			double xpos;
			double ypos;

			glfwGetCursorPos(handle, &xpos, &ypos);

			if (firstmouse) {
				lastx = xpos;
				lasty = ypos;
				firstmouse = !firstmouse;
			}

			double xoffset = xpos - lastx;
			double yoffset = lasty - ypos;

			lastx = xpos;
			lasty = ypos;

			renderer.cam.processMouse(xoffset, yoffset, false);
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
