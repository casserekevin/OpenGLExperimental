#pragma once
#include <GLEW/glew.h>

class VertexBuffer {
private:
	unsigned int m_RendererID;
public:
	VertexBuffer() {}
	VertexBuffer(unsigned int size, const void* data) {
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		Unbind();
	}
	~VertexBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}

	void Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	void Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};