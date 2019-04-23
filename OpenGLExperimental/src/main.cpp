#include <memory>
#include <windows.h>

#include "Window.h"
#include "MyScene.h"

#include "Configuration.h"
#include "OBJClasses/Reader/ConfigurationReader.h"

int main(int argc, char** argv) {
	ConfigurationReader* configurationReader = new ConfigurationReader();
	Configuration* configuration = configurationReader->loadConfiguration("res/cfg/configuration.cfg");

	Window w(configuration->getWidthViewport(), configuration->getHeigthViewport(), "Introducao OpenGL");
	w.setScene(std::unique_ptr<Scene>(new MyScene(w.getWidth(), w.getHeight(), configuration)));

	while (!glfwWindowShouldClose(w.getWindow()))
	{
		w.update();
	}
	return 0;
}