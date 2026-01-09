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
				void windowLoop();
		};
	} // namespace GFX
} // namespace Minecraft
