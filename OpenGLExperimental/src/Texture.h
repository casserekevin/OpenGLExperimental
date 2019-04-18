#pragma once

#include <GLEW/glew.h>
#include <SOIL2/SOIL2.h>

#include <iostream>

class Texture{
private:
	GLuint textureID;
	int width;
	int height;
	unsigned int type;
	GLint textureUnit;

public:

	Texture(const char* fileNameTexture, GLenum type, GLint texture_unit) {
		this->type = type;
		this->textureUnit = texture_unit;

		//Carrega imagem
		unsigned char* data_image = SOIL_load_image(fileNameTexture, &this->width, &this->height, nullptr, SOIL_LOAD_RGBA);

		//Gera textura
		glGenTextures(1, &this->textureID);
		glBindTexture(this->type, this->textureID);

		//Configura Textura
		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT); //Caso o obj for maior que a textura no eixo X, repete a textura
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT); //Caso o obj for maior que a textura no eixo Y, repete a textura
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Filtro usado quando o objeto aumentar de tamanho
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Filtro usado quando o objeto diminuir de tamanho

		// Se a imagem carregou corretamente
		if (data_image) {
			glTexImage2D(this->type, 0, GL_RGBA, this->width, this->width, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_image);
			glGenerateMipmap(this->type);
		}
		else {
			std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED" << std::endl;
		}
		unbind();
		SOIL_free_image_data(data_image);
	}

	~Texture() {
		glDeleteTextures(1, &this->textureID);
	}


	void bind() {
		glActiveTexture(GL_TEXTURE0 + this->textureUnit);
		glBindTexture(this->type, this->textureID);
	}

	void unbind() {
		glActiveTexture(0);
		glBindTexture(this->type, 0);
	}

	inline GLuint getTextureID() const{ return this->textureID; }
	
	inline GLint getTextureUnit() const{ return this->textureUnit; }
};