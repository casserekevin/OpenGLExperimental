#pragma once

#include <GLM/glm.hpp>

#include <vector>

#include "Point.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

class PointsManager {
private:
	VertexArray* vaoControlPoints;
	VertexBuffer* vboPositionControlPoints;
	VertexBuffer* vboColorControlPoints;
	std::vector<glm::vec3*> positionControlPoints;
	std::vector<glm::vec3*> colorControlPoints;

	VertexArray* vaoCalculatedPoints;
	VertexBuffer* vboPositionCalculatedPoints;
	VertexBuffer* vboColorCalculatedPoints;
	std::vector<glm::vec3*> positionCalculatedPoints;
	std::vector<glm::vec3*> colorCalculatedPoints;


	std::vector<Point*> controlPoints;
	std::vector<Point*> calculatedPoints;

	std::vector<glm::vec3> generateVboData(vector<glm::vec3*> data) {
		std::vector<glm::vec3> return_data;
		for (int i = 0; i < data.size(); i++) {
			return_data.push_back(*data.at(i));
		}
		return return_data;
	}

public:
	PointsManager(){
		this->vaoControlPoints = new VertexArray();
		this->vaoControlPoints->bind();

		this->vboPositionControlPoints = new VertexBuffer(this->positionControlPoints.size() * sizeof(glm::vec3), generateVboData(this->positionControlPoints).data());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		this->vboPositionControlPoints->Unbind();

		this->vboColorControlPoints = new VertexBuffer(this->colorControlPoints.size() * sizeof(glm::vec3), generateVboData(this->colorControlPoints).data());
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		this->vboColorControlPoints->Unbind();


		this->vaoCalculatedPoints = new VertexArray();
		this->vaoCalculatedPoints->bind();

		this->vboPositionCalculatedPoints = new VertexBuffer(this->positionCalculatedPoints.size() * sizeof(glm::vec3), generateVboData(this->positionCalculatedPoints).data());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		this->vboPositionCalculatedPoints->Unbind();

		this->vboColorCalculatedPoints = new VertexBuffer(this->colorCalculatedPoints.size() * sizeof(glm::vec3), generateVboData(this->colorCalculatedPoints).data());
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		this->vboColorCalculatedPoints->Unbind();

	}

	void updateVbosControlPoints() {
		this->vboPositionControlPoints->update(this->positionControlPoints.size() * sizeof(glm::vec3), generateVboData(this->positionControlPoints).data());
		this->vboColorControlPoints->update(this->colorControlPoints.size() * sizeof(glm::vec3), generateVboData(this->colorControlPoints).data());
	}
	void updateVbosCalculatedPoints() {
		this->vboPositionCalculatedPoints->update(this->positionCalculatedPoints.size() * sizeof(glm::vec3), generateVboData(this->positionCalculatedPoints).data());
		this->vboColorCalculatedPoints->update(this->colorCalculatedPoints.size() * sizeof(glm::vec3), generateVboData(this->colorCalculatedPoints).data());
	}

	void addControlPoint(glm::vec3* position, glm::vec3* color) {
		this->positionControlPoints.push_back(position);
		this->colorControlPoints.push_back(color);
		this->controlPoints.push_back(new Point(position, color));
	}
	void addCalculatedPoint(glm::vec3* position, glm::vec3* color) {
		this->positionCalculatedPoints.push_back(position);
		this->colorCalculatedPoints.push_back(color);
		this->calculatedPoints.push_back(new Point(position, color));
	}



	~PointsManager(){}



	//GETTERS
	inline VertexArray* getVaoControlPoints() { return this->vaoControlPoints; }
	inline VertexArray* getVaoCalculatedPoints() { return this->vaoCalculatedPoints; }

	inline size_t getSizeControlPoints() { return this->positionControlPoints.size(); }
	inline size_t getSizeCalculatedPoints() { return this->positionCalculatedPoints.size(); }

	inline glm::vec3* getFirstPositionControlPoints() { return this->positionControlPoints.at(0); }
	
	inline glm::vec3* getPositionControlPointAt(int index) { return this->positionControlPoints.at(index); }

	inline Point* getControlPointAt(int index) { return this->controlPoints.at(index); }


	//SETTERS
};