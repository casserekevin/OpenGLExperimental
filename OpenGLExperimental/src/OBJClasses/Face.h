#pragma once

#include <vector>

class Face {
private:
	std::vector<int> indexPositions;
	std::vector<int> indexTextures;
	std::vector<int> indexNormals;

	int size;
public:
	Face(int size = 3) : size(size) {}



	void addIndexPosition(int index) {
		if (this->indexPositions.size() < this->size) {
			this->indexPositions.push_back(index);
		}
	}
	void addIndexTexture(int index) {
		if (this->indexTextures.size() < this->size) {
			this->indexTextures.push_back(index);
		}
	}
	void addIndexNormal(int index) {
		if (this->indexNormals.size() < this->size) {
			this->indexNormals.push_back(index);
		}
	}



	~Face() {}



	//GETTERS
	inline int getIndexPositionAt(int index) { return this->indexPositions.at(index); }
	inline int getIndexTextureAt(int index) { return this->indexTextures.at(index); }
	inline int getIndexNormalAt(int index) { return this->indexNormals.at(index); }
	inline int getSizeOfFace() { return this->size; }
};
