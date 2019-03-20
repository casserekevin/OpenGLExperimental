#pragma once

#include <GLEW/glew.h>

class Scene{

public:
	virtual void update() = 0;

	virtual ~Scene() {

	}
};

