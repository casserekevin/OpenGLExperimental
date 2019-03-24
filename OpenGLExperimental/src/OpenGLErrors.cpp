#include <GLEW/glew.h>

#include <iostream>

#include "OpenGLErrors.h"

void clearErrors() {
	while (glGetError() != GL_NO_ERROR);
}

bool logErrors(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] COD(0x" << std::hex << error << std::dec << ")  Function:" << function << "  Pathfile:" << file << "  Line:" << line << std::endl;
		return false;
	}
	return true;
}