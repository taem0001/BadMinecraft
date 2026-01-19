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
				void windowLoop(Renderer &renderer, World::World &world);

			private:
				GLFWwindow *handle;
				int width, height;
				double deltatime, lastframe;

				static void errorCallback(int, const char *);
				static void keyCallback(GLFWwindow *, int, int, int, int);
				static void framebufferSizeCallback(GLFWwindow *, int, int);

				void processMouse(Renderer &renderer);
				void processInput(Renderer &renderer);
		};
	} // namespace GFX
} // namespace Minecraft
