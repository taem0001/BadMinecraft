#pragma once

#include "../util/types.hpp"
#include "gfx.hpp"
#include "renderer.hpp"
#include "../util/macros.hpp"

namespace Minecraft {
	namespace GFX {
#define WIDTH  1280
#define HEIGHT 720

		class Window {
			public:
				Window();
				~Window();
				void windowLoop();

			private:
				GLFWwindow *handle;
				struct Renderer renderer;
				int width, height;
				double deltatime, lastframe;

				static void errorCallback(int, const char *);
				static void keyCallback(GLFWwindow *, int, int, int, int);
				static void framebufferSizeCallback(GLFWwindow *, int, int);

				void render();
				void processMouse();
				void processInput();
		};
	} // namespace GFX
} // namespace Minecraft
