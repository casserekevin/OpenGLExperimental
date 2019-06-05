#include <memory>
#include <windows.h>

#include "Window.h"
#include "Render.h"
#include "Editor.h"

#include "Configuration.h"
#include "OBJClasses/IO/ConfigurationIO.h"

int main(int argc, char** argv) {
	ConfigurationIO* configurationIO = new ConfigurationIO();
	configurationIO->replace("configuration.cfg", "s", "n");
	while (true) {
		Configuration* configuration = configurationIO->read("configuration.cfg");
		if (configuration->getStop()) {
			break;
		}
		Window* w = new Window(configuration->getRender());
		
		if (configuration->getRender()) {
			w->setScene(std::unique_ptr<Scene>(new Render(w->getWindow(), w->getWidth(), w->getHeight(), configuration)));
		}
		else {
			w->setScene(std::unique_ptr<Scene>(new Editor(w->getWindow(), w->getWidth(), w->getHeight(), configuration)));
		}

		while (!glfwWindowShouldClose(w->getWindow()))
		{
			w->update();
		}
		w->~Window();
	}
	return 0;
}