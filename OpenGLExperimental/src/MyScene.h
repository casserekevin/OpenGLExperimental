#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
//glm includes
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
//SOIL2
#include <SOIL2/SOIL2.h>

#include <iostream>
#include <math.h>

#include "Program.h"
#include "Scene.h"
#include "OpenGLErrors.h"
#include "Normal.h"
#include "Speed.h"
#include "LightFunction.h"
#include "VertexBuffer.h"


class MyScene : public Scene{

private:
	GLFWwindow* m_windowThatIsInserted;
	
	int m_width;
	int m_height;

	Program* program;

	unsigned int m_vaoID;

	VertexBuffer* m_vboPositions;
	VertexBuffer* m_vboColors;
	VertexBuffer* m_vboTextures;
	unsigned int m_texture;

	//-------------------------------------------------------
	//DADOS 
	//time
	float m_deltaTime = 0.0f;
	
	//model matrix
	Speed m_speed = Speed(1.0f, 0.2f);

	float m_posicaoAtualX = 0.0f;
	float m_posicaoAtualY = 0.0f;

	float m_topPosition;
	float m_bottomPosition;
	float m_leftPosition;
	float m_rightPosition;
	
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f); //camera
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	bool m_firstMouse = true;
	float m_yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float m_pitch = 0.0f;
	float m_lastX = (float)m_width / 2.0;
	float m_lastY = (float)m_height / 2.0;

	float m_fov = 45.0f;
	//---------------------------------------------------------


	//Funcoes:
	void processKeyboardInput() override {
		if (glfwGetKey(m_windowThatIsInserted, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(m_windowThatIsInserted, true);
		}
		if (glfwGetKey(m_windowThatIsInserted, GLFW_KEY_M) == GLFW_PRESS) {
			glfwSetInputMode(m_windowThatIsInserted, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		if (glfwGetKey(m_windowThatIsInserted, GLFW_KEY_N) == GLFW_PRESS) {
			glfwSetInputMode(m_windowThatIsInserted, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
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
		program->sendMat4fv("projectionMatrix", projectionMatrix);
	}
	void processFrameBufferSize(int width, int height) override {
		m_width = width;
		m_height = height;
		glm::mat4 projectionMatrix(1.0f);
		projectionMatrix = glm::perspective(glm::radians(m_fov), static_cast<float>(m_width) / static_cast<float>(m_height), 0.1f, 100.0f);
		program->sendMat4fv("projectionMatrix", projectionMatrix);
	}

public:
	//CONSTRUTORES
	MyScene(){}
	MyScene(int width, int height) : m_width(width), m_height(height) {
		
		//------------------------------------------------------------

		program = new Program("res/shaders/vertex.shader", "res/shaders/fragment.shader");

		//-------------------------------------------------------------------------------------
		//DADOS
		//Vertex Array Object
		glGenVertexArrays(1, &m_vaoID);
		glBindVertexArray(m_vaoID);

		//Vertex Buffer Object das Posi�oes
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

		//Vertex Buffer Object das texturas
		float textures[] = {
			0.0f, 0.0f,  // lower-left corner  
			1.0f, 0.0f,  // lower-right corner
			0.5f, 1.0f   // top-center corner
		};
		m_vboTextures = new VertexBuffer(6 * sizeof(float), textures);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);

		//--------
		//Carrega imagem
		int image_width, image_height;
		unsigned char* data_image = SOIL_load_image("res/textures/wall.png", &image_width, &image_height, NULL, SOIL_LOAD_RGBA);

		//Gera textura
		glGenTextures(1, &m_texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		//Configura Textura
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Caso o obj for maior que a textura no eixo X, repete a textura
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //Caso o obj for maior que a textura no eixo Y, repete a textura
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Filtro usado quando o objeto aumentar de tamanho
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Filtro usado quando o objeto diminuir de tamanho

		// Se a imagem carregou corretamente
		if (data_image) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_image);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "ERROR::TEXTURE_LOADING_FAILED" << std::endl;
		}
		SOIL_free_image_data(data_image);
		program->send1i("texture0", 0);
		//glUniform1i(glGetUniformLocation(m_ProgramID, "texture0"), 0);
		//---------------------------------------------------------------------------------------	


		//---------------------------------------------------------------------------------------
		//PASSAGEM DAS MATRIZES
		//Passagem da model matrix
		glm::mat4 modelMatrix(1.0f);
		program->sendMat4fv("modelMatrix", modelMatrix);
		//m_modelMatrix = glGetUniformLocation(m_ProgramID, "modelMatrix");
		//glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		//Passagem da view matrix
		glm::mat4 viewMatrix(1.0f);
		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		program->sendMat4fv("viewMatrix", viewMatrix);
		//m_viewMatrix = glGetUniformLocation(m_ProgramID, "viewMatrix");
		//glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

		//Passagem da projection matrix
		glm::mat4 projectionMatrix(1.0f);
		projectionMatrix = glm::perspective(glm::radians(m_fov), (float)m_width / (float)m_height, 0.1f, 100.0f);
		program->sendMat4fv("projectionMatrix", projectionMatrix);
		//m_projectionMatrix = glGetUniformLocation(m_ProgramID, "projectionMatrix");
		//---------------------------------------------------------------------------------------

		m_topPosition = positions[1];
		m_bottomPosition = positions[4];
		m_rightPosition = positions[3];
		m_leftPosition = positions[6];
	}

	void update() override {
		glClearColor(0.5f, 0.5f, 1, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program->use();
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//---------------------------------------------------------
		//L�gica do tempo
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
		program->sendMat4fv("modelMatrix", modelMatrix);
		//glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		m_posicaoAtualX = glm::value_ptr(modelMatrix)[12];
		m_posicaoAtualY = glm::value_ptr(modelMatrix)[13];

		glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		program->sendMat4fv("viewMatrix", viewMatrix);
		//glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		//glUniformMatrix4fv(m_matrixLocation, 1, GL_FALSE, glm::value_ptr(m_matrix));
	}

	//setters
	inline void setWindow(GLFWwindow* window) {
		m_windowThatIsInserted = window;
	}


	~MyScene() {}
};

