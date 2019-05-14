#pragma once

#include <GLM/glm.hpp>

#include <string>

class OBJData {
private:
	std::string filepath;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
public:

	OBJData(std::string filepath, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale): filepath(filepath), position(position), rotation(rotation), scale(scale) {}



	~OBJData() {}



	//GETTERS
	inline std::string getFilepath() { return this->filepath; }
	
	inline glm::vec3 getPosition() { return this->position; }
	inline glm::vec3 getRotation() { return this->rotation; }
	inline glm::vec3 getScale() { return this->scale; }
};
