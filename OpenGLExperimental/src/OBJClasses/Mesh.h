#pragma once

#include <GLM/glm.hpp>

#include <vector>
#include <iostream>

#include "Group.h"

using std::vector;
class Mesh{
private:
	vector<glm::vec3*> m_vertices;
	vector<glm::vec2*> m_textures;
	vector<glm::vec3*> m_normals;

	vector<Group*> m_groups;
public:
	Mesh(){}

	void addVertice(float x, float y, float z) {
		m_vertices.push_back(new glm::vec3(x, y, z));
	}
	void addVertice(glm::vec3* vertice) {
		m_vertices.push_back(vertice);
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

	Group* getLastGroup() {
		return m_groups.at(m_groups.size() - 1);
	}

	void printAllVertices() {
		for (glm::vec3* vertice : m_vertices) {
			std::cout << "X:" << vertice->x << ", Y:" << vertice->y << ", Z:" << vertice->z << std::endl;
		}
	}
};