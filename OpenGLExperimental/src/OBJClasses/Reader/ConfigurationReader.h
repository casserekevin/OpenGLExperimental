#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "../../Configuration.h"

class ConfigurationReader {
private:

public:
	ConfigurationReader() {}

	Configuration* read(std::string filepath) {
		try {
			Configuration* configuration = new Configuration();

			std::ifstream file(filepath);
			file.exceptions(std::ifstream::badbit);

			while (!file.eof()) {
				std::string line;
				std::getline(file, line);

				std::stringstream ss_line(line);

				std::string command;
				ss_line >> command;

				if (command == "v") { //viewport data
					int width, height;

					ss_line >> width >> height;

					configuration->addViewport(width, height);
				}
				else if (command == "c") {
					float camPosX, camPosY, camPosZ, camFrontX, camFrontY, camFrontZ, camUpX, camUpY, camUpZ;

					ss_line >> camPosX >> camPosY >> camPosZ >> camFrontX >> camFrontY >> camFrontZ >> camUpX >> camUpY >> camUpZ;
					
					configuration->addCamera(glm::vec3(camPosX, camPosY, camPosZ), glm::vec3(camFrontX, camFrontY, camFrontZ), glm::vec3(camUpX, camUpY, camUpZ));
				}

				else if (command == "o") {
					std::string filePathOBJ;
					float posX, posY, posZ, rotX, rotY, rotZ, scale;

					ss_line >> filePathOBJ >> posX >> posY >> posZ >> rotX >> rotY >> rotZ >> scale;

					configuration->addOBJ(filePathOBJ, glm::vec3(posX, posY, posZ), glm::vec3(rotX, rotY, rotZ), glm::vec3(scale));
				}
			}
			file.close();


			return configuration;
		}
		catch (std::exception e) {
			std::cerr << "[ERRO] - " << filepath << ": " << e.what() << std::endl;
			return nullptr;
		}
	}



	~ConfigurationReader() {}
};
