#pragma once

#include <vector>

#include "FaceType.h"

class Face {
private:
	std::vector<int> m_indexPositions;
	std::vector<int> m_indexTextures;
	std::vector<int> m_indexNormals;
	
	int m_size;

	FaceType type;
public:
	Face(int size = 3): m_size(size) {}
	
	void addIndexPosition(int index) {
		if (m_indexPositions.size() < m_size)  {
			m_indexPositions.push_back(index);
		}
	}
	void addIndexTexture(int index) {
		if (m_indexTextures.size() < m_size) {
			m_indexTextures.push_back(index);
		}
	}
	void addIndexNormal(int index) {
		if (m_indexNormals.size() < m_size) {
			m_indexNormals.push_back(index);
		}
	}

	int getNumberOfPositions() {
		return m_indexPositions.size();
	}
	int getNumberOfTextures() {
		return m_indexTextures.size();
	}
	int getNumberOfNormals() {
		return m_indexNormals.size();
	}

	int getSizeOfFace() {
		return m_size;
	}

	FaceType getType() {
		if (m_indexPositions.size() > 0 && m_indexTextures.size() == 0 && m_indexNormals.size() == 0) {
			type = FaceType::V__;
		}
		else if (m_indexPositions.size() > 0 && m_indexTextures.size() > 0 && m_indexNormals.size() == 0) {
			type = FaceType::VT_;
		}
		else if (m_indexPositions.size() > 0 && m_indexTextures.size() == 0 && m_indexNormals.size() > 0) {
			type = FaceType::V_N;
		}
		else if (m_indexPositions.size() > 0 && m_indexTextures.size() > 0 && m_indexNormals.size() > 0) {
			type = FaceType::VTN;
		}

		return type;
	}

	int getIndexPositionAt(int index) {
		return m_indexPositions.at(index);
	}

	int getIndexNormalAt(int index) {
		return m_indexNormals.at(index);
	}

	int getIndexTextureAt(int index) {
		return m_indexTextures.at(index);
	}

	~Face() {}
};
