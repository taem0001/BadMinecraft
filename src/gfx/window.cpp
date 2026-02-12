#include "../../include/gfx/window.hpp"
#include "../../include/gfx/renderer.hpp"

namespace Minecraft {
	namespace GFX {
		void Window::errorCallback(int error, const char *description) {
			std::cerr << "[ERROR] " << description << "." << std::endl;
		}

		void Window::keyCallback(GLFWwindow *window, int key, int scancode,
								 int action, int mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			if (key == GLFW_KEY_L && action == GLFW_PRESS)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			if (key == GLFW_KEY_F && action == GLFW_PRESS)
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		void Window::framebufferSizeCallback(GLFWwindow *window, int width,
											 int height) {
			glViewport(0, 0, width, height);
		}

		Window::Window()
			: width(WIDTH), height(HEIGHT), deltatime(0.0f), lastframe(0.0f) {

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
			glEnable(GL_CULL_FACE);

			glCullFace(GL_FRONT);

			// Log info
			std::cout << "[INFO] Renderer: " << glGetString(GL_RENDERER) << "."
					  << std::endl;
			std::cout << "[INFO] OpenGL version supported "
					  << glGetString(GL_VERSION) << "." << std::endl;
		}

		void Window::windowLoop(Renderer &r, World::World &w) {
			while (!glfwWindowShouldClose(handle)) {
				double currentframe = glfwGetTime();
				deltatime = currentframe - lastframe;
				lastframe = currentframe;

				glfwPollEvents();
				processInput(r);
				processMouse(r);

				glClearColor(0.3, 0.7, 0.9, 1);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				r.updateChunks(w);
				r.renderWorld();
				glfwSwapBuffers(handle);
			}
		}

		Window::~Window() {
			glfwDestroyWindow(handle);
			glfwTerminate();
		}

		void Window::processMouse(Renderer &r) {
			Entity::Camera &cam = r.getCam();

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

			cam.processMouse(xoffset, yoffset);
		}

		void Window::processInput(Renderer &r) {
			Entity::Camera &cam = r.getCam();

			if (glfwGetKey(handle, GLFW_KEY_W) == GLFW_PRESS)
				cam.processKey(Entity::FORWARD, deltatime);
			if (glfwGetKey(handle, GLFW_KEY_S) == GLFW_PRESS)
				cam.processKey(Entity::BACKWARD, deltatime);
			if (glfwGetKey(handle, GLFW_KEY_A) == GLFW_PRESS)
				cam.processKey(Entity::LEFT, deltatime);
			if (glfwGetKey(handle, GLFW_KEY_D) == GLFW_PRESS)
				cam.processKey(Entity::RIGHT, deltatime);
			if (glfwGetKey(handle, GLFW_KEY_SPACE) == GLFW_PRESS)
				cam.processKey(Entity::UP, deltatime);
			if (glfwGetKey(handle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				cam.processKey(Entity::DOWN, deltatime);
		}
	} // namespace GFX
} // namespace Minecraft
