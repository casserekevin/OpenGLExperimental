#pragma once

#include <vector>
#include <string>

#include "Face.h"
using std::vector;
class Group {
private:
	vector<Face*> m_faces;
	std::string m_nome;


public:
	Group() {}

	void addFace(Face* face) {
		m_faces.push_back(face);
	}

	void setNome(std::string nome) {
		m_nome = nome;
	}
};
