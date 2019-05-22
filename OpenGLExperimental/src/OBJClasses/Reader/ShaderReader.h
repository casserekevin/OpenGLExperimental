#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include "../../Util/StringUtil.h"

class ShaderReader {
private:
	const std::string SHADER_DEFAULT_FILEPATH = "res/shaders/";

public:
	ShaderReader() {}

	bool read(std::string filepath, std::stringstream& result) {
		{
			try {
				std::ifstream file(StringUtil::concatenarString(this->SHADER_DEFAULT_FILEPATH, filepath));
				file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

				result << file.rdbuf();

				file.close();
				return true;
			}
			catch (std::exception e) {
				std::cerr << "[ERRO] - SHADER_READER - " << filepath << ": " << e.what() << std::endl;
				return false;
			}
		}
	}



	~ShaderReader() {}



};

