#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
//glm includes
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>
#include <math.h>
#include <vector>

#include "Scene.h"

#include "Program.h"
#include "OpenGLErrors.h"
#include "Normal.h"
#include "Speed.h"
#include "LightFunction.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Configuration.h"
#include "Camera.h"

#include "OBJClasses/OBJ.h"
#include "OBJClasses/IO/MeshIO.h"
#include "OBJClasses/IO/ConfigurationIO.h"


class Render : public Scene{

private:
	GLFWwindow* windowThatIsInserted;
	int width;
	int height;
	Configuration* configuration;

	Program* program;
	vector<OBJ* > objs;
	


	//-------------------------------------------------------
	//DADOS 
	//time
	float m_deltaTime = 0.0f;
	
	//model matrix
	Speed* speed = new Speed();

	float m_posicaoAtualX = 0.0f;
	float m_posicaoAtualY = 0.0f;
	
	Camera* camera;

	bool m_firstMouse = true;
	float m_lastX = (float)this->width / 2.0;
	float m_lastY = (float)this->height / 2.0;

	//---------------------------------------------------------

	//Key mapping
	bool is_SHIFT_down = false;
	bool is_R_down = false;
	bool is_S_down = false;
	bool is_X_down = false;
	bool is_Y_down = false;
	bool is_Z_down = false;

	void deselectAllMinus(int index) {
		for (int i = 0; i < objs.size(); i++) {
			if (i != index) {
				objs.at(i)->unselect();
			}
		}
	}

	OBJ* searchFirstOBJSelected() {
		for (OBJ* obj : objs) {
			if (obj->isSelected()) {
				return obj;
			}
		}
		return nullptr;
	}

	//Funcoes:
	void processKeyboard() {
		this->camera->calculateSpeedRate(this->m_deltaTime);
		if (glfwGetKey(this->windowThatIsInserted, GLFW_KEY_UP) == GLFW_PRESS) {
			this->camera->moveForward();
		}
		if (glfwGetKey(this->windowThatIsInserted, GLFW_KEY_DOWN) == GLFW_PRESS) {
			this->camera->moveBackward();
		}
		if (glfwGetKey(this->windowThatIsInserted, GLFW_KEY_LEFT) == GLFW_PRESS) {
			this->camera->moveLeft();
		}
		if (glfwGetKey(this->windowThatIsInserted, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			this->camera->moveRight();
		}
	}
	void processKeyboardInput(int key, int scancode, int action, int mods) override {
		if (key == GLFW_KEY_ESCAPE) {
			switch (action) {
				case GLFW_PRESS:
					glfwSetWindowShouldClose(this->windowThatIsInserted, true);
					ConfigurationIO* configurationIO = new ConfigurationIO();
					configurationIO->replace("configuration.cfg", "s", "y");

					break;
			}
		}

		if (key == GLFW_KEY_1) {
			if (objs.size() > 0) {
				switch (action) {
					case GLFW_PRESS:
						deselectAllMinus(0);
						objs.at(0)->changeSelection();

						break;
				}

			}
		}

		if (key == GLFW_KEY_2) {
			if (objs.size() > 1) {
				switch (action) {
				case GLFW_PRESS:
					deselectAllMinus(1);
					objs.at(1)->changeSelection();

					break;
				}

			}
		}

		if (key == GLFW_KEY_3) {
			if (objs.size() > 2) {
				switch (action) {
				case GLFW_PRESS:
					deselectAllMinus(2);
					objs.at(2)->changeSelection();

					break;
				}

			}
		}

		if (key == GLFW_KEY_4) {
			if (objs.size() > 3) {
				switch (action) {
				case GLFW_PRESS:
					deselectAllMinus(3);
					objs.at(3)->changeSelection();

					break;
				}

			}
		}

		if (key == GLFW_KEY_5) {
			if (objs.size() > 4) {
				switch (action) {
				case GLFW_PRESS:
					deselectAllMinus(4);
					objs.at(4)->changeSelection();

					break;
				}

			}
		}

		if (key == GLFW_KEY_6) {
			if (objs.size() > 5) {
				switch (action) {
				case GLFW_PRESS:
					deselectAllMinus(5);
					objs.at(5)->changeSelection();

					break;
				}

			}
		}

		if (key == GLFW_KEY_7) {
			if (objs.size() > 6) {
				switch (action) {
				case GLFW_PRESS:
					deselectAllMinus(6);
					objs.at(6)->changeSelection();

					break;
				}

			}
		}

		if (key == GLFW_KEY_8) {
			if (objs.size() > 7) {
				switch (action) {
				case GLFW_PRESS:
					deselectAllMinus(7);
					objs.at(7)->changeSelection();

					break;
				}

			}
		}

		if (key == GLFW_KEY_9) {
			if (objs.size() > 8) {
				switch (action) {
				case GLFW_PRESS:
					deselectAllMinus(8);
					objs.at(8)->changeSelection();

					break;
				}

			}
		}

		if (!is_SHIFT_down && is_R_down) {
			OBJ* obj = searchFirstOBJSelected();
			if (obj != nullptr) {
				obj->rotationYnegative();
			}
		}
		if (is_SHIFT_down && is_R_down) {
			OBJ* obj = searchFirstOBJSelected();
			if (obj != nullptr) {
				obj->rotationYpositive();
			}
		}
		if (!is_SHIFT_down && is_S_down) {
			OBJ* obj = searchFirstOBJSelected();
			if (obj != nullptr) {
				obj->scaleMinus();
			}
		}
		if (is_SHIFT_down && is_S_down) {
			OBJ* obj = searchFirstOBJSelected();
			if (obj != nullptr) {
				obj->scalePlus();
			}
		}
		if (!is_SHIFT_down && is_X_down) {
			OBJ* obj = searchFirstOBJSelected();
			if (obj != nullptr) {
				obj->translateXpositive();
			}
		}
		if (is_SHIFT_down && is_X_down) {
			OBJ* obj = searchFirstOBJSelected();
			if (obj != nullptr) {
				obj->translateXnegative();
			}
		}
		if (!is_SHIFT_down && is_Y_down) {
			OBJ* obj = searchFirstOBJSelected();
			if (obj != nullptr) {
				obj->translateYpositive();
			}
		}
		if (is_SHIFT_down && is_Y_down) {
			OBJ* obj = searchFirstOBJSelected();
			if (obj != nullptr) {
				obj->translateYnegative();
			}
		}
		if (!is_SHIFT_down && is_Z_down) {
			OBJ* obj = searchFirstOBJSelected();
			if (obj != nullptr) {
				obj->translateZpositive();
			}
		}
		if (is_SHIFT_down && is_Z_down) {
			OBJ* obj = searchFirstOBJSelected();
			if (obj != nullptr) {
				obj->translateZnegative();
			}
		}

		if (key == GLFW_KEY_LEFT_SHIFT) {
			is_SHIFT_down = action == GLFW_REPEAT;
		}
		if (key == GLFW_KEY_R) {
			is_R_down = action == GLFW_REPEAT;
		}
		if (key == GLFW_KEY_S) {
			is_S_down = action == GLFW_REPEAT;
		}
		if (key == GLFW_KEY_X) {
			is_X_down = action == GLFW_REPEAT;
		}
		if (key == GLFW_KEY_Y) {
			is_Y_down = action == GLFW_REPEAT;
		}
		if (key == GLFW_KEY_Z) {
			is_Z_down = action == GLFW_REPEAT;
		}
	}
	void processMouseMovementInput(double xpos, double ypos) override {
		if (m_firstMouse){
			m_lastX = xpos;
			m_lastY = ypos;
			m_firstMouse = false;
		}

		float xoffset = xpos - m_lastX;
		float yoffset = m_lastY - ypos; // reversed since y-coordinates go from bottom to top
		m_lastX = xpos;
		m_lastY = ypos;

		xoffset *= this->camera->getSensivity();
		yoffset *= this->camera->getSensivity();

		this->camera->calculateYawAngle(xoffset);
		this->camera->calculatePitchAngle(yoffset);

		this->camera->rotate();
	}
	void processMouseZoomInput(double xoffset, double yoffset) override {
		this->camera->calculateFOV(yoffset);

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(this->camera->getFOV()), static_cast<float>(this->width) / static_cast<float>(this->height), 0.1f, 100.0f);
		program->sendMat4fv("projectionMatrix", projectionMatrix);
	}
	void processMouseClickInput(int button, int action, int mods) override{}
	void processFrameBufferSize(int width, int height) override {
		this->width = width;
		this->height = height;

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(this->camera->getFOV()), static_cast<float>(this->width) / static_cast<float>(this->height), 0.1f, 2000.0f);
		program->sendMat4fv("projectionMatrix", projectionMatrix);
	}
	void processCloseWindow() {
		ConfigurationIO* configurationIO = new ConfigurationIO();
		configurationIO->replace("configuration.cfg", "s", "y");
	}

public:
	//CONSTRUTORES
	Render(){}
	Render(GLFWwindow* window, int width, int height, Configuration* configuration) : windowThatIsInserted(window), width(width), height(height), configuration(configuration), camera(configuration->getCamera()) {
		program = new Program("render/vertex.shader", "render/fragment.shader");

		MeshIO* meshIO = new MeshIO();
		for (int i = 0; i < this->configuration->getNumberOfData(); i++) {
			OBJ* obj = new OBJ(meshIO->read(this->configuration->getOBJDataAt(i)->getFilepath()), this->configuration->getOBJDataAt(i), program);
			objs.push_back(obj);
		}

		//LIGHT
		this->program->sendLight(new Light(glm::vec3((this->width / 2) + (this->width / 4), 400.0f, this->height / 2), glm::vec3(1.0f, 1.0f, 1.0f)));

		//Passagem da projection matrix
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(this->camera->getFOV()), (float)this->width / (float)this->height, 0.1f, 2000.0f);;
		this->program->sendMat4fv("projectionMatrix", projectionMatrix);
	}

	void update() override {
		glClearColor(0.5f, 0.5f, 1, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Passagem da view matrix
		this->program->sendMat4fv("viewMatrix", this->camera->generateViewMatrix());
		this->program->sendVec3fv("cameraPos", this->camera->getPosition());

		for (int i = 0; i < objs.size(); i++) {
			objs.at(i)->draw();
		}
		//---------------------------------------------------------
		//Lógica do tempo
		static float lastFrame = glfwGetTime();
		float currentFrame = glfwGetTime();
		m_deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}



	~Render() {}



	//GETTERS

};

