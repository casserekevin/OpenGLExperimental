#pragma once

#include <GLM/glm.hpp>

#include <string>

class OBJData {
private:
	std::string file;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
public:

	OBJData(std::string file, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale): file(file), position(position), rotation(rotation), scale(scale) {}

	std::string getFilepath() {
		return this->file;
	}

	glm::vec3 getPosition() {
		return this->position;
	}

	glm::vec3 getRotation() {
		return this->rotation;
	}

	glm::vec3 getScale() {
		return this->scale;
	}
};
