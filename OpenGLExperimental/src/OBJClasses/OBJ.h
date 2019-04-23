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

	glm::vec3 color = glm::vec3(1.f, 0.f, 0.f);

	bool selected;

	void calculateModelMatrix() {
		this->modelMatrix = glm::mat4(1.f);
		this->modelMatrix = glm::translate(this->modelMatrix, this->position);
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->modelMatrix = glm::scale(this->modelMatrix, this->scale);
	}
	//functions
	void initModelMatrix() {
		
		this->position = glm::vec3(0.f, 0.f, 0.f);
		this->rotation = glm::vec3(0.f);
		this->scale = glm::vec3(1.f);

		calculateModelMatrix();
	}

	void addColor() {
		this->m_mesh->applyColor();
	}

	void removeColor() {
		this->m_mesh->disapplyColor();
	}
	
public:

	OBJ(Mesh* mesh, Program* program) {
		this->m_mesh = mesh;
		this->m_mesh->setObjThatIsInserted(this);

		this->program = program;
		this->selected = false;


		initModelMatrix();
	}

	void setMesh(Mesh* mesh) {
		m_mesh = mesh;
	}
	Mesh* getMesh() {
		return m_mesh;
	}

	void setPosition(glm::vec3 position) {
		this->position = position;

		calculateModelMatrix();
	}

	void setRotation(glm::vec3 rotation) {
		this->rotation = rotation;

		calculateModelMatrix();
	}

	void setScale(glm::vec3 scale) {
		this->scale = scale;

		calculateModelMatrix();
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

	void select() {
		if (this->selected) {
			this->removeColor();
			this->selected = false;
		}
		else {
			this->addColor();
			this->selected = true;
		}
	}

	

	void draw() {
		program->sendMat4fv("modelMatrix", this->modelMatrix);
		program->sendVec3fv("color", this->color);
		m_mesh->draw(this->program);
	}
};