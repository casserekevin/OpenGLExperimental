#pragma once

#include <GLM/glm.hpp>

#include <string>

#include "Texture.h"

class Material {
private:
	glm::vec3 Ka; //ambient
	glm::vec3 Kd; //diffuse
	glm::vec3 Ks; //specular

	float Ns; //shiness

	Texture* texture1;
public:
	Material() {
	}



	void addKa(float x, float y, float z){
		this->Ka = glm::vec3(x, y, z);
	}
	void addKd(float x, float y, float z) {
		this->Kd = glm::vec3(x, y, z);
	}
	void addKs(float x, float y, float z) {
		this->Ks = glm::vec3(x, y, z);
	}
	void addNs(float value) {
		this->Ns = value;
	}
	void addTexture(std::string texturePathfile) {
		this->texture1 = new Texture(texturePathfile, GL_TEXTURE_2D, 0);
	}



	~Material(){}



	//GETTERS
	inline glm::vec3 getKa() { return this->Ka; }
	inline glm::vec3 getKd() { return this->Kd; }
	inline glm::vec3 getKs() { return this->Ks; }
	inline float getNs() { return this->Ns; }
	inline Texture* getTexture() { return this->texture1; }
};
