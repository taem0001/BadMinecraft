#include "../include/ui/gfx.h"
#include <iostream>

void error_callback(int error, const char *description) {
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
						 int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window =
		glfwCreateWindow(640, 480, "Minecraft", NULL, NULL);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
 
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window)) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
        glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
