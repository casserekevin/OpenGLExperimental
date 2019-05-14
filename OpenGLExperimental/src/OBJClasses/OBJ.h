#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "../Program.h"
#include "../OBJData.h"

class OBJ {
private:
	Mesh* mesh = nullptr;
	Program* program = nullptr;



	glm::mat4 modelMatrix;
	glm::vec3 color = glm::vec3(1.f, 0.f, 0.f);
	
	bool selected;
	//PRIVATE FUNCTIONS
	void addColor() {
		this->mesh->applyColor();
	}
	void removeColor() {
		this->mesh->disapplyColor();
	}
public:

	OBJ(Mesh* mesh, OBJData* objData, Program* program): mesh(mesh), program(program) {
		this->mesh->setObjThatIsInserted(this);
		this->selected = false;

		this->modelMatrix = glm::translate(glm::mat4(1.f), objData->getPosition());
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(objData->getRotation().x), glm::vec3(1.f, 0.f, 0.f));
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(objData->getRotation().y), glm::vec3(0.f, 1.f, 0.f));
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(objData->getRotation().z), glm::vec3(0.f, 0.f, 1.f));
		this->modelMatrix = glm::scale(this->modelMatrix, objData->getScale());
	}

	void draw() {
		this->program->sendMat4fv("modelMatrix", this->modelMatrix);
		this->program->sendVec3fv("color", this->color);
		this->mesh->draw(this->program);
	}

	void translateXpositive() {
		this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(0.2f, 0.f, 0.f));
	}
	void translateXnegative() {
		this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(-0.2f, 0.f, 0.f));
	}
	void translateYpositive() {
		this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(0.f, 0.2f, 0.f));
	}
	void translateYnegative() {
		this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(0.f, -0.2f, 0.f));
	}
	void translateZpositive() {
		this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(0.f, 0.f, 0.2f));
	}
	void translateZnegative() {
		this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(0.f, 0.f, -0.2f));
	}
	void rotationYpositive() {
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(2.f), glm::vec3(0.f, 1.f, 0.f));
	}
	void rotationYnegative() {
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(-2.f), glm::vec3(0.f, 1.f, 0.f));
	}
	void scalePlus() {
		if (this->modelMatrix[0][0] < 10.f) {
			this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(1.05f));
		}
	}
	void scaleMinus() {
		if (this->modelMatrix[0][0] > 0.01f) {
			this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(0.95f));
		}
	}



	void unselect() {
		if (isSelected()) {
			removeColor();
			this->selected = false;
		}
	}

	void changeSelection() {
		if (isSelected()) {
			removeColor();
			this->selected = false;
		}
		else {
			addColor();
			this->selected = true;
		}
	}



	~OBJ(){}



	//GETTERS
	inline bool isSelected() { return this->selected; }

	//SETTERS
};