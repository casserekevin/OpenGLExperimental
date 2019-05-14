#include <memory>
#include <windows.h>

#include "Window.h"
#include "MyScene.h"

#include "Configuration.h"
#include "OBJClasses/Reader/ConfigurationReader.h"

int main(int argc, char** argv) {
	Configuration* configuration = ConfigurationReader().read("res/cfg/configuration.cfg");

	Window* w = new Window(configuration->getWidth(), configuration->getHeigth(), "Introducao OpenGL");
	w->setScene(std::unique_ptr<Scene>(new MyScene(w->getWindow(), w->getWidth(), w->getHeight(), configuration)));

	while (!glfwWindowShouldClose(w->getWindow()))
	{
		w->update();
	}
	return 0;
}