#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

//Include para criação do smart pointer
#include <memory>
#include <iostream>

#include "Scene.h"

class Window {
private:
	int width;
	int height;

	GLFWwindow* window;
	//Declaração de um smart pointer(ponteiro único: garante que apenas um ponteiro apontará para o objeto Scene) começando com null
	std::unique_ptr<Scene> scene = nullptr;

	//Inline callbacks
	inline static void errorCallback(int error, const char* description) {
		std::cerr << "Erro " << error << ": " << description << std::endl;
	}
	inline static void frameBufferSizeCallback(GLFWwindow* win, int width, int height) {
		Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
		window->frameBufferSize(width, height);
	}
	void frameBufferSize(int w, int h) {
		glViewport(0, 0, w, h);
		this->scene->processFrameBufferSize(w, h);
	}
	inline static void mouseMovementCallback(GLFWwindow* win, double xpos, double ypos) {
		Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
		window->mouseMovement(xpos, ypos);
	}
	void mouseMovement(double xpos, double ypos) {
		this->scene->processMouseMovementInput(xpos, ypos);
	}
	inline static void mouseZoomCallback(GLFWwindow* win, double xoffset, double yoffset) {
		Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
		window->mouseZoom(xoffset, yoffset);
	}
	void mouseZoom(double xoffset, double yoffset) {
		this->scene->processMouseZoomInput(xoffset, yoffset);
	}
	inline static void keyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
		Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
		window->keyboard(key, scancode, action, mods);
	}
	void keyboard(int key, int scancode, int action, int mods) {
		this->scene->processKeyboardInput(key, scancode, action, mods);
	}



public:

	//CONSTRUCTOR
	Window(int width = 800, int height = 600, const char* title = "") : width(width), height(height) {
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
		this->window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!this->window) {
			std::cerr << "Erro ao criar janela" << std::endl;
			return;
		}

		//Tornar o contexto atual
		glfwMakeContextCurrent(this->window);
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
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glFrontFace(GL_CW);
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		//Necessário para glfwGetUserPointer() funcionar
		glfwSetWindowUserPointer(this->window, this);

		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//Callbacks:
		//Error callback
		glfwSetErrorCallback(Window::errorCallback);
		//Resize Window callback
		glfwSetFramebufferSizeCallback(this->window, frameBufferSizeCallback);
		//Mouse movement callback
		glfwSetCursorPosCallback(this->window, mouseMovementCallback);
		//Mouse scroll callback
		glfwSetScrollCallback(this->window, mouseZoomCallback);
		//Keyboard callback
		glfwSetKeyCallback(this->window, keyboardCallback);
	}

	void update() {
		this->scene->processKeyboard();
		glfwPollEvents();

		if (this->scene) {
			this->scene->update();
		}

		glfwSwapBuffers(this->window);
	}



	~Window() {
		glfwDestroyWindow(this->window);
		glfwTerminate();
	}



	//getters
	inline GLFWwindow* getWindow() { return this->window; }
	inline int getWidth() { return this->width; }
	inline int getHeight() { return this->height; }

	//setters
	void setScene(std::unique_ptr<Scene> scene) { this->scene = std::move(scene); }
};

