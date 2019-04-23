#pragma once

//#include <GLM/glm.hpp>

#include <vector>

#include "OBJData.h"

class Configuration {
private:
	int widthViewport;
	int heightViewport;

	glm::vec3 cameraPosition;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	std::vector<OBJData *> datas;

public:
	Configuration() {}

	void addViewport(int width, int height) {
		this->widthViewport = width;
		this->heightViewport = height;
	}

	void addCamera(glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::vec3 cameraUp) {
		this->cameraPosition = cameraPosition;
		this->cameraFront = cameraFront;
		this->cameraUp = cameraUp;
	}

	void addOBJ(std::string file, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
		this->datas.push_back(new OBJData(file, position, rotation, scale));
	}

	int getWidthViewport() {
		return widthViewport;
	}

	int getHeigthViewport() {
		return heightViewport;
	}

	glm::vec3 getCameraPos() {
		return this->cameraPosition;
	}

	glm::vec3 getCameraFront() {
		return this->cameraFront;
	}

	glm::vec3 getCameraUp() {
		return this->cameraUp;
	}

	int numberOfData() {
		return datas.size();
	}

	OBJData* getOBJDataAt(int index) {
		return datas.at(index);
	}
};
