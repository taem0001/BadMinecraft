#include "../../include/world/world.hpp"

namespace Minecraft {
	namespace World {
		World::World() {
			for (int x = 0; x < 5; x++) {
				for (int z = 0; z < 5; z++) {
					blocks[5 * x + z] = {.type = DIRT,
										 .pos = glm::vec3(x, 0.0f, z)};
				}
			}
		}
	} // namespace World
} // namespace Minecraft