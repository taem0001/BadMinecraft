#pragma once

#include "block.hpp"

namespace Minecraft {
    namespace World {
        class World {
            public:
                World();    

                struct Block blocks[25];

            private:
        };
    }
}