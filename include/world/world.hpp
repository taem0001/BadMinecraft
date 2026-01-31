#pragma once

#include "../gfx/chunkmesh.hpp"
#include "chunk.hpp"
#include <unordered_map>

namespace Minecraft {
	namespace World {
		class World {
			public:
				World();

				Chunk *getChunk(const ChunkCoord &coord);
				const Chunk *getChunk(const ChunkCoord &coord) const;
				Chunk &getOrCreateChunk(const ChunkCoord &coord);
				std::unordered_map<ChunkCoord, Chunk> &getChunks();
				const std::unordered_map<ChunkCoord, Chunk> &getChunks() const;

				Block::BlockID getBlockWorld(int x, int y, int z) const;

			private:
				std::unordered_map<ChunkCoord, Chunk> chunks;

				void setBlockWorld(int wx, int wy, int wz, Block::BlockID id);
				void createChunk(const ChunkCoord &coord);
				void markDirtyIfLoaded(const ChunkCoord &coord);

				static inline int floorDiv(int a, int b);
				static inline int floorMod(int a, int b);
		};
	} // namespace World
} // namespace Minecraft
