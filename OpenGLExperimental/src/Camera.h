#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include <math.h>

class Camera {
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	float speed;

	//Angles
	float yaw;
	float pitch;
	float sensivity;

	float fov;

public:

	Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up) : position(position), front(front), up(up) {
		this->yaw = -90.0f;
		this->pitch = 0.0f;

		this->fov = 44.0f;

		this->sensivity = 0.1f;
	}

	void calculateSpeedRate(float deltaTime) {
		this->speed = 4 * deltaTime;
	}

	void moveForward() {
		this->position += this->front * this->speed;
	}

	void moveBackward() {
		this->position -= this->front * this->speed;
	}

	void moveRight() {
		this->position += glm::normalize(glm::cross(this->front, this->up)) * this->speed;
	}

	void moveLeft() {
		this->position -= glm::normalize(glm::cross(this->front, this->up)) * this->speed;
	}

	glm::mat4 generateViewMatrix() {
		return glm::lookAt(this->position, this->position + this->front, this->up);
	}



	void calculateYawAngle(float xoffset) {
		this->yaw += xoffset;
	}

	void calculatePitchAngle(float yoffset) {
		this->pitch += yoffset;
		if (this->pitch > 89.0f) {
			this->pitch = 89.0f;
		}
		else if (this->pitch < -89.0f) {
			this->pitch = -89.0f;
		}
	}

	void rotate() {
		glm::vec3 vec;
		vec.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		vec.y = sin(glm::radians(this->pitch));
		vec.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(vec);
	}



	void calculateFOV(double yoffset) {
		if (this->fov >= 1.0f && this->fov <= 45.0f) {
			this->fov -= yoffset;
		}
		if (this->fov < 1.0f) {
			this->fov = 1.0f;
		}
		if (this->fov > 45.0f) {
			this->fov = 45.0f;
		}
	}



	~Camera() {}



	//GETTERS
	inline glm::vec3 getPosition() { return this->position; }
	inline float getFOV() { return this->fov; }
	inline float getSensivity() { return this->sensivity; }

	//SETTERS
};
