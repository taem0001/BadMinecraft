#include "../../include/gfx/chunkmesh.hpp"

namespace Minecraft {
	namespace GFX {
		ChunkMesh::ChunkMesh()
			: vao(0), vbo(0), ebo(0), vertexCount(0), configured(false) {
			createGLObjects();
		}

		ChunkMesh::~ChunkMesh() { destroyGLObjects(); }

		void ChunkMesh::upload(const Meshing::MeshData &data) {
			vertexCount = data.vertices.size();
			indexCount = data.indices.size();

			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

			glBufferData(GL_ARRAY_BUFFER,
						 (GLsizeiptr)(vertexCount * sizeof(Meshing::Vertex)),
						 data.vertices.data(), GL_DYNAMIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,
						 (GLsizeiptr)(indexCount * sizeof(unsigned int)),
						 data.indices.data(), GL_DYNAMIC_DRAW);

			// Configure vertex layout once
			if (!configured) {
				// loc 0 vertex location
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(
					0, 3, GL_FLOAT, GL_FALSE, sizeof(Meshing::Vertex),
					(void *)offsetof(Meshing::Vertex, position));

				// loc 1 uv location
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
									  sizeof(Meshing::Vertex),
									  (void *)offsetof(Meshing::Vertex, uv));

				configured = true;
			}

			glBindVertexArray(0);
		}

		void ChunkMesh::draw() const {
			if (empty()) return;
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, (GLsizei)indexCount, GL_UNSIGNED_INT,
						   (void *)0);
			glBindVertexArray(0);
		}

		bool ChunkMesh::empty() const { return vertexCount == 0; }
		size_t ChunkMesh::getVertexCount() const { return vertexCount; }
		size_t ChunkMesh::getIndexCount() const { return indexCount; }

		void ChunkMesh::createGLObjects() {
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);
		}

		void ChunkMesh::destroyGLObjects() {
			if (vbo) glDeleteBuffers(1, &vbo);
			if (ebo) glDeleteBuffers(1, &ebo);
			if (vao) glDeleteVertexArrays(1, &vao);
			vbo = 0;
			ebo = 0;
			vao = 0;
			vertexCount = 0;
			indexCount = 0;
			configured = false;
		}

		void ChunkMesh::moveFrom(ChunkMesh &other) noexcept {
			vao = std::exchange(other.vao, 0);
			vbo = std::exchange(other.vbo, 0);
			ebo = std::exchange(other.ebo, 0);
			vertexCount = std::exchange(other.vertexCount, 0);
			indexCount = std::exchange(other.indexCount, 0);
			configured = std::exchange(other.configured, false);
		}
	} // namespace GFX
} // namespace Minecraft