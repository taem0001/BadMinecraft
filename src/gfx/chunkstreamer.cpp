#include "../../include/gfx/chunkstreamer.hpp"

namespace Minecraft::GFX {
	ChunkStreamer::ChunkStreamer()
		: RENDER_RADIUS(5), UPDATE_RADIUS(8), RENDER_RADIUS_SQ(SQUARE(RENDER_RADIUS)),
		  UPDATE_RADIUS_SQ(SQUARE(UPDATE_RADIUS)) {}

	void ChunkStreamer::updateChunks(World::World &w, const World::ChunkCoord &playerChunk) {
		World::WorldGen &gen = w.getWorldGen();

		const CoordList wantedCoords = computeWantedCoords(playerChunk);
		const CoordList genJobs = requestMissingChunks(w, wantedCoords);

		for (World::ChunkCoord cc : genJobs) {
			World::ChunkPtr chunk = w.getOrCreateChunk(cc);
			gen.chunkGen(chunk);
		}

		const CoordList meshJobs = enqueDirtyChunksForMeshing(w, playerChunk);

		const MeshList uploadJobs = makeChunksMeshes(w, meshJobs);

		for (auto &it : uploadJobs) {
			World::ChunkCoord cc = it.first;
			World::ChunkPtr chunk = w.getChunk(cc);
			auto [mit, inserted] = meshes.try_emplace(cc);
			mit->second.upload(it.second);
			chunk->dirty = false;
		}

		for (auto it = meshes.begin(); it != meshes.end();) {
			World::ChunkCoord cc = it->first;
			double dist = EUCLDISTSQ(cc, playerChunk);
			if (dist > UPDATE_RADIUS_SQ) {
				it = meshes.erase(it);
				w.destroyChunk(cc);
			} else {
				it++;
			}
		}
	}

	const CoordList ChunkStreamer::computeWantedCoords(const World::ChunkCoord &coord) const {
		CoordList res;

		for (int dx = -UPDATE_RADIUS; dx <= UPDATE_RADIUS; dx++) {
			for (int dz = -UPDATE_RADIUS; dz <= UPDATE_RADIUS; dz++) {
				if (SQUARE(dx) + SQUARE(dz) <= UPDATE_RADIUS_SQ) {
					World::ChunkCoord offset = {dx, dz};
					res.push_back(coord + offset);
				}
			}
		}

		return res;
	}

	const CoordList ChunkStreamer::requestMissingChunks(World::World &w, const CoordList &list) {
		CoordList res;

		for (World::ChunkCoord cc : list) {
			if (!w.containsChunk(cc)) {
				res.push_back(cc);
			}
		}

		return res;
	}

	const CoordList ChunkStreamer::enqueDirtyChunksForMeshing(World::World &w, const World::ChunkCoord &playerCoord) {
		CoordList res;
		auto chunks = w.getChunkSnapshot();
		for (auto it : chunks) {
			World::ChunkCoord coord = it.first;
			World::ChunkPtr chunk = it.second;
			double dist = EUCLDISTSQ(coord, playerCoord);

			if (chunk->dirty && dist <= SQUARE(RENDER_RADIUS + 1)) {
				res.push_back(coord);
			}
		}

		return res;
	}

	const MeshList ChunkStreamer::makeChunksMeshes(World::World &w, const CoordList &list) {
		MeshList res;
		res.reserve(list.size());

		for (World::ChunkCoord cc : list) {
			World::ChunkPtr chunk = w.getChunk(cc);
			World::NeighborSnapshot snapshot = w.getNeighborSnapshot(chunk);
			Meshing::MeshData cpu = Meshing::ChunkMesher::build(snapshot);
			res.push_back({cc, std::move(cpu)});
		}

		return res;
	}
} // namespace Minecraft::GFX