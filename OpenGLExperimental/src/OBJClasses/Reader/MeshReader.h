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
					string s_pathFileTexture;
					ss_line >> s_pathFileTexture;

					Group* grupo = new Group(mesh);
					grupo->setPathFileTexture(s_pathFileTexture);
					mesh->addGroup(grupo);
				}
				else if (s_temp == "f") {
					std::string content;
					std::getline(ss_line, content);
					std::vector<std::string> vec_content = split(content, ' ');
					if (vec_content.size() == 5) { //faces quadradas
						std::string vertice1, vertice2, vertice3, vertice4;
						vertice1 = vec_content.at(1);
						vertice2 = vec_content.at(2);
						vertice3 = vec_content.at(3);
						vertice4 = vec_content.at(4);

						std::vector<string> s_vertice1 = split(vertice1, '/');

						if (s_vertice1.size() == 1) { // v

						}
						else if (s_vertice1.size() == 2) { // v/t
							Group* group = mesh->getLastGroup();

							Face* face1 = new Face();
							face1->addIndexPosition(std::stoi(s_vertice1.at(0)) - 1);
							face1->addIndexTexture(std::stoi(s_vertice1.at(1)) - 1);

							std::vector<string> s_vertice2 = split(vertice2, '/');
							face1->addIndexPosition(std::stoi(s_vertice2.at(0)) - 1);
							face1->addIndexTexture(std::stoi(s_vertice2.at(1)) - 1);

							std::vector<string> s_vertice3 = split(vertice3, '/');
							face1->addIndexPosition(std::stoi(s_vertice3.at(0)) - 1);
							face1->addIndexTexture(std::stoi(s_vertice3.at(1)) - 1);

							group->addFace(face1);

							Face* face2 = new Face();
							face2->addIndexPosition(std::stoi(s_vertice1.at(0)) - 1);
							face2->addIndexTexture(std::stoi(s_vertice1.at(1)) - 1);

							face2->addIndexPosition(std::stoi(s_vertice3.at(0)) - 1);
							face2->addIndexTexture(std::stoi(s_vertice3.at(1)) - 1);

							std::vector<string> s_vertice4 = split(vertice4, '/');
							face2->addIndexPosition(std::stoi(s_vertice4.at(0)) - 1);
							face2->addIndexTexture(std::stoi(s_vertice4.at(1)) - 1);

							group->addFace(face2);
						}
						else if (s_vertice1.size() == 3) { // v//n ou v/t/n
							if (s_vertice1.at(1) == "") { // v//n

							}
							else {

							}
						}
					}
					else if (vec_content.size() == 4) { // faces triangulares
						std::string vertice1, vertice2, vertice3;
						vertice1 = vec_content.at(1);
						vertice2 = vec_content.at(2);
						vertice3 = vec_content.at(3);

						std::vector<string> s_vertice1 = split(vertice1, '/');

						if (s_vertice1.size() == 1) { // v

						}
						else if (s_vertice1.size() == 2) { // v/t
							Group* group = mesh->getLastGroup();

							Face* face = new Face();
							face->addIndexPosition(std::stoi(s_vertice1.at(0)) - 1); 
							face->addIndexTexture(std::stoi(s_vertice1.at(1)) - 1);

							std::vector<string> s_vertice2 = split(vertice2, '/');
							face->addIndexPosition(std::stoi(s_vertice2.at(0)) - 1);
							face->addIndexTexture(std::stoi(s_vertice2.at(1)) - 1);

							std::vector<string> s_vertice3 = split(vertice2, '/');
							face->addIndexPosition(std::stoi(s_vertice3.at(0)) - 1);
							face->addIndexTexture(std::stoi(s_vertice3.at(1)) - 1);

							group->addFace(face);
						}
						else if (s_vertice1.size() == 3) { // v//n ou v/t/n
							if (s_vertice1.at(1) == "") { // v//n
								Group* group = mesh->getLastGroup();

								Face* face = new Face();
								face->addIndexPosition(std::stoi(s_vertice1.at(0)) - 1);
								face->addIndexNormal(std::stoi(s_vertice1.at(2)) - 1);

								std::vector<string> s_vertice2 = split(vertice2, '/');
								face->addIndexPosition(std::stoi(s_vertice2.at(0)) - 1);
								face->addIndexNormal(std::stoi(s_vertice2.at(2)) - 1);

								std::vector<string> s_vertice3 = split(vertice3, '/');
								face->addIndexPosition(std::stoi(s_vertice3.at(0)) - 1);
								face->addIndexNormal(std::stoi(s_vertice3.at(2)) - 1);

								group->addFace(face);
							}
							else { // v/t/n
								Group* group = mesh->getLastGroup();

								Face* face = new Face();
								face->addIndexPosition(std::stoi(s_vertice1.at(0)) - 1);
								face->addIndexTexture(std::stoi(s_vertice1.at(1)) - 1);
								face->addIndexNormal(std::stoi(s_vertice1.at(2)) - 1);

								std::vector<string> s_vertice2 = split(vertice2, '/');
								face->addIndexPosition(std::stoi(s_vertice2.at(0)) - 1);
								face->addIndexTexture(std::stoi(s_vertice2.at(1)) - 1);
								face->addIndexNormal(std::stoi(s_vertice2.at(2)) - 1);

								std::vector<string> s_vertice3 = split(vertice3, '/');
								face->addIndexPosition(std::stoi(s_vertice3.at(0)) - 1);
								face->addIndexTexture(std::stoi(s_vertice3.at(1)) - 1);
								face->addIndexNormal(std::stoi(s_vertice3.at(2)) - 1);

								group->addFace(face);
							}
						}
					}
					
				}
			}
			for (Group* g : mesh->getGroups()) {
				g->createVAOandTexture();
			}
			return mesh;
		}
		catch (std::exception e) {
			std::cerr << "[ERRO] - " << filepath << ": " << e.what() << std::endl;
			return nullptr;
		}
	}
};
