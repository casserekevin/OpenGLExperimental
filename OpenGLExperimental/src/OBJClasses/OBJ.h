#pragma once

#include "Mesh.h"

class OBJ {
private:
	Mesh* m_mesh;
public:
	OBJ() {}

	void setMesh(Mesh* mesh) {
		m_mesh = mesh;
	}
	Mesh* getMesh() {
		return m_mesh;
	}
};
