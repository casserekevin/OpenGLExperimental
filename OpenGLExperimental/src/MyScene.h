#pragma once
#include <GLM/glm.hpp>

#include "Scene.h"
#include "Speed.h"
#include "VertexBuffer.h"

class MyScene : public Scene{

private:
	GLFWwindow* m_windowThatIsInserted;
	
	int m_width;
	int m_height;

	unsigned int m_VertexShaderID;
	unsigned int m_FragmentShaderID;
	unsigned int m_ProgramID;

	unsigned int m_vaoID;

	VertexBuffer* m_vboPositions;
	VertexBuffer* m_vboColors;


	//-------------------------------------------------------
	//DADOS 
	//time
	float m_deltaTime = 0.0f;
	
	//matrizes
	//model matrix
	int m_modelMatrix;
	Speed* m_speed = new Speed(1.0f, 0.2f);

	float m_posicaoAtualX = 0.0f;
	float m_posicaoAtualY = 0.0f;

	float m_topPosition;
	float m_bottomPosition;
	float m_leftPosition;
	float m_rightPosition;
	
	//view matrix
	int m_viewMatrix;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f); //camera
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	bool m_firstMouse = true;
	float m_yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float m_pitch = 0.0f;
	float m_lastX = (float)m_width / 2.0;
	float m_lastY = (float)m_height / 2.0;

	//projection matrix
	int m_projectionMatrix;
	float m_fov = 45.0f;
	//---------------------------------------------------------


	//Funcoes:
	//Verifica erro na compilação do shader e na linkagem do programa
	int verifyShaderCompilation(unsigned int shaderID);
	int verifyProgramLink(unsigned int programID);
	
	unsigned int createShader(std::string shaderSource, unsigned int type);	

	void processKeyboardInput() override;
	void processMouseMovementInput(double xpos, double ypos) override;
	void processMouseZoomInput(double xoffset, double yoffset) override;

public:
	MyScene();
	MyScene(int width, int height);

	void update() override;

	void setNewResizeProjectionMatrix() override;

	//setters
	inline void setWidth(int width) override{
		m_width = width;
	}
	inline void setHeight(int height) override{
		m_height = height;
	}
	inline void setWindow(GLFWwindow* window) {
		m_windowThatIsInserted = window;
	}


	~MyScene();
};

