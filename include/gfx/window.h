#pragma once

#include "../util/types.h"
#include "gfx.h"

namespace Minecraft {
	namespace GFX {
		class Window {
			public:
				Window(int, int);
				~Window();
				void windowLoop();

			private:
				GLFWwindow *handle;
				int width, height;
		};
	} // namespace GFX
} // namespace Minecraft
