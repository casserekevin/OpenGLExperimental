#include <string>

#include "../src/OBJClasses/IO/ConfigurationIO.h"

int main() {
	ConfigurationIO* configurationIO = new ConfigurationIO();
	configurationIO->replace("teste.txt", "s", "y");


	return 0;
}