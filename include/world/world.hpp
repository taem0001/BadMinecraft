#pragma once

#include "../gfx/chunkmesh.hpp"
#include "../util/includes.hpp"
#include "../util/macros.hpp"
#include "../util/perlin.hpp"
#include "chunk.hpp"
#include "worldgen.hpp"

namespace Minecraft {
	namespace World {
		class World {
			public:
				World(u64 seed);

				Chunk *getChunk(const ChunkCoord &coord);
				const Chunk *getChunk(const ChunkCoord &coord) const;
				Chunk &getOrCreateChunk(const ChunkCoord &coord);
				std::unordered_map<ChunkCoord, Chunk> &getChunks();
				const std::unordered_map<ChunkCoord, Chunk> &getChunks() const;
				u64 getSeed() const;
                const WorldGen &getWorldGen() const;
                WorldGen &getWorldGen();

				Block::BlockID getBlockWorld(int x, int y, int z) const;
				void setBlockWorld(int wx, int wy, int wz, Block::BlockID id);

				bool containsChunk(const ChunkCoord &coord);

			private:
				u64 seed;
				std::unordered_map<ChunkCoord, Chunk> chunks;
                WorldGen gen;

				void createChunk(const ChunkCoord &coord);
				void markDirtyIfLoaded(const ChunkCoord &coord);
		};
	} // namespace World
} // namespace Minecraft
