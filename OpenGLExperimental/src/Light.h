#pragma once

#include <GLM/glm.hpp>

class Light {
private:
	glm::vec3 position;
	glm::vec3 color;
public:
	Light(glm::vec3 position, glm::vec3 color): position(position), color(color){}



	~Light() {}



	//GETTERS
	inline glm::vec3 getPosition() { return this->position; }
	inline glm::vec3 getColor() { return this->color; }
};
