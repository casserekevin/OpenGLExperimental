#include "Group.h"
#include "Mesh.h"

vector<glm::vec3> Group::generatePositionData() {
	vector<glm::vec3> vector;
	for (Face* face : m_faces) {
		for (int i = 0; i < face->getSizeOfFace(); i++) {
			vector.push_back(*(this->meshThatIsInserted->getPositionAt(face->getIndexPositionAt(i))));
		}
	}
	return vector;
}

vector<glm::vec3> Group::generateNormalData() {
	vector<glm::vec3> vector;
	for (Face* face : m_faces) {
		for (int i = 0; i < face->getSizeOfFace(); i++) {
			vector.push_back(*(this->meshThatIsInserted->getNormalAt(face->getIndexNormalAt(i))));
		}
	}
	return vector;
}

vector<glm::vec2> Group::generateTextureData() {
	vector<glm::vec2> vector;
	for (Face* face : m_faces) {
		for (int i = 0; i < face->getSizeOfFace(); i++) {
			vector.push_back(*(this->meshThatIsInserted->getTextureAt(face->getIndexTextureAt(i))));
		}
	}
	return vector;
}

bool Group::hasTexture() {
	return this->meshThatIsInserted->hasTexture();
}

