#pragma once

#include <GLEW/glew.h>

class Scene{

public:
	virtual void update() = 0;

	virtual void processKeyboard() = 0;
	virtual void processKeyboardInput(int key, int scancode, int action, int mods) = 0;
	virtual void processMouseMovementInput(double xpos, double ypos) = 0;
	virtual void processMouseZoomInput(double xoffset, double yoffset) = 0;
	virtual void processMouseClickInput(int button, int action, int mods) = 0;
	virtual void processFrameBufferSize(int width, int height) = 0;
	virtual void processCloseWindow() = 0;

	virtual ~Scene() {}
};

