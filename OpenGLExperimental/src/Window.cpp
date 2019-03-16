#include <iostream>

#include "Window.h"
#include "Scene.h"

Window::Window(int width, int height, const char* title) {

	// Inicializa a biblioteca GLFW
	if (!glfwInit()) {
		std::cerr << "Erro ao inicializar GLFW" << std::endl;
		return;
	}

	//Definicao dos atributos do contexto OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GL_FALSE);

	//Cria a janela
	m_Window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!m_Window) {
		std::cerr << "Erro ao criar janela" << std::endl;
		return;
	}

	//Tornar o contexto atual
	glfwMakeContextCurrent(m_Window);
	glewExperimental = GL_TRUE;
	//Inicializa a biblioteca GLEW
	GLenum glewInitErr = glewInit();
	if (glewInitErr != GLEW_OK) {
		std::cerr << "Erro ao inicializar GLEW: " << glewGetErrorString(glewInitErr) << std::endl;
		return;
	}

	//Mostra dados do ambiente
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "Renderizador: " << renderer << std::endl;
	std::cout << "OpenGL (Versao suportada): " << version << std::endl;
	
	//Callbacks:
	//Error callback
	glfwSetErrorCallback(Window::errorCallback);
	//Resize Window callback
	glfwSetFramebufferSizeCallback(m_Window, Window::frameBufferSizeCallback);

	
}

GLFWwindow* Window::getWindow() {
	return m_Window;
}

void Window::update() {

	glfwPollEvents();

	if (m_scene) {
		m_scene->update();
	}

	glfwSwapBuffers(m_Window);
}

//callbacks
void Window::errorCallback(int error, const char* description) {
	std::cerr << "Erro " << error << ": " << description << std::endl;
}

void Window::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

Window::~Window() {
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

