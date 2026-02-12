#include "../../include/entity/camera.hpp"

namespace Minecraft {
	namespace Entity {
		Camera::Camera(glm::vec3 p, glm::vec3 u, double y, double pi, double n,
					   double f)
			: front(glm::vec3(0.0f, 0.0f, -1.0f)), movespeed(SPEED),
			  mousesens(SENSITIVITY), fovy(FOVY) {
			pos = p;
			worldup = u;
			yaw = y;
			pitch = pi;
			near = n;
			far = f;
			updateCamVects();
		}

		Camera::Camera(double posX, double posY, double posZ, double upX,
					   double upY, double upZ, double y, double p, double n,
					   double f)
			: front(glm::vec3(0.0f, 0.0f, -1.0f)), movespeed(SPEED),
			  mousesens(SENSITIVITY), fovy(FOVY) {
			pos = glm::vec3(posX, posY, posZ);
			worldup = glm::vec3(upX, upY, upZ);
			yaw = y;
			pitch = p;
			near = n;
			far = f;
			updateCamVects();
		}

		glm::mat4 Camera::getViewMat() {
			return glm::lookAt(pos, pos + front, up);
		}

		void Camera::processKey(CamMovement move, double deltatime) {
			double vel = movespeed * deltatime;
			switch (move) {
			case FORWARD:
				pos += front * CASTTOFLOAT(vel);
				break;
			case BACKWARD:
				pos -= front * CASTTOFLOAT(vel);
				break;
			case LEFT:
				pos -= right * CASTTOFLOAT(vel);
				break;
			case RIGHT:
				pos += right * CASTTOFLOAT(vel);
				break;
			case UP:
				pos += worldup * CASTTOFLOAT(vel);
				break;
			case DOWN:
				pos -= worldup * CASTTOFLOAT(vel);
			default:
				break;
			}
		}

		void Camera::processMouse(double xoffset, double yoffset,
								  GLboolean constrainpitch, GLboolean constrainyaw) {
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
			if (constrainyaw) {
				if (yaw > 89.0f) 
					yaw = 89.0f;
				if (yaw < -89.0f)
					yaw = -89.0f;
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
