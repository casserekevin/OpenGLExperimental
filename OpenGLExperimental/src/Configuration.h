#pragma once

//#include <GLM/glm.hpp>

#include <vector>

#include "OBJData.h"
#include "Camera.h"

class Configuration {
private:
	bool stop;
	bool render;

	int width;
	int height;

	Camera* camera;

	std::vector<OBJData *> datas;

public:
	Configuration() {}

	void addStop(std::string stop) {
		if (stop == "y") {
			this->stop = true;
		}
		else if (stop == "n") {
			this->stop = false;
		}
	}

	void addRender(std::string render) {
		if (render == "y") {
			this->render = true;
		}
		else if (render == "n") {
			this->render = false;
		}
	}

	void addViewport(int width, int height) {
		this->width = width;
		this->height = height;
	}

	void addCamera(glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::vec3 cameraUp) {
		this->camera = new Camera(cameraPosition, cameraFront, cameraUp);
	}

	void addOBJ(std::string file, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
		this->datas.push_back(new OBJData(file, position, rotation, scale));
	}



	~Configuration() {}



	//GETTERS
	inline bool getStop() { return this->stop; }
	inline bool getRender() { return this->render; }
	inline int getWidth() { return this->width; }
	inline int getHeigth() { return this->height; }
	inline Camera* getCamera() { return this->camera; }
	inline int getNumberOfData() { return this->datas.size(); }
	inline OBJData* getOBJDataAt(int index) { return this->datas.at(index); }
};
