#pragma once

#include "../util/types.hpp"
#include "../util/includes.hpp"

namespace Minecraft {
    namespace Block {
        using BlockID = u8;

        enum BlockType : BlockID { AIR, DIRT, STONE, GRASS };

        struct TexCoord {
                int u;
                int v;
        };

        TexCoord getTexCoord(BlockType type, FaceDir dir);
    } // namespace Block
} // namespace Minecraft