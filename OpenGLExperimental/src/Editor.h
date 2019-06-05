#pragma once

#include <GLEW/glew.h>
#include <GLM/glm.hpp>
//nclude <GLM/gtc/matrix_transform.hpp>
//#include <GLM/gtc/type_ptr.hpp>

#include <vector>

#include "Scene.h"

#include "Program.h"
#include "BSpline.h"
#include "VertexBuffer.h"
#include "Configuration.h"

class Editor : public Scene{
private:
	GLFWwindow* windowThatIsInserted;
	int width;
	int height;
	Configuration* configuration;

	Program* program;

	GLuint vaoID;

	BSpline* bSpline;

	void processKeyboard() {}
	void processKeyboardInput(int key, int scancode, int action, int mods) override {
		if (key == GLFW_KEY_ESCAPE) {
			switch (action) {
			case GLFW_PRESS:
				ConfigurationIO* configurationIO = new ConfigurationIO();
				configurationIO->replace("configuration.cfg", "s", "y");
				glfwSetWindowShouldClose(this->windowThatIsInserted, true);

				break;
			}
		}

		if (key == GLFW_KEY_S) {
			switch (action) {
			case GLFW_PRESS:
				ConfigurationIO* configurationIO = new ConfigurationIO();
				configurationIO->replace("configuration.cfg", "r", "y");
				glfwSetWindowShouldClose(this->windowThatIsInserted, true);

				break;
			}
		}
	}
	void processMouseMovementInput(double xpos, double ypos) override {}
	void processMouseZoomInput(double xoffset, double yoffset) override {}
	void processMouseClickInput(int button, int action, int mods) override {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			switch (action) {
			case GLFW_PRESS:
				double xpos, ypos;
				glfwGetCursorPos(this->windowThatIsInserted, &xpos, &ypos);
				ypos = this->height - ypos;
				std::cout << "Cursor Position (" << xpos << " : " << ypos << ")" << std::endl;

				this->bSpline->addCtrlPoints(xpos, ypos, 0);
				break;
			}
		}
	}
	void processFrameBufferSize(int width, int height) override {
		this->width = width;
		this->height = height;

		this->bSpline->setWidth(this->width);
		this->bSpline->setHeight(this->height);

		glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)this->width, 0.0f, (float)this->height);
		this->program->sendMat4fv("projectionMatrix", projectionMatrix);
	}

public:
	Editor(){}
	Editor(GLFWwindow* window, int width, int height, Configuration* configuration) : windowThatIsInserted(window), width(width), height(height), configuration(configuration) {
		this->program = new Program("editor/vertex.shader", "editor/fragment.shader");
	
		//Passagem da projection matrix
		glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)this->width, 0.0f, (float)this->height);
		this->program->sendMat4fv("projectionMatrix", projectionMatrix);
	
		this->bSpline = new BSpline(this->program, this->width, this->height);
	}

	void update() override{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->bSpline->draw();
	}


	~Editor() {}
};
