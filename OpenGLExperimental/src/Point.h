#pragma once

#include <GLM/glm.hpp>

class Point {
private:
	glm::vec3* position;
	glm::vec3* color;

public:
	Point() {}
	Point(glm::vec3* position, glm::vec3* color): position(position), color(color){}



	void increaseHeight(double rate, int maxHeight, bool controlPoints) {
		if (controlPoints) {
			this->position->z += rate;
			if (this->position->z > maxHeight) {
				this->position->z = maxHeight;
			}

			double qtdVezes = maxHeight / rate;
			double porcentagem = 1 / qtdVezes;

			this->color->x -= porcentagem;
			this->color->y -= porcentagem;
			if (this->color->x < 0 && this->color->y < 0) {
				this->color->x = 0;
				this->color->y = 0;
			}
		}
	}

	void decreaseHeight(double rate, int maxHeight, bool controlPoints) {
		if (controlPoints) {
			this->position->z -= rate;
			if (this->position->z < 0) {
				this->position->z = 0;
			}

			double qtdVezes = maxHeight / rate;
			double porcentagem = 1 / qtdVezes;

			this->color->x += porcentagem;
			this->color->y += porcentagem;
			if (this->color->x > 1 && this->color->y > 1) {
				this->color->x = 1;
				this->color->y = 1;
			}
		}
	}



	~Point() {}

	//GETTERS
	inline glm::vec3* getPosition() { return this->position; }

	//SETTERS
	void setPosition(glm::vec3* position) { this->position = position; }
	void setColor(glm::vec3* color) { this->color = color; }
};