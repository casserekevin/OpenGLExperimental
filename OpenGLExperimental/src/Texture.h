#pragma once

#include <GLEW/glew.h>
#include <SOIL2/SOIL2.h>

#include <iostream>

class Texture {
private:
	GLuint textureID;
	GLenum type;
	GLint textureUnit;

public:
	Texture(const char* pathfile, GLenum type, GLint textureUnit) : type(type), textureUnit(textureUnit) {
		//Carrega imagem
		int width;
		int height;
		unsigned char* dataImage = SOIL_load_image(pathfile, &width, &height, nullptr, SOIL_LOAD_RGBA);

		//Gera textura
		glGenTextures(1, &this->textureID);
		glBindTexture(this->type, this->textureID);

		//Configura Textura
		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT); //Caso o obj for maior que a textura no eixo X, repete a textura
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT); //Caso o obj for maior que a textura no eixo Y, repete a textura
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Filtro usado quando o objeto aumentar de tamanho
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Filtro usado quando o objeto diminuir de tamanho

		// Se a imagem carregou corretamente
		if (dataImage) {
			glTexImage2D(this->type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataImage);
			glGenerateMipmap(this->type);
		}
		else {
			std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED" << std::endl;
		}
		unbind();
		SOIL_free_image_data(dataImage);
	}



	void bind() {
		glActiveTexture(GL_TEXTURE0 + this->textureUnit);
		glBindTexture(this->type, this->textureID);
	}

	void unbind() {
		glActiveTexture(0);
		glBindTexture(this->type, 0);
	}



	~Texture() {
		glDeleteTextures(1, &this->textureID);
	}



	//GETTERS
	inline GLuint getTextureID() const { return this->textureID; }
	inline GLint getTextureUnit() const { return this->textureUnit; }
};