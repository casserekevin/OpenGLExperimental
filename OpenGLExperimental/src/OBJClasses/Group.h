#pragma once

#include <GLEW/glew.h>
#include <GLM/glm.hpp>

#include <vector>
#include <string>

#include "Face.h"
#include "../VertexBuffer.h"

class Mesh;

using std::vector;
class Group {
private:
	Mesh* meshThatIsInserted = nullptr;

	vector<Face*> m_faces;
	std::string m_nome;

	GLuint vaoID;

	//functions
	vector<glm::vec3> generatePositionData();

	vector<glm::vec3> generateNormalData();

	vector<glm::vec2> generateTextureData();

	bool hasTexture();
public:
	Group(Mesh* mesh) {
		this->meshThatIsInserted = mesh;
	}

	void addFace(Face* face) {
		m_faces.push_back(face);
	}

	void setNome(std::string nome) {
		m_nome = nome;
	}

	void createVAO() {
		glGenVertexArrays(1, &this->vaoID);
		glBindVertexArray(this->vaoID);

		vector<glm::vec3> vec_positions = generatePositionData();
		VertexBuffer* vbo_position = new VertexBuffer(vec_positions.size() * sizeof(glm::vec3), vec_positions.data());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		vector<glm::vec3> vec_normals = generateNormalData();
		VertexBuffer* vbo_normal = new VertexBuffer(vec_normals.size() * sizeof(glm::vec3), vec_normals.data());
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		if (hasTexture()) {
			vector<glm::vec2> vec_textures = generateTextureData();
			VertexBuffer* vbo_texture = new VertexBuffer(vec_textures.size() * sizeof(glm::vec2), vec_textures.data());
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		}

		unbindVAO();
	}

	void bindVAO() {
		glBindVertexArray(this->vaoID);
	}

	void unbindVAO() {
		glBindVertexArray(0);
	}
};
