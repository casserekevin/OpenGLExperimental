#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <map>

#include "../../Util/StringUtil.h"

#include "../../Material.h"

class MaterialReader {
private:
	const std::string MTL_DEFAULT_FILEPATH = "res/obj/mtl/";

public:
	MaterialReader() {}



	std::map<std::string, Material* >* read(std::string filepath) {
		try {
			std::map<std::string, Material* >* map_materials = new std::map<std::string, Material* >;

			Material* material = nullptr;

			std::ifstream file(StringUtil::concatenarString(this->MTL_DEFAULT_FILEPATH, filepath));
			file.exceptions(std::ifstream::badbit);

			while (!file.eof()) {
				std::string line;
				std::getline(file, line);

				std::stringstream ss_line(line);

				std::string command;
				ss_line >> command;

				if (command == "newmtl") {
					std::string materialName;
					ss_line >> materialName;

					material = new Material();

					map_materials->insert(std::pair<std::string, Material* >(materialName, material));
				}
				else if (command == "Ka") {
					float x, y, z;
					ss_line >> x >> y >> z;

					material->addKa(x, y, z);
				}
				else if (command == "Kd") {
					float x, y, z;
					ss_line >> x >> y >> z;

					material->addKd(x, y, z);
				}
				else if (command == "Ks") {
					float x, y, z;
					ss_line >> x >> y >> z;

					material->addKs(x, y, z);
				}
				else if (command == "Ns") {
					float value;
					ss_line >> value;

					material->addNs(value);
				}
				else if (command == "map_Kd") {
					std::string textureFilepath;
					ss_line >> textureFilepath;

					material->addTexture(textureFilepath);
				}
			}
			file.close();

			return map_materials;
		}
		catch (std::exception e) {
			std::cerr << "[ERRO] - MATERIAL_READER - " << filepath << ": " << e.what() << std::endl;
			return nullptr;
		}
	}



	~MaterialReader() {}
};
