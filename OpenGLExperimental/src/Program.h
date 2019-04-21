#pragma once

#include <GLEW/glew.h>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>

#include "ShaderReader.h";


class Program {
private:
	unsigned int programID;
	unsigned int vertexShaderID = 0;
	unsigned int fragmentShaderID = 0;
	unsigned int geometryShaderID = 0;

	std::string vertexSource;
	std::string fragmentSource;
	std::string geometrySource = "";

	//-----------------------------------------------
	//constructor functions
	void loadShadersFile(const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath) {
		std::stringstream vertexSourceStream;
		std::stringstream fragmentSourceStream;

		ShaderReader* sh = new ShaderReader();
		sh->read(vertexFilePath, vertexSourceStream);
		sh->read(fragmentFilePath, fragmentSourceStream);

		vertexSource = vertexSourceStream.str();
		fragmentSource = fragmentSourceStream.str();

		//se o geometry shader existir
		if (geometryFilePath != "") {
			std::stringstream geometrySourceStream;

			sh->read(geometryFilePath, geometrySourceStream);

			geometrySource = geometrySourceStream.str();
		}
	}
	void createProgram() {
		programID = glCreateProgram();

		//cria o Vertex Shader
		vertexShaderID = this->createShader(vertexSource, GL_VERTEX_SHADER);
		glAttachShader(programID, vertexShaderID); //linka o shader ao programa
		//cria o Fragment Shader
		fragmentShaderID = this->createShader(fragmentSource, GL_FRAGMENT_SHADER);
		glAttachShader(programID, fragmentShaderID); //linka o shader ao programa
		//cria o Geometry Shader
		if (geometrySource != "") {
			geometryShaderID = this->createShader(geometrySource, GL_GEOMETRY_SHADER);
			glAttachShader(programID, geometryShaderID); //linka o shader ao programa
		}

		//linka o program
		glLinkProgram(programID);
		this->verifyProgramLink(programID); //verifica erros na linkagem do programa

		glUseProgram(0);
	}
	void deleteShaders() {
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		if (geometryShaderID != 0) {
			glDeleteShader(fragmentShaderID);
		}
	}
	//-----------------------------------------------


	//-----------------------------------------------
	//auxiliar functions
	unsigned int createShader(std::string shaderSource, unsigned int type) {
		const char* buff;

		buff = shaderSource.c_str();

		unsigned int shaderID = glCreateShader(type);
		glShaderSource(shaderID, 1, &buff, NULL);
		glCompileShader(shaderID);
		this->verifyShaderCompilation(shaderID);

		return shaderID;
	}
	
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
	//-----------------------------------------------

public:

	Program(const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath = "") {
		this->loadShadersFile(vertexFilePath, fragmentFilePath, geometryFilePath);
		this->createProgram();
		this->deleteShaders();
	}

	~Program() {
		glDeleteProgram(this->programID);
	}

	void use() {
		//usa o program
		glUseProgram(this->programID);
	}

	void unuse() {
		glUseProgram(0);
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
};