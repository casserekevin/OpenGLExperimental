#include <iostream>

#include "Window.h"
#include "Scene.h"



Window::Window(int width, int height, const char* title): m_width(width), m_height(height) {

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
	
	//habilita o corte de faces; corta as faces de tras; define quais são as faces frontais
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	//Necessário para glfwGetUserPointer() funcionar
	glfwSetWindowUserPointer(m_Window, this);

	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Callbacks:
	//Error callback
	glfwSetErrorCallback(Window::errorCallback);
	//Resize Window callback
	glfwSetFramebufferSizeCallback(m_Window, frameBufferSizeCallback);
	//Mouse movement callback
	glfwSetCursorPosCallback(m_Window, mouseMovementCallback);
	//Mouse scroll callback
	glfwSetScrollCallback(m_Window, mouseZoomCallback);
}

void Window::update() {

	m_scene->processKeyboardInput();
	glfwPollEvents();

	if (m_scene) {
		m_scene->update();
	}

	glfwSwapBuffers(m_Window);
}

//callbacks implementations
void Window::didResize(int w, int h) {
	glViewport(0, 0, w, h);
	m_scene->setWidth(w);
	m_scene->setHeight(h);
	m_scene->setNewResizeProjectionMatrix();
}
void Window::mouseMovement(double xpos, double ypos) {
	m_scene->processMouseMovementInput(xpos, ypos);
}

void Window::mouseZoom(double xoffset, double yoffset) {
	m_scene->processMouseZoomInput(xoffset, yoffset);
}

Window::~Window() {
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

