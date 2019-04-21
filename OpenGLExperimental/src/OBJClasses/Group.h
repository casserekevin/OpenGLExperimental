#pragma once

#include <GLEW/glew.h>
#include <GLM/glm.hpp>

#include <string>
#include <sstream>

#include "Face.h"
#include "../Program.h"
#include "../VertexBuffer.h"
#include "../Texture.h"

class Mesh;

using std::vector;
class Group {
private:
	Mesh* meshThatIsInserted;

	vector<Face*> m_faces;
	std::string pathFileTexture;

	GLuint vaoID;

	int numberOfVertices;
	
	int typeDraw;

	Texture* texture = nullptr;

	//functions
	vector<glm::vec3> generatePositionData();

	vector<glm::vec3> generateNormalData();

	vector<glm::vec2> generateTextureData();

	vector<glm::vec3> generateColorData();

	bool hasTexture();
public:
	Group(Mesh* mesh) {
		this->meshThatIsInserted = mesh;
	}

	void addFace(Face* face) {
		m_faces.push_back(face);
	}

	void setPathFileTexture(std::string pathFile) {
		pathFileTexture = pathFile;
	}

	GLint getTextureUnit() {
		return this->texture->getTextureUnit();
	}

	int getTypeDraw() {
		return this->typeDraw;
	}

	void enableColor() {
		this->typeDraw = typeDraw + 1;
	}

	void disableColor() {
		this->typeDraw = typeDraw - 1;
	}

	void createVAOandTexture() {
		glGenVertexArrays(1, &this->vaoID);
		glBindVertexArray(this->vaoID);

		vector<glm::vec3> vec_positions = generatePositionData();
		VertexBuffer* vbo_position = new VertexBuffer(vec_positions.size() * sizeof(glm::vec3), vec_positions.data());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		this->numberOfVertices = vec_positions.size();

		vector<glm::vec3> vec_normals = generateNormalData();
		VertexBuffer* vbo_normal = new VertexBuffer(vec_normals.size() * sizeof(glm::vec3), vec_normals.data());
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		vector<glm::vec2> vec_textures = generateTextureData();
		VertexBuffer* vbo_texture = new VertexBuffer(vec_textures.size() * sizeof(glm::vec2), vec_textures.data());
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		unbindVAO();

		if (this->pathFileTexture == "\"\"") {
			texture = new Texture("res/textures/null.png", GL_TEXTURE_2D, 0);
		}
		else {
			std::stringstream ss;
			ss << "res/textures/" << this->pathFileTexture;
			this->pathFileTexture = ss.str();
			texture = new Texture(this->pathFileTexture.c_str(), GL_TEXTURE_2D, 0);
		}
		
	}

	void bindVAO() {
		glBindVertexArray(this->vaoID);
	}

	void unbindVAO() {
		glBindVertexArray(0);
	}

	void draw(Program* program) {
		program->send1i("texture0", this->texture->getTextureUnit());
		program->send1i("typeDraw", this->typeDraw);

		texture->bind();

		program->use();
		bindVAO();
		glDrawArrays(GL_TRIANGLES, 0, this->numberOfVertices);
		unbindVAO();
	}
};
