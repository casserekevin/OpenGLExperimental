#pragma once
#include <GLEW/glew.h>

class VertexBuffer {
private:
	GLuint vertexBufferID;
public:
	VertexBuffer(unsigned int size, const void* data) {
		glGenBuffers(1, &this->vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}



	void Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID);
	}
	void Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}



	~VertexBuffer() {
		glDeleteBuffers(1, &this->vertexBufferID);
	}
};