#include "../../include/world/world.hpp"

namespace Minecraft {
	namespace World {
		World::World() {
			for (int z = 0; z < CHUNK_MAX_Z * 2; z++) {
				for (int y = 0; y < CHUNK_MAX_Y; y++) {
					for (int x = 0; x < CHUNK_MAX_X; x++) {
						setBlockWorld(x, y, z, Block::DIRT);
					}
				}
			}
		}

		// Getters / Setters
		Chunk *World::getChunk(const ChunkCoord &coord) {
			// If chunk exists, return pointer, otherwise null
			auto it = chunks.find(coord);
			if (it == chunks.end()) return nullptr;
			return &it->second;
		}

		const Chunk *World::getChunk(const ChunkCoord &coord) const {
			// If chunk exists, return pointer, otherwise null
			auto it = chunks.find(coord);
			if (it == chunks.end()) return nullptr;
			return &it->second;
		}

		Chunk &World::getOrCreateChunk(const ChunkCoord &coord) {
			Chunk *chunkptr = getChunk(coord);
			// If chunk does not exist, then create it
			if (chunkptr == nullptr) {
				createChunk(coord);
				return chunks.at(coord);
			}
			return *chunkptr;
		}

		std::unordered_map<ChunkCoord, Chunk> &World::getChunks() {
			return chunks;
		}

		const std::unordered_map<ChunkCoord, Chunk> &World::getChunks() const {
			return chunks;
		}

		void World::markDirtyIfLoaded(const ChunkCoord &coord) {
			Chunk *c = getChunk(coord);
			if (c != nullptr) {
				c->dirty = true;
			}
		}

		Block::BlockID World::getBlockWorld(int wx, int wy, int wz) const {
			// Find chunk with world coordinates
			ChunkCoord coord = {floorDiv(wx, CHUNK_MAX_X),
								floorDiv(wy, CHUNK_MAX_Y),
								floorDiv(wz, CHUNK_MAX_Z)};
			const Chunk *chunk = getChunk(coord);

			if (chunk == nullptr) return Block::AIR;

			// Convert world coordinates to local chunk coordinates
			int localx = floorMod(wx, CHUNK_MAX_X);
			int localy = floorMod(wy, CHUNK_MAX_Y);
			int localz = floorMod(wz, CHUNK_MAX_Z);

			// Get block from chunk
			return chunk->getLocalBlock(localx, localy, localz);
		}

		// World editing functions
		void World::createChunk(const ChunkCoord &coord) {
			auto [it, inserted] = chunks.try_emplace(coord);
			Chunk &c = it->second;
			if (inserted) {
				c.coord = coord;
				c.dirty = true;
				c.blocks.fill(Block::AIR);

				markDirtyIfLoaded(coord + (ChunkCoord){-1, 0, 0});
				markDirtyIfLoaded(coord + (ChunkCoord){1, 0, 0});
				markDirtyIfLoaded(coord + (ChunkCoord){0, 0, -1});
				markDirtyIfLoaded(coord + (ChunkCoord){0, 0, 1});

				std::cout << "[INFO] Created chunk at " << coord << "."
						  << std::endl;
			}
		}

		void World::setBlockWorld(int wx, int wy, int wz, Block::BlockID id) {
			// Define the chunk coordinates and get the chunk
			ChunkCoord coord = {floorDiv(wx, CHUNK_MAX_X),
								floorDiv(wy, CHUNK_MAX_Y),
								floorDiv(wz, CHUNK_MAX_Z)};
			Chunk &chunk = getOrCreateChunk(coord);

			// Convert world coordinates to local chunk coordinates
			int localx = floorMod(wx, CHUNK_MAX_X);
			int localy = floorMod(wy, CHUNK_MAX_Y);
			int localz = floorMod(wz, CHUNK_MAX_Z);

			// Mark neighboring chunk dirty if at chunk border
			if (localx == 0) markDirtyIfLoaded(coord + (ChunkCoord){-1, 0, 0});
			if (localx == CHUNK_MAX_X - 1)
				markDirtyIfLoaded(coord + (ChunkCoord){1, 0, 0});
			if (localz == 0) markDirtyIfLoaded(coord + (ChunkCoord){0, 0, -1});
			if (localz == CHUNK_MAX_Z - 1)
				markDirtyIfLoaded(coord + (ChunkCoord){0, 0, 1});

			// Place block at the coordinates
			chunk.setLocalBlock(localx, localy, localz, id);
			chunk.dirty = true;
		}

		inline int World::floorDiv(int a, int b) {
			int q = a / b;
			int r = a % b;
			if (r != 0 && ((r < 0) != (b < 0))) --q;
			return q;
		}

		inline int World::floorMod(int a, int b) {
			int r = a % b;
			if (r < 0) r += b;
			return r;
		}
	} // namespace World
} // namespace Minecraft
