#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "../../Util/StringUtil.h"

#include "../../Configuration.h"

class ConfigurationIO {
private:
	const std::string CFG_DEFAULT_FILEPATH = "res/cfg/";

public:
	ConfigurationIO() {}

	Configuration* read(std::string filepath) {
		try {
			Configuration* configuration = new Configuration();
			
			std::ifstream file(StringUtil::concatenarString(this->CFG_DEFAULT_FILEPATH, filepath));
			file.exceptions(std::ifstream::badbit);

			while (!file.eof()) {
				std::string line;
				std::getline(file, line);

				std::stringstream ss_line(line);

				std::string command;
				ss_line >> command;

				if (command == "s") {
					std::string stop;

					ss_line >> stop;

					configuration->addStop(stop);
					if (configuration->getStop() == true) {
						break;
					}
				}
				else if (command == "r") {
					std::string render;

					ss_line >> render;

					configuration->addRender(render);
				}
				else if	(command == "v") { //viewport data
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
			std::cerr << "[ERRO] - CONFIGURATION_IO - READ" << filepath << ": " << e.what() << std::endl;
			return nullptr;
		}
	}

	void replace(std::string filepath, std::string searched_command, std::string word) {
		bool equal = false;
		try {
			std::ifstream file_IN(StringUtil::concatenarString(this->CFG_DEFAULT_FILEPATH, filepath), std::ifstream::in);
			file_IN.exceptions(std::ifstream::badbit);
			std::ofstream file_OUT(StringUtil::concatenarString(this->CFG_DEFAULT_FILEPATH, "temp.cfg"), std::ifstream::out);
			file_OUT.exceptions(std::ofstream::badbit);

			while (!file_IN.eof()) {
				std::string line;
				std::getline(file_IN, line);

				std::stringstream ss_line(line);

				std::string command;
				ss_line >> command;

				if (command == searched_command) {
					ss_line >> command;
					if (command == word) {
						equal = true;
						break;
					}
					else {
						file_OUT << searched_command << " " << word << std::endl;
					}
				}
				else {
					if (!file_IN.eof()) {
						file_OUT << line << std::endl;
					}
				}
			}


			file_IN.close();
			file_OUT.close();

			if (equal) {
				remove(StringUtil::concatenarString(this->CFG_DEFAULT_FILEPATH, "temp.cfg").c_str());
			}
			else {
				remove(StringUtil::concatenarString(this->CFG_DEFAULT_FILEPATH, filepath).c_str());
				rename(StringUtil::concatenarString(this->CFG_DEFAULT_FILEPATH, "temp.cfg").c_str(), StringUtil::concatenarString(this->CFG_DEFAULT_FILEPATH, filepath).c_str());
			}
		}
		catch (std::exception e) {
			std::cerr << "[ERRO] - CONFIGURATION_IO - REPLACE" << filepath << ": " << e.what() << std::endl;
		}
	}



	~ConfigurationIO() {}
};
