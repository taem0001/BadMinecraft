#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Minecraft {
	namespace World {
		enum BlockType { AIR = 0, DIRT };
#define LASTBLOCK DIRT

		enum FaceDir { PZ = LASTBLOCK, NZ, PX, NX, PY, NY };

		struct Vertex {
				float px, py, pz;
				float tx, ty;
		};

		static float z_pos_face[] = {
			// +Z face
			+0.5f, +0.5f, +0.5f, +0.5f, -0.5f, +0.5f,
			-0.5f, -0.5f, +0.5f, -0.5f, +0.5f, +0.5f,
		};

		static float z_neg_face[] = {
			// -Z face
			+0.5f, +0.5f, -0.5f, -0.5f, +0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f, +0.5f, -0.5f, -0.5f,
		};

		static float y_pos_face[] = {
			// +Y
			+0.5f, +0.5f, +0.5f, -0.5f, +0.5f, +0.5f,
			-0.5f, +0.5f, -0.5f, +0.5f, +0.5f, -0.5f,
		};

		static float y_neg_face[] = {
			// -Y
			+0.5f, -0.5f, +0.5f, +0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, +0.5f,
		};

		static float x_pos_face[] = {
			// +X
			+0.5f, +0.5f, +0.5f, +0.5f, +0.5f, -0.5f,
			+0.5f, -0.5f, -0.5f, +0.5f, -0.5f, +0.5f,
		};

		static float x_neg_face[] = {
			// -X
			-0.5f, +0.5f, +0.5f, -0.5f, -0.5f, +0.5f,
			-0.5f, -0.5f, -0.5f, -0.5f, +0.5f, -0.5f,
		};

		static float *face_verts[] = {z_pos_face, z_neg_face, x_pos_face,
									  x_neg_face, y_pos_face, y_neg_face};
	} // namespace World
} // namespace Minecraft