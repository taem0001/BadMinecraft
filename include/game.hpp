#pragma once

#include "gfx/renderer.hpp"
#include "gfx/window.hpp"
#include "util/includes.hpp"
#include "world/world.hpp"

class Game {
	public:
		Game() : window(), renderer(), world() {}

		void runGame() {
			std::cout << "[INFO] Starting game." << std::endl;
			window.windowLoop(renderer, world);
			std::cout << "[INFO] Shutting down." << std::endl;
		}

	private:
		Minecraft::GFX::Window window;
		Minecraft::GFX::Renderer renderer;
		Minecraft::World::World world;
};