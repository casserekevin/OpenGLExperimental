#pragma once
#include <GLEW/glew.h>

#include <iostream>

//MACRO DE DEBUG
#define ASSERT(x) if(!(x)) __debugbreak();
#define DEBUG_ERROR(x) clearErrors();\
	x;\
	ASSERT(logErrors(#x, __FILE__, __LINE__))

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



