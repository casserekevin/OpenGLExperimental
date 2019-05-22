#pragma once

#include <string>
#include <sstream>

class StringUtil {
private:

public:
	StringUtil() {}



	static std::string concatenarString(std::string str1, std::string str2) {
		std::stringstream ss;
		ss << str1 << str2;

		return ss.str();
	}



	~StringUtil(){}
};