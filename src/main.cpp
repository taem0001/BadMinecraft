#include "../include/gfx/gfx.h"
#include "../include/gfx/window.h"
#include <iostream>

using namespace Minecraft::GFX;

int main() {
	Window window(640, 480);
	window.windowLoop();
	return 0;
}
