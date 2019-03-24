#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
//glm includes
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>
#include <math.h>

#include "MyScene.h"
#include "ShaderReader.h"
#include "OpenGLErrors.h"
#include "Normal.h"
#include "LightFunction.h"

//MACRO DE DEBUG
#define ASSERT(x) if(!(x)) __debugbreak();
#define DEBUG_ERROR(x) clearErrors();\
	x;\
	ASSERT(logErrors(#x, __FILE__, __LINE__))

MyScene::MyScene() {

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

	//Vertex Buffer Object das Posi�oes
	float positions[9] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};
	glGenBuffers(1, &m_vboPositionsID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboPositionsID);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);

	//Vertex Buffer Object das Cores
	float colors[9] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
	glGenBuffers(1, &m_vboColorsID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboColorsID);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colors, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);

	//---------------------------------------------------------------------------------------	

	//usa o program
	glUseProgram(m_ProgramID);
	//Passagem da matriz 
	m_matrixLocation = glGetUniformLocation(m_ProgramID, "matrix");
	glUniformMatrix4fv(m_matrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

	m_topPosition = positions[1];
	m_bottomPosition = positions[4];
	m_rightPosition = positions[3];
	m_leftPosition = positions[6];
}

void MyScene::update() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DEBUG_ERROR(glDrawArrays(GL_TRIANGLES, 0, 3));

	//---------------------------------------------------------
	//L�gica do tempo
	static double previousSeconds = glfwGetTime();
	double currentSeconds = glfwGetTime();
	double elapsedSeconds = currentSeconds - previousSeconds;
	previousSeconds = currentSeconds;


	Normal normal;
	if ((m_rightPosition + m_posicaoAtualX) >= 1.0f){
		normal.x = -1;
		normal.y = 0;
		LightReflection(normal, m_speed);
	}
	else if ((m_leftPosition + m_posicaoAtualX) <= -1.0f){
		normal.x = 1;
		normal.y = 0;
		LightReflection(normal, m_speed);
	}
	if ((m_topPosition + m_posicaoAtualY) >= 1.0f) {
		normal.x = 0;
		normal.y = -1;
		LightReflection(normal, m_speed);
	}
	else if ((m_bottomPosition + m_posicaoAtualY)  <= -1.0f) {
		normal.x = 0;
		normal.y = -1;
		LightReflection(normal, m_speed);
	}
	
	std::cout << "Posicao Atual X: " << m_posicaoAtualX << std::endl;
	std::cout << "Posicao Atual Y: " << m_posicaoAtualY << std::endl;
	std::cout << "------------------------------------------------" << std::endl;

	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), glm::vec3(elapsedSeconds * m_speed->x + m_posicaoAtualX, elapsedSeconds * m_speed->y + m_posicaoAtualY, 0.0f));
	
	m_posicaoAtualX = glm::value_ptr(matrix)[12];
	m_posicaoAtualY = glm::value_ptr(matrix)[13];
	//-------------------------------------------------------------------------------

	glUniformMatrix4fv(m_matrixLocation, 1, GL_FALSE, glm::value_ptr(matrix));
	//glUniformMatrix4fv(m_matrixLocation, 1, GL_FALSE, glm::value_ptr(m_matrix));
}

int MyScene::verifyShaderCompilation(unsigned int shaderID) {
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

int MyScene::verifyProgramLink(unsigned int programID) {
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

unsigned int MyScene::createShader(std::string shaderSource, unsigned int type) {
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

MyScene::~MyScene() {
	glDeleteProgram(m_ProgramID);
}
