#pragma once

#include "../meshing/chunkmesher.hpp"
#include "../util/includes.hpp"
#include "../util/macros.hpp"
#include "../world/world.hpp"
#include "../world/worldgen.hpp"
#include "camera.hpp"
#include "chunkmesh.hpp"
#include "gfx.hpp"
#include "shader.hpp"
#include "texture.hpp"

namespace Minecraft::GFX {
	using CoordList = std::vector<World::ChunkCoord>;
	using MeshList = std::vector<std::pair<World::ChunkCoord, Meshing::MeshData>>;

	class ChunkStreamer {
		public:
			ChunkStreamer();

			double const RENDER_RADIUS, UPDATE_RADIUS;
			double const RENDER_RADIUS_SQ, UPDATE_RADIUS_SQ;

			void updateChunks(World::World &world, const World::ChunkCoord &playerPos);
			const std::unordered_map<World::ChunkCoord, ChunkMesh> &getMeshes() const { return meshes; }

		private:
			std::unordered_map<World::ChunkCoord, ChunkMesh> meshes;

			const CoordList computeWantedCoords(const World::ChunkCoord &playerCoord) const;
			const CoordList requestMissingChunks(World::World &w, const CoordList &list);
			const CoordList enqueDirtyChunksForMeshing(World::World &w, const World::ChunkCoord &playerCoord);
			const MeshList makeChunksMeshes(World::World &w, const CoordList &list);
	};
} // namespace Minecraft::GFX