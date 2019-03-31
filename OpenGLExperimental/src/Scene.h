#pragma once

#include <GLEW/glew.h>

class Scene{

public:
	virtual void update() = 0;

	virtual void setWindow(GLFWwindow* window) = 0;

	virtual void processKeyboardInput() = 0;
	virtual void processMouseMovementInput(double xpos, double ypos) = 0;
	virtual void processMouseZoomInput(double xoffset, double yoffset) = 0;
	virtual void processFrameBufferSize(int width, int height) = 0;

	virtual ~Scene() {

	}
};

