#pragma once

#include "../gfx/chunkmesh.hpp"
#include "../util/includes.hpp"
#include "../util/macros.hpp"
#include "../util/perlin.hpp"
#include "chunk.hpp"
#include "worldgen.hpp"

namespace Minecraft {
	namespace World {
		using ChunkPtr = std::shared_ptr<Chunk>;
		using ChunkMap = std::unordered_map<ChunkCoord, ChunkPtr>;
		using ChunkSnapshot = std::vector<std::pair<ChunkCoord, ChunkPtr>>;

		struct NeighborSnapshot {
				std::shared_ptr<const Chunk> center, px, nx, pz, nz;
		};

		class World {
			public:
				World(u64 seed);

				ChunkPtr getChunk(const ChunkCoord &coord);
				std::shared_ptr<const Chunk>
				getChunk(const ChunkCoord &coord) const;
				ChunkPtr getOrCreateChunk(const ChunkCoord &coord);
				void destroyChunk(const ChunkCoord &coord);

				ChunkSnapshot getChunkSnapshot() const;
				NeighborSnapshot getNeighborSnapshot(const ChunkPtr &chunk) const;

				u64 getSeed() const;

				const WorldGen &getWorldGen() const;
				WorldGen &getWorldGen();

				Block::BlockID getBlockWorld(int x, int y, int z) const;
				void setBlockWorld(int wx, int wy, int wz, Block::BlockID id);

				bool containsChunk(const ChunkCoord &coord);

			private:
				u64 seed;
				ChunkMap chunks;
				WorldGen gen;

				void createChunk(const ChunkCoord &coord);
				void markDirtyIfLoaded(const ChunkCoord &coord);
		};
	} // namespace World
} // namespace Minecraft
