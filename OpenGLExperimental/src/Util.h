#ifndef _UTIL_H
#define _UTIL_H

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>

bool loadFile(const char* filename, std::stringstream& result) {
	try{
		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file.open(filename);

		result << file.rdbuf();

		file.close();
		
		return true;
	}
	catch (std::exception e){
		std::cerr << "[ERRO] - " << filename << ": " << e.what() << std::endl;
	}
	return false;
}

#endif