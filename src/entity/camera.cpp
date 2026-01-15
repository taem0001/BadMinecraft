#include "../../include/entity/camera.h"
#include "glm/geometric.hpp"

namespace Minecraft {
	namespace Entity {
		Camera::Camera(glm::vec3 p, glm::vec3 u, float y, float pi)
			: front(glm::vec3(0.0f, 0.0f, -1.0f)), movespeed(SPEED),
			  mousesens(SENSITIVITY) {
			pos = p;
			worldup = u;
			yaw = y;
			pitch = pi;
			updateCamVects();
		}

		Camera::Camera(float posX, float posY, float posZ, float upX, float upY,
					   float upZ, float y, float p)
			: front(glm::vec3(0.0f, 0.0f, -1.0f)), movespeed(SPEED),
			  mousesens(SENSITIVITY) {
			pos = glm::vec3(posX, posY, posZ);
			worldup = glm::vec3(upX, upY, upZ);
			yaw = y;
			pitch = p;
			updateCamVects();
		}

		glm::mat4 Camera::getViewMat() {
			return glm::lookAt(pos, pos + front, up);
		}

		void Camera::processKey(CamMovement move, float deltatime) {
			float vel = movespeed * deltatime;
			switch (move) {
			case FORWARD:
				pos += front * vel;
				break;
			case BACKWARD:
				pos -= front * vel;
				break;
			case LEFT:
				pos -= right * vel;
				break;
			case RIGHT:
				pos += right * vel;
				break;
			default:
				break;
			}
		}

		void Camera::processMouse(float xoffset, float yoffset,
								  GLboolean constrainpitch = true) {
			xoffset *= mousesens;
			yoffset *= mousesens;
			yaw += xoffset;
			pitch += yoffset;

			if (constrainpitch) {
				if (pitch > 89.0f)
					pitch = 89.0f;
				if (pitch < -89.0f)
					pitch = -89.0f;
			}

			updateCamVects();
		}

		void Camera::updateCamVects() {
			glm::vec3 f;
			f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			f.y = sin(glm::radians(pitch));
			f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			front = glm::normalize(f);
			right = glm::normalize(glm::cross(front, worldup));
			up = glm::normalize(glm::cross(right, front));
		}
	} // namespace Entity
} // namespace Minecraft
