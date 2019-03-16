#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Include para criação do smart pointer
#include <memory>


//como se fosse um include: boas práticas
class Scene;

class Window{
private:
	GLFWwindow* m_Window;
	//Declaração de um smart pointer(ponteiro único: garante que apenas um ponteiro apontará para o objeto Scene) começando com null
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

