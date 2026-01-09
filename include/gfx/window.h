#pragma once

#include "gfx.h"

namespace Minecraft {
	namespace GFX {
		class Window {
			private:
				GLFWwindow *handle;
				int width, height;

			public:
				Window(int, int);
				~Window();
				GLFWwindow *get_handle();
		};
	} // namespace GFX
} // namespace Minecraft
