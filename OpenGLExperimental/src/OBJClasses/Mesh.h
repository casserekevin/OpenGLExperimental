#pragma once

#include <GLM/glm.hpp>


#include <iostream>

#include "Group.h"

class OBJ;

class Mesh {
private:
	OBJ* objThatIsInserted = nullptr;
	
	vector<glm::vec3*> m_positions;
	vector<glm::vec2*> m_textures;
	vector<glm::vec3*> m_normals;

	vector<Group*> m_groups;

public:
	Mesh() {}

	void setObjThatIsInserted(OBJ* obj) {
	objThatIsInserted = obj;
	}

	void addVertice(float x, float y, float z) {
		m_positions.push_back(new glm::vec3(x, y, z));
	}
	void addVertice(glm::vec3* vertice) {
		m_positions.push_back(vertice);
	}

	void addTexture(float x, float y) {
		m_textures.push_back(new glm::vec2(x, y));
	}
	void addTexture(glm::vec2* texture) {
		m_textures.push_back(texture);
	}

	void addNormal(float x, float y, float z) {
		m_normals.push_back(new glm::vec3(x, y, z));
	}
	void addNormal(glm::vec3* normal) {
		m_normals.push_back(normal);
	}
	void addGroup(Group* group) {
		m_groups.push_back(group);
	}

	vector<Group*> getGroups() {
		return m_groups;
	}

	Group* getLastGroup() {
		return m_groups.at(m_groups.size() - 1);
	}

	void printAllVertices() {
		for (glm::vec3* vertice : m_positions) {
			std::cout << "X:" << vertice->x << ", Y:" << vertice->y << ", Z:" << vertice->z << std::endl;
		}
	}

	glm::vec3* getPositionAt(int index) {
		return m_positions.at(index);
	}

	glm::vec3* getNormalAt(int index) {
		return m_normals.at(index);
	}

	glm::vec2* getTextureAt(int index) {
		return m_textures.at(index);
	}

	bool isTexture(){
		return (this->m_textures.size() == 0)? false : true;
	}
	
	GLint getFirstTextureUnit() {
		return this->m_groups.at(0)->getTextureUnit();
	}

	int getFirstTypeDraw() {
		return this->m_groups.at(0)->getTypeDraw();
	}

	void applyColor() {
		for (Group* g : this->m_groups) {
			g->enableColor();
		}
	}

	void disapplyColor() {
		for (Group* g : this->m_groups) {
			g->disableColor();
		}
	}

	void draw(Program* program) {
		for (Group* g : this->m_groups) {
			g->draw(program);
		}
	}

};

