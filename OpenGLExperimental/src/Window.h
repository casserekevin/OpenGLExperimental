#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

//Include para cria��o do smart pointer
#include <memory>

#include "Scene.h"

class Window{
private:
	int m_width;
	int m_height;

	GLFWwindow* m_Window;
	//Declara��o de um smart pointer(ponteiro �nico: garante que apenas um ponteiro apontar� para o objeto Scene) come�ando com null
	std::unique_ptr<Scene> m_scene = nullptr;

	//Inline callbacks
	inline static void errorCallback(int error, const char* description) {
		std::cerr << "Erro " << error << ": " << description << std::endl;
	}
	inline static void frameBufferSizeCallback(GLFWwindow* win, int width, int height) {
		Window *window = static_cast<Window*>(glfwGetWindowUserPointer(win));
		window->didResize(width, height);
	}
	void didResize(int w, int h);
	inline static void mouseMovementCallback(GLFWwindow* win, double xpos, double ypos) {
		Window *window = static_cast<Window*>(glfwGetWindowUserPointer(win));
		window->mouseMovement(xpos, ypos);
	}
	void mouseMovement(double xpos, double ypos);
	inline static void mouseZoomCallback(GLFWwindow* win, double xoffset, double yoffset) {
		Window *window = static_cast<Window*>(glfwGetWindowUserPointer(win));
		window->mouseZoom(xoffset, yoffset);
	}
	void mouseZoom(double xoffset, double yoffset);



public:
	Window(int width = 800, int height = 600, const char* title = "");
	
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

	void update();

	~Window();

};

