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

	vector<Face*> faces;

	GLuint vaoID;

	Texture* texture = nullptr;
	std::string filepathTexture;
	
	int numberOfVertices;
	int typeDraw;

	//functions
	vector<glm::vec3> generatePositionData();
	vector<glm::vec2> generateTextureData();
	vector<glm::vec3> generateNormalData();

	bool hasTexture();
	bool hasNormal();
public:
	Group(Mesh* mesh): meshThatIsInserted(mesh) {}
	
	void draw(Program* program) {
		program->send1i("texture0", this->texture->getTextureUnit());
		program->send1i("typeDraw", this->typeDraw);

		texture->bind();

		program->use();
		bindVAO();
		glDrawArrays(GL_TRIANGLES, 0, this->numberOfVertices);
		unbindVAO();
	}



	void addFace(Face* face) {
		this->faces.push_back(face);
	}
	
	
	
	void createVAOandTexture() {
		glGenVertexArrays(1, &this->vaoID);
		glBindVertexArray(this->vaoID);

		vector<glm::vec3> vec_positions = generatePositionData();
		VertexBuffer* vbo_position = new VertexBuffer(vec_positions.size() * sizeof(glm::vec3), vec_positions.data());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		this->numberOfVertices = vec_positions.size();

		vector<glm::vec2> vec_textures = generateTextureData();
		VertexBuffer* vbo_texture = new VertexBuffer(vec_textures.size() * sizeof(glm::vec2), vec_textures.data());
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		vector<glm::vec3> vec_normals = generateNormalData();
		VertexBuffer* vbo_normal = new VertexBuffer(vec_normals.size() * sizeof(glm::vec3), vec_normals.data());
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		unbindVAO();

		if (this->filepathTexture == "\"\"") {
			texture = new Texture("res/textures/null.png", GL_TEXTURE_2D, 0);
		}
		else {
			std::stringstream ss;
			ss << "res/textures/" << this->filepathTexture;
			this->filepathTexture = ss.str();
			texture = new Texture(this->filepathTexture.c_str(), GL_TEXTURE_2D, 0);
		}
	}

	void enableColor() {
		this->typeDraw = typeDraw + 1;
	}
	void disableColor() {
		this->typeDraw = typeDraw - 1;
	}

	void bindVAO() {
		glBindVertexArray(this->vaoID);
	}
	void unbindVAO() {
		glBindVertexArray(0);
	}



	~Group() {}



	//GETTERS

	//SETTERS
	void setFilepathTexture(std::string filepath) { this->filepathTexture = filepath; }
};
