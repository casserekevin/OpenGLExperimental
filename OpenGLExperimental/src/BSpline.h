#pragma once

#include <GLEW/glew.h>
#include <GLM/glm.hpp>

#include <vector>

#include "Program.h"
#include "VertexBuffer.h"


class BSpline {
private:
	int widthWindow;
	int heightWindow;

	Program* program;

	std::vector<glm::vec3> ctrlPoints;
	std::vector<glm::vec3> calculatedPoints;

	GLuint vaoCtrlPointsID;
	GLuint vaoCalculatedPointsID;

	bool stop = false;
	
	void bindVao(GLuint vao) {
		glBindVertexArray(vao);
	}
	void unbindVAO() {
		glBindVertexArray(0);
	}

	float B0(float t) {
		return (-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1);
	}
	float B1(float t) {
		return (3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4);
	}
	float B2(float t) {
		return (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1);
	}
	float B3(float t) {
		return (1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0);
	}

	glm::vec3 calculatePoint(glm::vec3 P0, glm::vec3 P1, glm::vec3 P2, glm::vec3 P3, float t) {
		float x = ((B0(t)*P0.x + B1(t)*P1.x + B2(t)*P2.x + B3(t)*P3.x) / 6);
		float y = ((B0(t)*P0.y + B1(t)*P1.y + B2(t)*P2.y + B3(t)*P3.y) / 6);

		return glm::vec3(x, y, 0.0f);
	}

public:
	BSpline(Program* program, int width, int height): program(program), widthWindow(width), heightWindow(height) {
		generateCtrlPointsVAO();
	}



	void draw() {
		drawCtrlPoints();
		drawCalculatedPoints();
	}
	void drawCtrlPoints() {
		program->sendVec3fv("color", glm::vec3(1.0f, 0.0f, 0.0f));

		program->use();
		
		bindVao(this->vaoCtrlPointsID);
		if (stop) {
			glDrawArrays(GL_LINE_LOOP, 0, this->ctrlPoints.size());
		}
		else {
			glDrawArrays(GL_LINE_STRIP, 0, this->ctrlPoints.size());
		}
		unbindVAO();

		program->sendVec3fv("color", glm::vec3(1.0f, 1.0f, 0.0f));

		program->use();

		glPointSize(5);
		bindVao(this->vaoCtrlPointsID);
		glDrawArrays(GL_POINTS, 0, this->ctrlPoints.size());
		unbindVAO();
	}
	void drawCalculatedPoints() {
		program->sendVec3fv("color", glm::vec3(1.0f, 1.0f, 1.0f));

		program->use();

		glPointSize(2);
		bindVao(this->vaoCalculatedPointsID);
		glDrawArrays(GL_POINTS, 0, this->calculatedPoints.size());
		unbindVAO();
	}


	void addCtrlPoints(float x, float y, float z) {
		if (this->stop == false) {
			if (closeToFirstPoint(x, y)) {
				calculatePoints(true);
				generateCalculatedPointsVAO();
			}
			else {
				this->ctrlPoints.push_back(glm::vec3(x, y, z));
				generateCtrlPointsVAO();

				calculatePoints(false);
				generateCalculatedPointsVAO();
			}
		}
	}

	bool closeToFirstPoint(float x, float y) {
		if (this->ctrlPoints.size() >= 4) {
			int porcentagem = 2;
			
			int widthPixel = this->widthWindow * porcentagem / 100;
			int heightPixel = this->heightWindow * porcentagem / 100;

			glm::vec3 P = ctrlPoints.at(0);
			if (((x >= P.x - widthPixel) && (x <= P.x + widthPixel)) && ((y >= P.y - heightPixel) && (y <= P.y + heightPixel))) {
				this->stop = true;
				return true;
			}
		}
		return false;
	}



	void generateCtrlPointsVAO() {
		glGenVertexArrays(1, &this->vaoCtrlPointsID);
		glBindVertexArray(this->vaoCtrlPointsID);

		VertexBuffer* vertexbuffer = new VertexBuffer(ctrlPoints.size() * sizeof(glm::vec3), ctrlPoints.data());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		unbindVAO();
	}
	void generateCalculatedPointsVAO() {
		glGenVertexArrays(1, &this->vaoCalculatedPointsID);
		glBindVertexArray(this->vaoCalculatedPointsID);

		VertexBuffer* vertexbuffer = new VertexBuffer(calculatedPoints.size() * sizeof(glm::vec3), calculatedPoints.data());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		unbindVAO();
	}

	void calculatePoints(bool closed) {
		if (this->ctrlPoints.size() >= 4) {
			if (closed) {
				for (int i = 0; i < ctrlPoints.size(); i++) {
					glm::vec3 P0 = ctrlPoints.at(i);
					glm::vec3 P1 = ctrlPoints.at((i + 1) % ctrlPoints.size());
					glm::vec3 P2 = ctrlPoints.at((i + 2) % ctrlPoints.size());
					glm::vec3 P3 = ctrlPoints.at((i + 3) % ctrlPoints.size());

					for (float t = 0.0f; t < 1.0f; t += 0.01f) {
						glm::vec3 P = calculatePoint(P0, P1, P2, P3, t);
						this->calculatedPoints.push_back(P);
					}
				}
			}
			else {
				for (int i = 0; i < ctrlPoints.size() - 3; i++) {
					glm::vec3 P0 = ctrlPoints.at(i);
					glm::vec3 P1 = ctrlPoints.at(i + 1);
					glm::vec3 P2 = ctrlPoints.at(i + 2);
					glm::vec3 P3 = ctrlPoints.at(i + 3);

					for (float t = 0.0f; t < 1.0f; t += 0.01f) {
						glm::vec3 P = calculatePoint(P0, P1, P2, P3, t);
						this->calculatedPoints.push_back(P);
					}
				}
			}
			
		}
	}



	//GETTERS

	//SETTERS
	inline void setWidth(int width) { this->widthWindow = width; }
	inline void setHeight(int height) { this->heightWindow = height; }
	
	~BSpline(){}
};
