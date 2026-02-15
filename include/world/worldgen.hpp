#pragma once

#include "../util/includes.hpp"
#include "../util/perlin.hpp"
#include "../util/types.hpp"
#include "chunk.hpp"

namespace Minecraft {
    namespace World {
        using SetQuery = std::function<void(int, int, int, Block::BlockID)>;

        class WorldGen {
            public:
                WorldGen(u64 seed, int octaves = 4, double scale = 50.0, double persistance= 0.5, double lacunarity = 2.0);
                void chunkGen(const ChunkCoord &playerChunk,
                                     SetQuery query);

            private:
                u64 seed;
                int octaves;
                double scale, persistance, lacunarity;
                std::vector<std::array<int, 2>> octaveOffset;
        };
    } // namespace World
} // namespace Minecraft