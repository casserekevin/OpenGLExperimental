#pragma once

#include <GLM/glm.hpp>

#include <vector>

class RouteIO {
private:
	const std::string ROUTE_DEFAULT_FILEPATH = "res/routes/";
public:
	RouteIO(){}



	void write(std::vector<glm::vec3*> vector) {
		try {
			std::ofstream file_OUT(StringUtil::concatenarString(this->ROUTE_DEFAULT_FILEPATH, "route.rt"), std::ifstream::out);
			file_OUT.exceptions(std::ofstream::badbit);

			for (int i = 0; i < vector.size(); i++) {
				glm::vec3* vertex = vector.at(i);
				file_OUT << vertex->x << " " << vertex->z << " " << vertex->y << std::endl;
			}

			file_OUT.close();
		}
		catch (std::exception e) {
			std::cerr << "[ERRO] - ROUTE_IO - WRITE" << ": " << e.what() << std::endl;
		}
	}



	~RouteIO(){}
};
