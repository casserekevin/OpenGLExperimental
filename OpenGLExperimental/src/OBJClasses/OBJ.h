#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "../Program.h"

class OBJ {
private:
	Mesh* m_mesh = nullptr;
	Program* program = nullptr;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 modelMatrix;

	//functions
	void initModelMatrix() {
		
		this->position = glm::vec3(1.f, 0.f, 0.f);
		this->rotation = glm::vec3(0.f);
		this->scale = glm::vec3(1.f);

		this->modelMatrix = glm::mat4(1.f);
		this->modelMatrix = glm::translate(this->modelMatrix, this->position);
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->modelMatrix = glm::scale(this->modelMatrix, this->scale);
	}
	
public:

	OBJ(Mesh* mesh, Program* program) {
		this->m_mesh = mesh;
		this->m_mesh->setObjThatIsInserted(this);

		this->program = program;

		initModelMatrix();
	}

	void setMesh(Mesh* mesh) {
		m_mesh = mesh;
	}
	Mesh* getMesh() {
		return m_mesh;
	}

	glm::mat4 getModelMatrix() {
		return modelMatrix;
	}

	GLint getFirstTextureUnit() {
		return m_mesh->getFirstTextureUnit();
	}

	int getFirstTypeDraw() {
		return m_mesh->getFirstTypeDraw();
	}

	void draw() {
		m_mesh->draw(this->program);
		program->sendMat4fv("modelMatrix", this->modelMatrix);
	}
};