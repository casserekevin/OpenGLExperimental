#pragma once

#include <GLEW/glew.h>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>

#include "OBJClasses/Reader/ShaderReader.h";


class Program {
private:
	GLuint programID;
	GLuint vertexShaderID = 0;
	GLuint fragmentShaderID = 0;
	GLuint geometryShaderID = 0;

	std::string vertexSource;
	std::string fragmentSource;
	std::string geometrySource = "";

	//-----------------------------------------------
	//constructor functions
	void loadShadersFile(const char* filepathVertex, const char* filepathFragment, const char* filepathGeometry) {
		std::stringstream vertexSourceStream;
		std::stringstream fragmentSourceStream;

		ShaderReader* shaderReader = new ShaderReader();
		shaderReader->read(filepathVertex, vertexSourceStream);
		shaderReader->read(filepathFragment, fragmentSourceStream);

		this->vertexSource = vertexSourceStream.str();
		this->fragmentSource = fragmentSourceStream.str();

		//se o geometry shader existir
		if (filepathGeometry != "") {
			std::stringstream geometrySourceStream;

			shaderReader->read(filepathGeometry, geometrySourceStream);

			this->geometrySource = geometrySourceStream.str();
		}
	}
	void createProgram() {
		this->programID = glCreateProgram();

		//cria o Vertex Shader
		this->vertexShaderID = createShader(this->vertexSource, GL_VERTEX_SHADER);
		glAttachShader(this->programID, this->vertexShaderID); //linka o shader ao programa
		//cria o Fragment Shader
		this->fragmentShaderID = createShader(this->fragmentSource, GL_FRAGMENT_SHADER);
		glAttachShader(this->programID, this->fragmentShaderID); //linka o shader ao programa
		//cria o Geometry Shader
		if (this->geometrySource != "") {
			this->geometryShaderID = createShader(this->geometrySource, GL_GEOMETRY_SHADER);
			glAttachShader(this->programID, this->geometryShaderID); //linka o shader ao programa
		}

		//linka o program
		glLinkProgram(this->programID);
		verifyProgramLink(this->programID); //verifica erros na linkagem do programa

		glUseProgram(0);
	}
	void deleteShaders() {
		glDeleteShader(this->vertexShaderID);
		glDeleteShader(this->fragmentShaderID);
		if (this->geometryShaderID != 0) {
			glDeleteShader(this->geometryShaderID);
		}
	}
	//-----------------------------------------------


	//-----------------------------------------------
	//auxiliar functions
	GLuint createShader(std::string shaderSource, GLenum type) {
		const char* buff = shaderSource.c_str();

		GLuint shaderID = glCreateShader(type);
		glShaderSource(shaderID, 1, &buff, NULL);
		glCompileShader(shaderID);
		verifyShaderCompilation(shaderID);

		return shaderID;
	}

	int verifyShaderCompilation(GLuint shaderID) {
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
	int verifyProgramLink(GLuint programID) {
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
	//-----------------------------------------------

public:

	Program(const char* filepathVertex, const char* filepathFragment, const char* filepathGeometry = "") {
		this->loadShadersFile(filepathVertex, filepathFragment, filepathGeometry);
		this->createProgram();
		this->deleteShaders();
	}

	//send methods
	void send1i(const GLchar* name, GLint value) {
		this->use();
		glUniform1i(glGetUniformLocation(this->programID, name), value);
		this->unuse();
	}

	void sendVec2fv(const GLchar* name, glm::fvec2 value) {
		this->use();
		glUniform2fv(glGetUniformLocation(this->programID, name), 1, glm::value_ptr(value));
		this->unuse();
	}

	void sendVec3fv(const GLchar* name, glm::fvec3 value) {
		this->use();
		glUniform3fv(glGetUniformLocation(this->programID, name), 1, glm::value_ptr(value));
		this->unuse();
	}

	void sendVec4fv(const GLchar* name, glm::fvec4 value) {
		this->use();
		glUniform4fv(glGetUniformLocation(this->programID, name), 1, glm::value_ptr(value));
		this->unuse();
	}

	void sendMat3fv(const GLchar* name, glm::mat3 value, GLboolean transpose = GL_FALSE) {
		this->use();
		glUniformMatrix3fv(glGetUniformLocation(this->programID, name), 1, transpose, glm::value_ptr(value));
		this->unuse();
	}

	void sendMat4fv(const GLchar* name, glm::mat4 value, GLboolean transpose = GL_FALSE) {
		this->use();
		glUniformMatrix4fv(glGetUniformLocation(this->programID, name), 1, transpose, glm::value_ptr(value));
		this->unuse();
	}



	void use() {
		glUseProgram(this->programID);
	}

	void unuse() {
		glUseProgram(0);
	}



	~Program() {
		glDeleteProgram(this->programID);
	}
};