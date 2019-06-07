#pragma once

#include <GLEW/glew.h>

class VertexArray {
private:
	GLuint vertexArrayID;
public:
	VertexArray() {
		glGenVertexArrays(1, &this->vertexArrayID);
	}

	void bind() {
		glBindVertexArray(this->vertexArrayID);
	}

	void unbind() {
		glBindVertexArray(0);
	}
};