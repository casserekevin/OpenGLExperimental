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

	Configuration* loadConfiguration(std::string filepath) {
		try {
			Configuration* configuration = new Configuration();

			std::ifstream file;
			file.exceptions(std::ifstream::badbit);
			file.open(filepath);

			while (!file.eof()) {
				std::string s_line;
				std::getline(file, s_line);
				std::stringstream ss_line;
				ss_line << s_line;
				std::string s_temp;
				ss_line >> s_temp;

				if (s_temp == "v") { //viewport data
					int width, height;

					ss_line >> width >> height;

					configuration->addViewport(width, height);
				}
				else if (s_temp == "c") {
					float camPosX, camPosY, camPosZ, camFrontX, camFrontY, camFrontZ, camUpX, camUpY, camUpZ;

					ss_line >> camPosX >> camPosY >> camPosZ >> camFrontX >> camFrontY >> camFrontZ >> camUpX >> camUpY >> camUpZ;
					
					configuration->addCamera(glm::vec3(camPosX, camPosY, camPosZ), glm::vec3(camFrontX, camFrontY, camFrontZ), glm::vec3(camUpX, camUpY, camUpZ));
				}

				else if (s_temp == "o") {
					std::string filePathOBJ;
					float posX, posY, posZ, rotX, rotY, rotZ, scale;

					ss_line >> filePathOBJ >> posX >> posY >> posZ >> rotX >> rotY >> rotZ >> scale;

					configuration->addOBJ(filePathOBJ, glm::vec3(posX, posY, posZ), glm::vec3(rotX, rotY, rotZ), glm::vec3(scale));
				}
			}

			return configuration;
		}
		catch (std::exception e) {
			std::cerr << "[ERRO] - " << filepath << ": " << e.what() << std::endl;
			return nullptr;
		}
	}
};
