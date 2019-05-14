#pragma once

#include <sstream>
#include <iostream>
#include <fstream>

class ShaderReader {
private:

public:
	ShaderReader() {}

	bool read(const char* filepath, std::stringstream& result) {
		{
			try {
				std::ifstream file(filepath);
				file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

				result << file.rdbuf();

				file.close();
				return true;
			}
			catch (std::exception e) {
				std::cerr << "[ERRO] - " << filepath << ": " << e.what() << std::endl;
				return false;
			}
		}
	}



	~ShaderReader() {}



};

