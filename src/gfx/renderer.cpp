#include "../../include/gfx/renderer.hpp"

namespace Minecraft {
	namespace GFX {
		Renderer::Renderer()
			: cam(Camera(glm::vec3(0.0f, 70.0f, 0.0f))), width(WIDTH),
			  height(HEIGHT), RENDER_RADIUS(3), UPDATE_RADIUS(5),
			  RENDER_RADIUS_SQ(SQUARE(RENDER_RADIUS)),
			  UPDATE_RADIUS_SQ(SQUARE(UPDATE_RADIUS)) {
			shader.init("res/shaders/block.vert", "res/shaders/block.frag");
			texture.init("res/textures/blockatlas.png");
		}

		void Renderer::updateChunks(World::World &w) {
			auto blockQuery = [&](int x, int y, int z) {
				return w.getBlockWorld(x, y, z);
			};

			auto setQuery = [&](int x, int y, int z, Block::BlockID id) {
				w.setBlockWorld(x, y, z, id);
			};

			World::ChunkCoord playerChunk = {floorDiv(cam.pos[0], CHUNK_MAX_X),
											 floorDiv(cam.pos[2], CHUNK_MAX_Z)};

			World::WorldGen &gen = w.getWorldGen();

			const CoordList wantedCoords = computeWantedCoords(playerChunk);
			const CoordList genJobs = requestMissingChunks(w, wantedCoords);

			for (World::ChunkCoord cc : genJobs) {
				gen.chunkGen(cc, setQuery);
			}

			const CoordList meshJobs =
				enqueDirtyChunksForMeshing(w, playerChunk);

			MeshList uploadJobs;
			uploadJobs.reserve(meshJobs.size());

			for (World::ChunkCoord cc : meshJobs) {
				World::ChunkPtr chunk = w.getChunk(cc);
				Meshing::MeshData cpu =
					Meshing::ChunkMesher::build(chunk, blockQuery);
				uploadJobs.push_back({cc, std::move(cpu)});
			}

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

		void Renderer::renderWorld() {
			shader.use();
			texture.bind();

			glm::mat4 proj = cam.getProjMat(width, height);
			shader.setMat4("projection", proj);

			glm::mat4 view = cam.getViewMat();
			shader.setMat4("view", view);

			World::ChunkCoord playerChunk = {floorDiv(cam.pos[0], CHUNK_MAX_X),
											 floorDiv(cam.pos[2], CHUNK_MAX_Z)};

			Frustum f = getFrustum();

			for (auto &[coord, mesh] : meshes) {
				if (mesh.empty()) continue;

				aabb b = getAABB(coord);
				if (!aabbInFrustum(b, f)) continue;

				double dist = EUCLDISTSQ(playerChunk, coord);
				if (dist > RENDER_RADIUS_SQ) continue;

				glm::mat4 model =
					glm::translate(glm::mat4(1.0f), coord.worldOrigin());
				shader.setMat4("model", model);

				mesh.draw();
			}
		}

		Plane Renderer::normalizePlane(const glm::vec4 &v) {
			glm::vec3 n(v.x, v.y, v.z);
			float len = glm::length(n);
			return {n / len, v.w / len};
		}

		Frustum Renderer::getFrustum() {
			Frustum f;
			const glm::mat4 VP =
				cam.getProjMat(width, height) * cam.getViewMat();

			glm::vec4 r0 = glm::row(VP, 0);
			glm::vec4 r1 = glm::row(VP, 1);
			glm::vec4 r2 = glm::row(VP, 2);
			glm::vec4 r3 = glm::row(VP, 3);

			f.p[0] = normalizePlane(r3 + r0); // Left
			f.p[1] = normalizePlane(r3 - r0); // Right
			f.p[2] = normalizePlane(r3 + r1); // Bottom
			f.p[3] = normalizePlane(r3 - r1); // Top
			f.p[4] = normalizePlane(r3 + r2); // Near
			f.p[5] = normalizePlane(r3 - r2); // Far

			return f;
		}

		aabb Renderer::getAABB(const World::ChunkCoord &coord) {
			glm::vec3 mn(coord.x * CHUNK_MAX_X, 0.0, coord.z * CHUNK_MAX_Z);
			glm::vec3 mx =
				mn + glm::vec3(CHUNK_MAX_X, CHUNK_MAX_Y, CHUNK_MAX_Z);
			return {mn, mx};
		}

		bool Renderer::aabbOutsidePlane(const aabb &b, const Plane &plane) {
			glm::vec3 p = b.mn;

			if (plane.normal.x >= 0) p.x = b.mx.x;
			if (plane.normal.y >= 0) p.y = b.mx.y;
			if (plane.normal.z >= 0) p.z = b.mx.z;

			return glm::dot(plane.normal, p) + plane.distance < 0.0f;
		}

		bool Renderer::aabbInFrustum(const aabb &b, const Frustum &f) {
			for (int i = 0; i < 6; i++) {
				if (aabbOutsidePlane(b, f.p[i])) return false;
			}
			return true;
		}

		const CoordList
		Renderer::computeWantedCoords(const World::ChunkCoord &coord) const {
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

		const CoordList Renderer::requestMissingChunks(World::World &w,
													   const CoordList &list) {
			CoordList res;

			for (World::ChunkCoord cc : list) {
				if (!w.containsChunk(cc)) {
					res.push_back(cc);
				}
			}

			return res;
		}

		const CoordList Renderer::enqueDirtyChunksForMeshing(
			World::World &w, const World::ChunkCoord &playerCoord) {
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
	} // namespace GFX
} // namespace Minecraft
