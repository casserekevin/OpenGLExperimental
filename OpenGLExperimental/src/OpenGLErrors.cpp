#include <GL/glew.h>

#include "OpenGLErrors.h"



OpenGLErrors::OpenGLErrors(){
}

void OpenGLErrors::clearErrors() {
	while (glGetError() != GL_NO_ERROR);
}

OpenGLErrors::~OpenGLErrors(){
}
