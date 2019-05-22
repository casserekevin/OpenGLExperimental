#pragma once


#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>

#include "../../Util/StringUtil.h"

#include "MaterialReader.h"
#include "../OBJ.h"
#include "../Mesh.h"
#include "../../Material.h"

using std::string;
class MeshReader {
private:
	const std::string OBJ_DEFAULT_FILEPATH = "res/obj/";

	std::map<std::string, Material* >* materials;

	std::vector<std::string> split(const std::string& s, char delimiter){
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

	Mesh* read(string filepath) {
		bool gFirst = false;
		bool usemtlFirst = false;


		Mesh* mesh = new Mesh();	
		Group* grupo = nullptr;
		try {

			std::ifstream file(StringUtil::concatenarString(this->OBJ_DEFAULT_FILEPATH, filepath));
			file.exceptions(std::ifstream::badbit);

			while (!file.eof()){
				string line;
				std::getline(file, line);

				std::stringstream ss_line(line);

				string command;
				ss_line >> command;
				
				if (command == "mtllib") {
					std::string fileMTL;
					ss_line >> fileMTL;

					this->materials = MaterialReader().read(fileMTL);
				}
				else if (command == "v") {
					float x, y, z;
					ss_line >> x >> y >> z;

					mesh->addVertice(x, y, z);
				}
				else if (command == "vt") {
					float x, y;
					ss_line >> x >> y;

					mesh->addTexture(x, y);
				}
				else if (command == "vn") {
					float x, y, z;
					ss_line >> x >> y >> z;

					mesh->addNormal(x, y, z);
				}
				else if (command == "g") {
					std::string groupName;
					ss_line >> groupName;
					if ((gFirst == false && usemtlFirst == false) || gFirst == true) {
						if (gFirst == false && usemtlFirst == false) {
							gFirst = true;
						}
						grupo = new Group(mesh);
						grupo->setGroupName(groupName);
						mesh->addGroup(grupo);
					}
					else {
						grupo->setGroupName(groupName);
					}
				}
				else if (command == "usemtl") {
					std::string materialName;
					ss_line >> materialName;
					if ((gFirst == false && usemtlFirst == false) || usemtlFirst == true) {
						if (gFirst == false && usemtlFirst == false) {
							usemtlFirst = true;
						}
						grupo = new Group(mesh);
						grupo->setMaterial(materials->find(materialName)->second);
						mesh->addGroup(grupo);
					}
					else {
						grupo->setMaterial(materials->find(materialName)->second);
					}
				}
				else if (command == "f") {
					std::string content;
					std::getline(ss_line, content);
					std::vector<std::string> vec_content = split(content, ' ');
					if (vec_content.size() == 5) { //faces quadradas
						std::string vertice1, vertice2, vertice3, vertice4;
						vertice1 = vec_content.at(1);
						vertice2 = vec_content.at(2);
						vertice3 = vec_content.at(3);
						vertice4 = vec_content.at(4);

						std::vector<string> vec_vertice1 = split(vertice1, '/');

						if (vec_vertice1.size() == 1) { // v

						}
						else if (vec_vertice1.size() == 2) { // v/t
							Group* group = mesh->getLastGroup();

							Face* face1 = new Face();
							face1->addIndexPosition(std::stoi(vec_vertice1.at(0)) - 1);
							face1->addIndexTexture(std::stoi(vec_vertice1.at(1)) - 1);

							std::vector<string> vec_vertice2 = split(vertice2, '/');
							face1->addIndexPosition(std::stoi(vec_vertice2.at(0)) - 1);
							face1->addIndexTexture(std::stoi(vec_vertice2.at(1)) - 1);

							std::vector<string> vec_vertice3 = split(vertice3, '/');
							face1->addIndexPosition(std::stoi(vec_vertice3.at(0)) - 1);
							face1->addIndexTexture(std::stoi(vec_vertice3.at(1)) - 1);

							group->addFace(face1);

							Face* face2 = new Face();
							face2->addIndexPosition(std::stoi(vec_vertice1.at(0)) - 1);
							face2->addIndexTexture(std::stoi(vec_vertice1.at(1)) - 1);

							face2->addIndexPosition(std::stoi(vec_vertice3.at(0)) - 1);
							face2->addIndexTexture(std::stoi(vec_vertice3.at(1)) - 1);

							std::vector<string> vec_vertice4 = split(vertice4, '/');
							face2->addIndexPosition(std::stoi(vec_vertice4.at(0)) - 1);
							face2->addIndexTexture(std::stoi(vec_vertice4.at(1)) - 1);

							group->addFace(face2);
						}
						else if (vec_vertice1.size() == 3) { // v//n ou v/t/n
							if (vec_vertice1.at(1) == "") { // v//n

							}
							else { //v/t/n
								Group* group = mesh->getLastGroup();

								Face* face1 = new Face();
								face1->addIndexPosition(std::stoi(vec_vertice1.at(0)) - 1);
								face1->addIndexTexture(std::stoi(vec_vertice1.at(1)) - 1);
								face1->addIndexNormal(std::stoi(vec_vertice1.at(2)) - 1);

								std::vector<string> vec_vertice2 = split(vertice2, '/');
								face1->addIndexPosition(std::stoi(vec_vertice2.at(0)) - 1);
								face1->addIndexTexture(std::stoi(vec_vertice2.at(1)) - 1);
								face1->addIndexNormal(std::stoi(vec_vertice2.at(2)) - 1);

								std::vector<string> vec_vertice3 = split(vertice3, '/');
								face1->addIndexPosition(std::stoi(vec_vertice3.at(0)) - 1);
								face1->addIndexTexture(std::stoi(vec_vertice3.at(1)) - 1);
								face1->addIndexNormal(std::stoi(vec_vertice3.at(2)) - 1);

								group->addFace(face1);

								Face* face2 = new Face();
								face2->addIndexPosition(std::stoi(vec_vertice1.at(0)) - 1);
								face2->addIndexTexture(std::stoi(vec_vertice1.at(1)) - 1);
								face2->addIndexNormal(std::stoi(vec_vertice1.at(2)) - 1);

								face2->addIndexPosition(std::stoi(vec_vertice3.at(0)) - 1);
								face2->addIndexTexture(std::stoi(vec_vertice3.at(1)) - 1);
								face2->addIndexNormal(std::stoi(vec_vertice3.at(2)) - 1);

								std::vector<string> vec_vertice4 = split(vertice4, '/');
								face2->addIndexPosition(std::stoi(vec_vertice4.at(0)) - 1);
								face2->addIndexTexture(std::stoi(vec_vertice4.at(1)) - 1);
								face2->addIndexNormal(std::stoi(vec_vertice4.at(2)) - 1);

								group->addFace(face2);
							}
						}
					}
					else if (vec_content.size() == 4) { // faces triangulares
						std::string vertice1, vertice2, vertice3;
						vertice1 = vec_content.at(1);
						vertice2 = vec_content.at(2);
						vertice3 = vec_content.at(3);

						std::vector<string> vec_vertice1 = split(vertice1, '/');

						if (vec_vertice1.size() == 1) { // v

						}
						else if (vec_vertice1.size() == 2) { // v/t
							Group* group = mesh->getLastGroup();

							Face* face = new Face();
							face->addIndexPosition(std::stoi(vec_vertice1.at(0)) - 1); 
							face->addIndexTexture(std::stoi(vec_vertice1.at(1)) - 1);

							std::vector<string> vec_vertice2 = split(vertice2, '/');
							face->addIndexPosition(std::stoi(vec_vertice2.at(0)) - 1);
							face->addIndexTexture(std::stoi(vec_vertice2.at(1)) - 1);

							std::vector<string> vec_vertice3 = split(vertice3, '/');
							face->addIndexPosition(std::stoi(vec_vertice3.at(0)) - 1);
							face->addIndexTexture(std::stoi(vec_vertice3.at(1)) - 1);

							group->addFace(face);
						}
						else if (vec_vertice1.size() == 3) { // v//n ou v/t/n
							if (vec_vertice1.at(1) == "") { // v//n
								Group* group = mesh->getLastGroup();

								Face* face = new Face();
								face->addIndexPosition(std::stoi(vec_vertice1.at(0)) - 1);
								face->addIndexNormal(std::stoi(vec_vertice1.at(2)) - 1);

								std::vector<string> vec_vertice2 = split(vertice2, '/');
								face->addIndexPosition(std::stoi(vec_vertice2.at(0)) - 1);
								face->addIndexNormal(std::stoi(vec_vertice2.at(2)) - 1);

								std::vector<string> vec_vertice3 = split(vertice3, '/');
								face->addIndexPosition(std::stoi(vec_vertice3.at(0)) - 1);
								face->addIndexNormal(std::stoi(vec_vertice3.at(2)) - 1);

								group->addFace(face);
							}
							else { // v/t/n
								Group* group = mesh->getLastGroup();

								Face* face = new Face();
								face->addIndexPosition(std::stoi(vec_vertice1.at(0)) - 1);
								face->addIndexTexture(std::stoi(vec_vertice1.at(1)) - 1);
								face->addIndexNormal(std::stoi(vec_vertice1.at(2)) - 1);

								std::vector<string> vec_vertice2 = split(vertice2, '/');
								face->addIndexPosition(std::stoi(vec_vertice2.at(0)) - 1);
								face->addIndexTexture(std::stoi(vec_vertice2.at(1)) - 1);
								face->addIndexNormal(std::stoi(vec_vertice2.at(2)) - 1);

								std::vector<string> vec_vertice3 = split(vertice3, '/');
								face->addIndexPosition(std::stoi(vec_vertice3.at(0)) - 1);
								face->addIndexTexture(std::stoi(vec_vertice3.at(1)) - 1);
								face->addIndexNormal(std::stoi(vec_vertice3.at(2)) - 1);

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
			std::cerr << "[ERRO] - MESH_READER - " << filepath << ": " << e.what() << std::endl;
			return nullptr;
		}
	}



	~MeshReader() {}
};
