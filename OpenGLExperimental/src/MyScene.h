#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
//glm includes
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>
#include <math.h>

#include "Scene.h"
#include "OpenGLErrors.h"
#include "Normal.h"
#include "Speed.h"
#include "LightFunction.h"
#include "ShaderReader.h"
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
	Speed m_speed = Speed(1.0f, 0.2f);

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
	int verifyShaderCompilation(unsigned int shaderID) {
		int result;

		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			int length;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
			int type;
			glGetShaderiv(shaderID, GL_SHADER_TYPE, &type);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(shaderID, length, &length, message);

			std::string typeS;
			if (type == GL_VERTEX_SHADER) {
				typeS = "vertex";
			}
			else if (type == GL_FRAGMENT_SHADER) {
				typeS = "fragment";
			}
			else {
				typeS = "geometry";
			}
			std::cout << "Failed to compile " << typeS << " shader" << std::endl;
			std::cout << message << std::endl;

		}

		return result;
	}
	int verifyProgramLink(unsigned int programID) {
		int result;

		glGetProgramiv(programID, GL_LINK_STATUS, &result);
		if (result == GL_FALSE) {
			int length;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetProgramInfoLog(programID, length, &length, message);

			std::cout << "Failed to link program" << std::endl;
			std::cout << message << std::endl;
		}

		return result;
	}
	
	unsigned int createShader(std::string shaderSource, unsigned int type) {
		const char* buff;

		buff = shaderSource.c_str();

		unsigned int shaderID = glCreateShader(type);
		glShaderSource(shaderID, 1, &buff, NULL);
		glCompileShader(shaderID);
		verifyShaderCompilation(shaderID);
		glAttachShader(m_ProgramID, shaderID);
		glCompileShader(shaderID);

		return shaderID;
	}

	void processKeyboardInput() override {
		if (glfwGetKey(m_windowThatIsInserted, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(m_windowThatIsInserted, true);
		}
		float cameraSpeed = 2.5 * m_deltaTime;
		if (glfwGetKey(m_windowThatIsInserted, GLFW_KEY_W) == GLFW_PRESS) {
			cameraPos += cameraSpeed * cameraFront;
		}
		if (glfwGetKey(m_windowThatIsInserted, GLFW_KEY_S) == GLFW_PRESS) {
			cameraPos -= cameraSpeed * cameraFront;
		}
		if (glfwGetKey(m_windowThatIsInserted, GLFW_KEY_A) == GLFW_PRESS) {
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (glfwGetKey(m_windowThatIsInserted, GLFW_KEY_D) == GLFW_PRESS) {
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
	}
	void processMouseMovementInput(double xpos, double ypos) override {
		std::cout << "mouse X: " << xpos << " mouse Y: " << ypos << std::endl;
		if (m_firstMouse)
		{
			m_lastX = xpos;
			m_lastY = ypos;
			m_firstMouse = false;
		}

		float xoffset = xpos - m_lastX;
		float yoffset = m_lastY - ypos; // reversed since y-coordinates go from bottom to top
		m_lastX = xpos;
		m_lastY = ypos;

		float sensitivity = 0.1f; // change this value to your liking
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_yaw += xoffset;
		m_pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		cameraFront = glm::normalize(front);
	}
	void processMouseZoomInput(double xoffset, double yoffset) override {
		if (m_fov >= 1.0f && m_fov <= 45.0f) {
			m_fov -= yoffset;
		}
		if (m_fov <= 1.0f) {
			m_fov = 1.0f;
		}
		if (m_fov >= 45.0f) {
			m_fov = 45.0f;
		}
		glm::mat4 projectionMatrix(1.0f);
		projectionMatrix = glm::perspective(glm::radians(m_fov), static_cast<float>(m_width) / static_cast<float>(m_height), 0.1f, 100.0f);
		glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}

public:
	MyScene(){}
	MyScene(int width, int height) : m_width(width), m_height(height) {
		glClearColor(0.5f, 0.5f, 1, 0);

		//le os arquivos dos shaders
		std::stringstream vertexSourceStream;
		std::stringstream fragmentSourceStream;

		loadFile("res/shaders/vertex.shader", vertexSourceStream);
		loadFile("res/shaders/fragment.shader", fragmentSourceStream);

		std::string vertexSource = vertexSourceStream.str();
		std::string fragmentSource = fragmentSourceStream.str();

		//criar o programa
		m_ProgramID = glCreateProgram();

		//cria o Vertex Shader
		m_VertexShaderID = createShader(vertexSource, GL_VERTEX_SHADER);
		//cria o Fragment Shader
		m_FragmentShaderID = createShader(fragmentSource, GL_FRAGMENT_SHADER);

		//linka o program
		glLinkProgram(m_ProgramID);
		verifyProgramLink(m_ProgramID);

		//-------------------------------------------------------------------------------------
		//DADOS
		//Vertex Array Object
		glGenVertexArrays(1, &m_vaoID);
		glBindVertexArray(m_vaoID);

		//Vertex Buffer Object das Posiçoes
		float positions[9] = {
			0.0f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
		};
		m_vboPositions = new VertexBuffer(9 * sizeof(float), positions);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);

		//Vertex Buffer Object das Cores
		float colors[9] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		};
		m_vboColors = new VertexBuffer(9 * sizeof(float), colors);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);
		//---------------------------------------------------------------------------------------	

		//usa o program
		glUseProgram(m_ProgramID);

		//---------------------------------------------------------------------------------------
		//PASSAGEM DAS MATRIZES
		//Passagem da model matrix
		glm::mat4 modelMatrix(1.0f);
		m_modelMatrix = glGetUniformLocation(m_ProgramID, "modelMatrix");
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		//Passagem da view matrix
		glm::mat4 viewMatrix(1.0f);
		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		m_viewMatrix = glGetUniformLocation(m_ProgramID, "viewMatrix");
		glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

		//Passagem da projection matrix
		glm::mat4 projectionMatrix(1.0f);
		projectionMatrix = glm::perspective(glm::radians(m_fov), (float)m_width / (float)m_height, 0.1f, 100.0f);
		m_projectionMatrix = glGetUniformLocation(m_ProgramID, "projectionMatrix");
		glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		//---------------------------------------------------------------------------------------

		m_topPosition = positions[1];
		m_bottomPosition = positions[4];
		m_rightPosition = positions[3];
		m_leftPosition = positions[6];
	}

	void update() override {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//---------------------------------------------------------
		//Lógica do tempo
		static float lastFrame = glfwGetTime();
		float currentFrame = glfwGetTime();
		m_deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		Normal normal;
		if ((m_rightPosition + m_posicaoAtualX) >= 1.0f) {
			normal.x = -1;
			normal.y = 0;
			LightReflection(normal, m_speed);
		}
		else if ((m_leftPosition + m_posicaoAtualX) <= -1.0f) {
			normal.x = 1;
			normal.y = 0;
			LightReflection(normal, m_speed);
		}
		if ((m_topPosition + m_posicaoAtualY) >= 1.0f) {
			normal.x = 0;
			normal.y = -1;
			LightReflection(normal, m_speed);
		}
		else if ((m_bottomPosition + m_posicaoAtualY) <= -1.0f) {
			normal.x = 0;
			normal.y = -1;
			LightReflection(normal, m_speed);
		}

		std::cout << "Posicao Atual X: " << m_posicaoAtualX << std::endl;
		std::cout << "Posicao Atual Y: " << m_posicaoAtualY << std::endl;
		std::cout << "Width: " << m_width << " Height: " << m_height << std::endl;
		std::cout << "------------------------------------------------" << std::endl;
		//-------------------------------------------------------------------------------

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_deltaTime * m_speed.x + m_posicaoAtualX, m_deltaTime * m_speed.y + m_posicaoAtualY, 0.0f));
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		m_posicaoAtualX = glm::value_ptr(modelMatrix)[12];
		m_posicaoAtualY = glm::value_ptr(modelMatrix)[13];

		glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		//glUniformMatrix4fv(m_matrixLocation, 1, GL_FALSE, glm::value_ptr(m_matrix));
	}

	void setNewResizeProjectionMatrix() override {
		glm::mat4 projectionMatrix(1.0f);
		projectionMatrix = glm::perspective(glm::radians(m_fov), static_cast<float>(m_width) / static_cast<float>(m_height), 0.1f, 100.0f);
		glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}

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


	~MyScene() {
		glDeleteProgram(m_ProgramID);
	}
};

