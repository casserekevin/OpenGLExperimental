#include <memory>
#include <windows.h>

#include "Window.h"
#include "MyScene.h"

int main(int argc, char** argv) {
	Window w(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), "Introducao OpenGL");
	w.setScene(std::unique_ptr<Scene>(new MyScene(w.getWidth(), w.getHeight())));

	while (!glfwWindowShouldClose(w.getWindow()))
	{
		w.update();
	}
	return 0;
}