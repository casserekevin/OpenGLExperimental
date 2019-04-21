#pragma once

#include <sstream>
#include <iostream>
#include <fstream>

class ShaderReader {
private:

public:
	ShaderReader(){}

	bool read(const char* filename, std::stringstream& result) {
		{
			try {
				std::ifstream file;
				file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
				file.open(filename);

				result << file.rdbuf();

				file.close();

				return true;
			}
			catch (std::exception e) {
				std::cerr << "[ERRO] - " << filename << ": " << e.what() << std::endl;
			}
			return false;
		}
	}
};

