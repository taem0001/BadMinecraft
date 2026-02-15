#pragma once

#include "../gfx/gfx.hpp"
#include "../util/includes.hpp"
#include "../util/macros.hpp"
#include "../util/types.hpp"

namespace Minecraft {
	namespace GFX {
		enum CamMovement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

		const double YAW = -90.0;
		const double PITCH = 0.0f;
		const double SPEED = 7.0f;
		const double SENSITIVITY = 0.1f;
		const double FOVY = 45.0f;
		const double NEAR = 0.1f;
		const double FAR = 100.0f;

		class Camera {
			public:
				glm::vec3 pos, front, up, right, worldup;
				double yaw, pitch;
				double movespeed;
				double mousesens;
				double fovy;
				double near, far;

				Camera(glm::vec3 = glm::vec3(0.0f, 0.0f, 0.0f),
					   glm::vec3 = glm::vec3(0.0f, 1.0f, 0.0f), double = YAW,
					   double = PITCH, double = NEAR, double = FAR);
				Camera(double, double, double, double, double, double, double,
					   double, double, double);

				glm::mat4 getViewMat();
				glm::mat4 getProjMat(int width, int height);
				void processKey(CamMovement, double);
				void processMouse(double, double, GLboolean = true,
								  GLboolean = false);

			private:
				void updateCamVects();
		};
	} // namespace GFX
} // namespace Minecraft
