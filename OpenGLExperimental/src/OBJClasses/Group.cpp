#pragma once

#include "Group.h"
#include "Mesh.h"

vector<glm::vec3> Group::generatePositionData() {
	vector<glm::vec3> vector;
	for (Face* face : this->faces) {
		for (int i = 0; i < face->getSizeOfFace(); i++) {
			vector.push_back(*(this->meshThatIsInserted->getPositionAt(face->getIndexPositionAt(i))));
		}
	}
	return vector;
}

vector<glm::vec2> Group::generateTextureData() {
	vector<glm::vec2> vector;
	if (hasTexture()) {
		for (Face* face : this->faces) {
			for (int i = 0; i < face->getSizeOfFace(); i++) {
				vector.push_back(*(this->meshThatIsInserted->getTextureAt(face->getIndexTextureAt(i))));
			}
		}
		typeDraw = 1;
	}
	else {
		for (Face* face : this->faces) {
			for (int i = 0; i < face->getSizeOfFace(); i++) {
				vector.push_back(glm::vec2(-1.f, -1.f));
			}
		}
		typeDraw = 3;
	}
	return vector;
}

vector<glm::vec3> Group::generateNormalData() {
	vector<glm::vec3> vector;
	if (hasNormal()) {
		for (Face* face : this->faces) {
			for (int i = 0; i < face->getSizeOfFace(); i++) {
				vector.push_back(*(this->meshThatIsInserted->getNormalAt(face->getIndexNormalAt(i))));
			}
		}
	}
	else {
		for (Face* face : this->faces) {
			for (int i = 0; i < face->getSizeOfFace(); i++) {
				vector.push_back(glm::vec3(-1.f, -1.f, -1.f));
			}
		}
	}
	return vector;
}



bool Group::hasTexture() {
	return this->meshThatIsInserted->hasTexture();
}
bool Group::hasNormal() {
	return this->meshThatIsInserted->hasNormal();
}

