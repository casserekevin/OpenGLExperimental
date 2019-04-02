#pragma once



#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
//#include <stdio.h>

#include "../OBJ.h"

using std::string;
class OBJReader {
private:

	std::vector<std::string> split(const std::string& s, char delimiter)
	{
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(s);
		while (std::getline(tokenStream, token, delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}

public:
	OBJReader(){}

	OBJ* loadOBJ(string filepath) {
		try {
			Mesh* mesh = new Mesh();
			std::ifstream file;
			file.exceptions(std::ifstream::badbit);
			file.open(filepath);
			while (!file.eof()){
				string s_line;
				std::getline(file, s_line);
				std::stringstream ss_line;
				ss_line << s_line;
				string s_temp;
				ss_line >> s_temp;
				
				if (s_temp == "v") {
					float x, y, z;
					ss_line >> x >> y >> z;

					mesh->addVertice(x, y, z);
				}
				else if (s_temp == "vt") {

				}
				else if (s_temp == "vn") {
					float x, y, z;
					ss_line >> x >> y >> z;

					mesh->addNormal(x, y, z);
				}
				else if (s_temp == "g") {
					string s_group;
					ss_line >> s_group;

					Group* grupo = new Group();
					grupo->setNome(s_group);
					mesh->addGroup(grupo);
				}
				else if (s_temp == "f") {
					string vertice1, vertice2, vertice3;
					ss_line >> vertice1 >> vertice2 >> vertice3;

					std::vector<string> s_vertice1 = split(vertice1, '/');

					if (s_vertice1.size() == 1) { // v

					}
					else if (s_vertice1.size() == 2) { // v/t
						
					}
					else if (s_vertice1.size() == 3) { // v//n ou v/t/n
						if (s_vertice1.at(1) == "") { // v//n
							Group* group = mesh->getLastGroup();

							Face* face = new Face();
							face->addIndexVertice(std::stoi(s_vertice1.at(0)));
							face->addIndexNormal(std::stoi(s_vertice1.at(2)));
							
							std::vector<string> s_vertice2 = split(vertice2, '/');
							face->addIndexVertice(std::stoi(s_vertice2.at(0)));
							face->addIndexNormal(std::stoi(s_vertice2.at(2)));

							std::vector<string> s_vertice3 = split(vertice3, '/');
							face->addIndexVertice(std::stoi(s_vertice3.at(0)));
							face->addIndexNormal(std::stoi(s_vertice3.at(2)));

							group->addFace(face);
						}
						else { // v/t/n

						}
					}
				}
			}
			OBJ* obj = new OBJ();
			obj->setMesh(mesh);
			return obj;
		}
		catch (std::exception e) {
			std::cerr << "[ERRO] - " << filepath << ": " << e.what() << std::endl;
			return nullptr;
		}
	}
	
	bool CloadOBJ(const char* filepath) {
		FILE* file = fopen(filepath, "r");
		if (file == NULL) {
			printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
			getchar();
			return false;
		}

		while (true) {
			char lineHeader[128];
			// read the first word of the line
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.

			// else : parse lineHeader

			if (strcmp(lineHeader, "v") == 0) {
				float f_x, f_y, f_z;
				fscanf(file, "%f %f %f\n", &f_x, &f_y, &f_z);
				std::cout << f_x << ", " << f_y << ", " << f_z << std::endl;
			}
		}
	}
};
