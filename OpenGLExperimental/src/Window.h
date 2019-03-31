#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

//Include para criação do smart pointer
#include <memory>
#include <iostream>

#include "Scene.h"

class Window{
private:
	int m_width;
	int m_height;

	GLFWwindow* m_Window;
	//Declaração de um smart pointer(ponteiro único: garante que apenas um ponteiro apontará para o objeto Scene) começando com null
	std::unique_ptr<Scene> m_scene = nullptr;

	//Inline callbacks
	inline static void errorCallback(int error, const char* description) {
		std::cerr << "Erro " << error << ": " << description << std::endl;
	}
	inline static void frameBufferSizeCallback(GLFWwindow* win, int width, int height) {
		Window *window = static_cast<Window*>(glfwGetWindowUserPointer(win));
		window->frameBufferSize(width, height);
	}
	void frameBufferSize(int w, int h) {
		glViewport(0, 0, w, h);
		m_scene->processFrameBufferSize(w, h);
	}
	inline static void mouseMovementCallback(GLFWwindow* win, double xpos, double ypos) {
		Window *window = static_cast<Window*>(glfwGetWindowUserPointer(win));
		window->mouseMovement(xpos, ypos);
	}
	void mouseMovement(double xpos, double ypos) {
		m_scene->processMouseMovementInput(xpos, ypos);
	}
	inline static void mouseZoomCallback(GLFWwindow* win, double xoffset, double yoffset) {
		Window *window = static_cast<Window*>(glfwGetWindowUserPointer(win));
		window->mouseZoom(xoffset, yoffset);
	}
	void mouseZoom(double xoffset, double yoffset) {
		m_scene->processMouseZoomInput(xoffset, yoffset);
	}



public:
	Window(int width = 800, int height = 600, const char* title = "") : m_width(width), m_height(height) {
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
	
	//getters
	inline GLFWwindow* getWindow() {
		return m_Window;
	}
	inline int getWidth() {
		return m_width;
	}
	inline int getHeight() {
		return m_height;
	}
	
	//setters
	inline void setScene(std::unique_ptr<Scene> scene) {
		m_scene = std::move(scene);
		m_scene->setWindow(m_Window);
	}

	void update() {
		m_scene->processKeyboardInput();
		glfwPollEvents();

		if (m_scene) {
			m_scene->update();
		}

		glfwSwapBuffers(m_Window);
	}

	~Window() {
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
};

