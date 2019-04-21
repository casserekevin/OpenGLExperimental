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

#include "Program.h"
#include "Scene.h"
#include "OpenGLErrors.h"
#include "Normal.h"
#include "Speed.h"
#include "LightFunction.h"
#include "VertexBuffer.h"
#include "Texture.h"

#include "OBJClasses/OBJ.h"
#include "OBJClasses/Reader/MeshReader.h"


class MyScene : public Scene{

private:
	GLFWwindow* m_windowThatIsInserted;
	
	int m_width;
	int m_height;

	Program* program;
	OBJ* obj1;
	OBJ* obj2;

	unsigned int m_vaoID;

	VertexBuffer* m_vboPositions;
	VertexBuffer* m_vboNormals;
	VertexBuffer* m_vboTextures;
	VertexBuffer* m_vboColors;
	Texture* texture0;

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

	int clickTecla = 0;

	//Funcoes:
	void processKeyboardInput() override {
		if (glfwGetKey(m_windowThatIsInserted, GLFW_KEY_1) == GLFW_PRESS) {
			if (this->clickTecla == 0) {
				obj1->addColor(glm::vec3(1.f, 0.f, 0.f));
				clickTecla++;
			}
			else {
				obj1->removeColor();
				clickTecla = 0;
			}
		}
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
		program = new Program("res/shaders/vertex.shader", "res/shaders/fragment.shader");

		MeshReader* meshReader = new MeshReader();
		Mesh* mesh1 = meshReader->loadMesh("res/obj/cube2.obj");
		obj1 = new OBJ(mesh1, program);
		Mesh* mesh2 = meshReader->loadMesh("res/obj/cube.obj");
		obj2 = new OBJ(mesh2, program);


		//-------------------------------------------------------------------------------------
		//DADOS
		//Vertex Array Object
		//glGenVertexArrays(1, &m_vaoID);
		//glBindVertexArray(m_vaoID);

		////Vertex Buffer Object das Posiçoes
		//std::vector<glm::vec3> vec_positions = {
		//	glm::vec3(0.0f, 0.5f, 0.0f),
		//	glm::vec3(0.5f, -0.5f, 0.0f),
		//	glm::vec3(-0.5f, -0.5f, 0.0f),
		//	glm::vec3(0.0f, 0.5f, 0.0f),
		//	glm::vec3(-0.5f, -0.5f, 0.0f),
		//	glm::vec3(-1.0f, 0.5f, 0.0f)
		//};

		//m_vboPositions = new VertexBuffer(vec_positions.size() * sizeof(glm::vec3), vec_positions.data());

		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		//std::vector<glm::vec3> vec_normals = {
		//	glm::vec3(1.0f, 0.0f, 0.0f),
		//	glm::vec3(0.0f, 1.0f, 0.0f),
		//	glm::vec3(0.0f, 0.0f, 1.0f),
		//	glm::vec3(1.0f, 0.0f, 0.0f),
		//	glm::vec3(0.0f, 1.0f, 0.0f),
		//	glm::vec3(0.0f, 0.0f, 1.0f)
		//};
		//m_vboNormals = new VertexBuffer(vec_normals.size() * sizeof(glm::vec3), vec_normals.data());

		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		//std::vector<glm::vec2> vec_textures = {
		//	glm::vec2(0.5f, 1.0f),
		//	glm::vec2(1.0f, 0.0f),
		//	glm::vec2(0.0f, 0.0f),
		//	glm::vec2(0.5f, 1.0f),
		//	glm::vec2(1.0f, 0.0f),
		//	glm::vec2(0.0f, 0.0f)
		//};
		///*std::vector<glm::vec2> vec_textures = {
		//	glm::vec2(-1.f, -1.f),
		//	glm::vec2(-1.f, -1.f),
		//	glm::vec2(-1.f, -1.f),
		//	glm::vec2(-1.f, -1.f),
		//	glm::vec2(-1.f, -1.f),
		//	glm::vec2(-1.f, -1.f)
		//};*/
		//m_vboTextures = new VertexBuffer(vec_textures.size() * sizeof(glm::vec2), vec_textures.data());

		//glEnableVertexAttribArray(2);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		//std::vector<glm::vec3> vec_colors = {
		//	glm::vec3(1.0f, 0.0f, 0.0f),
		//	glm::vec3(0.0f, 1.0f, 0.0f),
		//	glm::vec3(0.0f, 0.0f, 1.0f),
		//	glm::vec3(1.0f, 0.0f, 0.0f),
		//	glm::vec3(0.0f, 1.0f, 0.0f),
		//	glm::vec3(0.0f, 0.0f, 1.0f)
		//};
		///*std::vector<glm::vec3> vec_colors = {
		//	glm::vec3(-2.f, -2.f, -2.f),
		//	glm::vec3(-2.f, -2.f, -2.f),
		//	glm::vec3(-2.f, -2.f, -2.f),
		//	glm::vec3(-2.f, -2.f, -2.f),
		//	glm::vec3(-2.f, -2.f, -2.f),
		//	glm::vec3(-2.f, -2.f, -2.f),
		//};*/
		//m_vboColors = new VertexBuffer(vec_colors.size() * sizeof(glm::vec3), vec_colors.data());

		//glEnableVertexAttribArray(3);
		//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		//glBindVertexArray(0);


		//Criação e passagem de textura
		//texture0 = new Texture("res/textures/wall.png", GL_TEXTURE_2D, 0);
		//program->send1i("texture0", obj1->getFirstTextureUnit());
		//---------------------------------------------------------------------------------------	
		/*program->send1i("typeDraw", obj1->getFirstTypeDraw());
		program->sendVec3fv("color", glm::vec3(0.f, 0.f, 0.f));*/


		//---------------------------------------------------------------------------------------
		//PASSAGEM DAS MATRIZES
		//Passagem da model matrix
		/*glm::mat4 modelMatrix(1.0f);
		program->sendMat4fv("modelMatrix", modelMatrix);*/

		//Passagem da view matrix
		glm::mat4 viewMatrix(1.0f);
		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		program->sendMat4fv("viewMatrix", viewMatrix);

		//Passagem da projection matrix
		glm::mat4 projectionMatrix(1.0f);
		projectionMatrix = glm::perspective(glm::radians(m_fov), (float)m_width / (float)m_height, 0.1f, 100.0f);
		program->sendMat4fv("projectionMatrix", projectionMatrix);
		//---------------------------------------------------------------------------------------

		/*m_topPosition = positions[1];
		m_bottomPosition = positions[4];
		m_rightPosition = positions[3];
		m_leftPosition = positions[6];*/
	}

	void update() override {
		glClearColor(0.5f, 0.5f, 1, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		obj1->draw();
		obj2->draw();
		//---------------------------------------------------------
		//Lógica do tempo
		static float lastFrame = glfwGetTime();
		float currentFrame = glfwGetTime();
		m_deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		//Normal normal;
		//if ((m_rightPosition + m_posicaoAtualX) >= 1.0f) {
		//	normal.x = -1;
		//	normal.y = 0;
		//	LightReflection(normal, m_speed);
		//}
		//else if ((m_leftPosition + m_posicaoAtualX) <= -1.0f) {
		//	normal.x = 1;
		//	normal.y = 0;
		//	LightReflection(normal, m_speed);
		//}
		//if ((m_topPosition + m_posicaoAtualY) >= 1.0f) {
		//	normal.x = 0;
		//	normal.y = -1;
		//	LightReflection(normal, m_speed);
		//}
		//else if ((m_bottomPosition + m_posicaoAtualY) <= -1.0f) {
		//	normal.x = 0;
		//	normal.y = -1;
		//	LightReflection(normal, m_speed);
		//}

		//std::cout << "Posicao Atual X: " << m_posicaoAtualX << std::endl;
		//std::cout << "Posicao Atual Y: " << m_posicaoAtualY << std::endl;
		//std::cout << "Width: " << m_width << " Height: " << m_height << std::endl;
		//std::cout << "------------------------------------------------" << std::endl;
		////-------------------------------------------------------------------------------

		//glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_deltaTime * m_speed.x + m_posicaoAtualX, m_deltaTime * m_speed.y + m_posicaoAtualY, 0.0f));
		//program->sendMat4fv("modelMatrix", modelMatrix);
		//m_posicaoAtualX = glm::value_ptr(modelMatrix)[12];
		//m_posicaoAtualY = glm::value_ptr(modelMatrix)[13];

		glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		program->sendMat4fv("viewMatrix", viewMatrix);
	}

	//setters
	inline void setWindow(GLFWwindow* window) {
		m_windowThatIsInserted = window;
	}


	~MyScene() {}
};

