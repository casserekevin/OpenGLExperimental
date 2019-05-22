#pragma once

#include <GLM/glm.hpp>

#include <iostream>

#include "Group.h"

class OBJ;

class Mesh {
private:
	OBJ* objThatIsInserted = nullptr;
	
	vector<glm::vec3*> positions;
	vector<glm::vec2*> textures;
	vector<glm::vec3*> normals;

	vector<Group*> groups;

public:
	Mesh() {}

	void draw(Program* program) {
		for (Group* g : this->groups) {
			g->draw(program);
		}
	}



	void addVertice(float x, float y, float z) {
		this->positions.push_back(new glm::vec3(x, y, z));
	}
	void addVertice(glm::vec3* vertice) {
		this->positions.push_back(vertice);
	}

	void addTexture(float x, float y) {
		this->textures.push_back(new glm::vec2(x, y));
	}
	void addTexture(glm::vec2* texture) {
		this->textures.push_back(texture);
	}

	void addNormal(float x, float y, float z) {
		this->normals.push_back(new glm::vec3(x, y, z));
	}
	void addNormal(glm::vec3* normal) {
		this->normals.push_back(normal);
	}
	void addGroup(Group* group) {
		this->groups.push_back(group);
	}



	void applyColor() {
		for (Group* g : this->groups) {
			g->enableColor();
		}
	}
	void disapplyColor() {
		for (Group* g : this->groups) {
			g->disableColor();
		}
	}



	~Mesh() {}



	//GETTERS
	inline int getSizeOfGroups() { return this->groups.size(); }
	inline vector<Group*> getGroups() { return this->groups; }
	inline Group* getLastGroup() { return this->groups.at(this->groups.size() - 1); }
	
	inline glm::vec3* getPositionAt(int index) { return this->positions.at(index); }
	inline glm::vec2* getTextureAt(int index) { return this->textures.at(index); }
	inline glm::vec3* getNormalAt(int index) { return this->normals.at(index); }
	
	inline bool hasTexture(){ return (this->textures.size() == 0)? false : true; }
	inline bool hasNormal() { return (this->normals.size() == 0) ? false : true; }

	//SETTERS
	void setObjThatIsInserted(OBJ* obj) { this->objThatIsInserted = obj; }
};

