#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Include para cria��o do smart pointer
#include <memory>


//como se fosse um include: boas pr�ticas
class Scene;

class Window{
private:
	GLFWwindow* m_Window;
	//Declara��o de um smart pointer(ponteiro �nico: garante que apenas um ponteiro apontar� para o objeto Scene) come�ando com null
	std::unique_ptr<Scene> m_scene = nullptr;


	static void errorCallback(int error, const char* description);

	static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

public:
	Window(int width = 800, int height = 600, const char* title = "");
	
	//getters
	GLFWwindow* getWindow();
	
	//setters
	inline void setScene(std::unique_ptr<Scene> scene) {
		m_scene = std::move(scene);
	}

	void update();

	~Window();

};

