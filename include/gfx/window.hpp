#pragma once

#include "../util/macros.hpp"
#include "../util/types.hpp"
#include "../world/world.hpp"
#include "gfx.hpp"
#include "renderer.hpp"

namespace Minecraft {
	namespace GFX {
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

				void processMouse();
				void processInput();
		};
	} // namespace GFX
} // namespace Minecraft
