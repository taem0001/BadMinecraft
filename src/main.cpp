#include "../include/gfx/gfx.h"
#include "../include/gfx/window.h"
#include <iostream>

using namespace Minecraft::GFX;

int main() {
	Window window(640, 480);
	GLFWwindow *handle = window.get_handle();

	while (!glfwWindowShouldClose(handle)) {
		int width, height;
		glfwGetFramebufferSize(handle, &width, &height);

		glViewport(0, 0, width, height);
		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(handle);
		glfwPollEvents();
	}

	return 0;
}
