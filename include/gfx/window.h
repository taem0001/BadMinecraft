#pragma once

#include "../util/types.h"
#include "gfx.h"
#include "renderer.h"
#include <chrono>

namespace Minecraft {
	namespace GFX {
		class Window {
			public:
				Window(int, int);
				~Window();
				void windowLoop();

			private:
				GLFWwindow *handle;
				struct Renderer renderer;
				int width, height;

				void render(Renderer *);
				u64 getTickCount();
		};
	} // namespace GFX
} // namespace Minecraft
