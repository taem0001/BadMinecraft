#pragma once

#include "../gfx/gfx.h"
#include "glm/ext/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Minecraft {
	namespace Entity {
		enum CamMovement { FORWARD, BACKWARD, LEFT, RIGHT };

		const float YAW = -90.0;
		const float PITCH = 0.0f;
		const float SPEED = 2.5f;
		const float SENSITIVITY = 0.1f;
		const float ZOOM = 45.0f;

		class Camera {
			public:
				glm::vec3 pos, front, up, right, worldup;
				float yaw, pitch;
				float movespeed;
				float mousesens;

				Camera(glm::vec3 = glm::vec3(0.0f, 0.0f, 0.0f),
					   glm::vec3 = glm::vec3(0.0f, 1.0f, 0.0f), float = YAW,
					   float = PITCH);
				Camera(float, float, float, float, float, float, float, float);

				glm::mat4 getViewMat();
				void processKey(CamMovement, float);
				void processMouse(float, float, GLboolean);

			private:
				void updateCamVects();
		};
	} // namespace Entity
} // namespace Minecraft
