#include "../../include/world/world.hpp"

namespace Minecraft {
	namespace World {
		World::World(u64 s) : seed(s), gen(WorldGen(s, 4, 50.0, 0.35, 1.7)) {}

		// Getters / Setters
		ChunkPtr World::getChunk(const ChunkCoord &coord) {
			auto it = chunks.find(coord);
			if (it == chunks.end()) return nullptr;
			return it->second;
		}

		std::shared_ptr<const Chunk>
		World::getChunk(const ChunkCoord &coord) const {
			auto it = chunks.find(coord);
			if (it == chunks.end()) return nullptr;
			return it->second;
		}

		ChunkPtr World::getOrCreateChunk(const ChunkCoord &coord) {
			ChunkPtr c = getChunk(coord);
			if (c == nullptr) {
				createChunk(coord);
				return getChunk(coord);
			}
			return c;
		}

		ChunkSnapshot World::getChunkSnapshot() const {
			ChunkSnapshot res;
			res.reserve(chunks.size());
			for (auto &[coord, cptr] : chunks) {
				res.push_back({coord, cptr});
			}
			return res;
		}

		NeighborSnapshot World::getNeighborSnapshot(const ChunkPtr &chunk) const {
			NeighborSnapshot res;
			res.center = chunk;
		
			ChunkCoord px = chunk->coord + (ChunkCoord){1, 0};
			res.px = getChunk(px);
			ChunkCoord nx = chunk->coord + (ChunkCoord){-1, 0};
			res.nx = getChunk(nx);
			ChunkCoord pz = chunk->coord + (ChunkCoord){0, 1};
			res.pz = getChunk(pz);
			ChunkCoord nz = chunk->coord + (ChunkCoord){0, -1};
			res.nz = getChunk(nz);

			return res;
		}

		void World::markDirtyIfLoaded(const ChunkCoord &coord) {
			ChunkPtr c = getChunk(coord);
			if (c != nullptr) {
				c->dirty = true;
			}
		}

		Block::BlockID World::getBlockWorld(int wx, int wy, int wz) const {
			// Find chunk with world coordinates
			ChunkCoord coord = {floorDiv(wx, CHUNK_MAX_X),
								floorDiv(wz, CHUNK_MAX_Z)};
			const std::shared_ptr<const Chunk> chunk = getChunk(coord);

			// If chunk doesn't exist, then return air
			if (chunk == nullptr) return Block::AIR;

			// Convert world coordinates to local chunk coordinates
			int localx = floorMod(wx, CHUNK_MAX_X);
			int localy = floorMod(wy, CHUNK_MAX_Y);
			int localz = floorMod(wz, CHUNK_MAX_Z);

			// Get block from chunk
			return chunk->getLocalBlock(localx, localy, localz);
		}

		u64 World::getSeed() const { return seed; }

		const WorldGen &World::getWorldGen() const { return gen; }

		WorldGen &World::getWorldGen() { return gen; }

		bool World::containsChunk(const ChunkCoord &coord) {
			return chunks.contains(coord);
		}

		// World editing functions
		void World::destroyChunk(const ChunkCoord &coord) {
			chunks.erase(coord);
		}

		void World::createChunk(const ChunkCoord &coord) {
			ChunkPtr chunk = std::make_shared<Chunk>(coord);
			auto [it, inserted] = chunks.try_emplace(coord, chunk);
			ChunkPtr c = it->second;
			if (inserted) {
				c->coord = coord;
				c->dirty = true;
				c->blocks.fill(Block::AIR);

				markDirtyIfLoaded(coord + (ChunkCoord){-1, 0});
				markDirtyIfLoaded(coord + (ChunkCoord){1, 0});
				markDirtyIfLoaded(coord + (ChunkCoord){0, -1});
				markDirtyIfLoaded(coord + (ChunkCoord){0, 1});
			}
		}

		void World::setBlockWorld(int wx, int wy, int wz, Block::BlockID id) {
			// Define the chunk coordinates and get the chunk
			ChunkCoord coord = {floorDiv(wx, CHUNK_MAX_X),
								floorDiv(wz, CHUNK_MAX_Z)};
			ChunkPtr chunk = getOrCreateChunk(coord);

			// Convert world coordinates to local chunk coordinates
			int localx = floorMod(wx, CHUNK_MAX_X);
			int localy = floorMod(wy, CHUNK_MAX_Y);
			int localz = floorMod(wz, CHUNK_MAX_Z);

			// Mark neighboring chunk dirty if at chunk border
			if (localx == 0) markDirtyIfLoaded(coord + (ChunkCoord){-1, 0});
			if (localx == CHUNK_MAX_X - 1)
				markDirtyIfLoaded(coord + (ChunkCoord){1, 0});
			if (localz == 0) markDirtyIfLoaded(coord + (ChunkCoord){0, -1});
			if (localz == CHUNK_MAX_Z - 1)
				markDirtyIfLoaded(coord + (ChunkCoord){0, 1});

			// Place block at the coordinates
			chunk->setLocalBlock(localx, localy, localz, id);
			chunk->dirty = true;
		}
	} // namespace World
} // namespace Minecraft
