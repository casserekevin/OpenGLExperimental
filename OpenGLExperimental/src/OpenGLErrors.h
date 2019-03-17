#pragma once

#include <iostream>

void clearErrors() {
	while (glGetError() != GL_NO_ERROR);
}

void checkErrors() {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (0x" << std::hex << error << std::dec << ")" << std::endl;
	}
}



