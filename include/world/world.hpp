#pragma once

#include "chunk.hpp"

namespace Minecraft {
    namespace World {
        class World {
            public:
                World();
                
                struct Chunk getChunk();

            private:
                struct Chunk chunk;
        };
    }
}