#pragma once


#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "../OBJ.h"
#include "../Mesh.h"

using std::string;
class MeshReader {
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
	MeshReader(){}

	Mesh* loadMesh(string filepath) {
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
					float x, y;
					ss_line >> x >> y;

					mesh->addTexture(x, y);
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
							face->addIndexVertice(std::stoi(s_vertice1.at(0)) - 1);
							face->addIndexNormal(std::stoi(s_vertice1.at(2)) - 1);
							
							std::vector<string> s_vertice2 = split(vertice2, '/');
							face->addIndexVertice(std::stoi(s_vertice2.at(0)) - 1);
							face->addIndexNormal(std::stoi(s_vertice2.at(2)) - 1);

							std::vector<string> s_vertice3 = split(vertice3, '/');
							face->addIndexVertice(std::stoi(s_vertice3.at(0)) - 1);
							face->addIndexNormal(std::stoi(s_vertice3.at(2)) - 1);

							group->addFace(face);
						}
						else { // v/t/n
							Group* group = mesh->getLastGroup();

							Face* face = new Face();
							face->addIndexVertice(std::stoi(s_vertice1.at(0)) - 1);
							face->addIndexTexture(std::stoi(s_vertice1.at(1)) - 1);
							face->addIndexNormal(std::stoi(s_vertice1.at(2)) - 1);

							std::vector<string> s_vertice2 = split(vertice2, '/');
							face->addIndexVertice(std::stoi(s_vertice2.at(0)) - 1);
							face->addIndexTexture(std::stoi(s_vertice2.at(1)) - 1);
							face->addIndexNormal(std::stoi(s_vertice2.at(2)) - 1);

							std::vector<string> s_vertice3 = split(vertice3, '/');
							face->addIndexVertice(std::stoi(s_vertice3.at(0)) - 1);
							face->addIndexTexture(std::stoi(s_vertice3.at(1)) - 1);
							face->addIndexNormal(std::stoi(s_vertice3.at(2)) - 1);

							group->addFace(face);
						}
					}
				}
			}
			return mesh;
		}
		catch (std::exception e) {
			std::cerr << "[ERRO] - " << filepath << ": " << e.what() << std::endl;
			return nullptr;
		}
	}
};
