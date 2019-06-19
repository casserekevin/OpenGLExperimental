#pragma once

#include <GLM/glm.hpp>

#include <vector>
#include <math.h>

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



	// curva interna
	VertexArray* vaoInternalPoints;
	std::vector<glm::vec3*> internalPoints;
	// curva externa
	VertexArray* vaoExternalPoints;
	std::vector<glm::vec3*> externalPoints;


	//math constant
	const double PI = 3.14159265358979323846;


	std::vector<glm::vec3> generateVboData(vector<glm::vec3*> data) {
		std::vector<glm::vec3> return_data;
		for (int i = 0; i < data.size(); i++) {
			return_data.push_back(*data.at(i));
		}
		return return_data;
	}

	std::vector<glm::vec3*> generateVboDataCurves(vector<glm::vec3*> data, int distance, bool intern) {
		std::vector<glm::vec3*> return_data;
		if (intern) {
			for (int i = 0; i < data.size(); i++) {
				glm::vec3* A = data.at(i);
				glm::vec3* B = data.at((i + 1) % data.size());


				double w = B->x - A->x;
				double h = B->y - A->y;

				double angle;
				if (w < 0) {
					angle = (atan(h / w)) - (PI / 2);
				}
				else {
					angle = (atan(h / w)) + (PI / 2);
				}

				double Cx = cos(angle) * distance + A->x;
				double Cy = sin(angle) * distance + A->y;
				return_data.push_back(new glm::vec3(Cx, Cy, A->z));
			}
		}
		else {
			for (int i = 0; i < data.size(); i++) {
				glm::vec3* A = data.at(i);
				glm::vec3* B = data.at((i + 1) % data.size());


				double w = B->x - A->x;
				double h = B->y - A->y;

				double angle;
				if (w < 0) {
					angle = (atan(h / w)) + (PI / 2);
				}
				else {
					angle = (atan(h / w)) - (PI / 2);
				}

				double Cx = cos(angle) * distance + A->x;
				double Cy = sin(angle) * distance + A->y;
				return_data.push_back(new glm::vec3(Cx, Cy, A->z));
			}
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

	void eraseAllCalculatedPoints() {
		this->positionCalculatedPoints.clear();
		this->colorCalculatedPoints.clear();
		this->calculatedPoints.clear();
	}



	void generateInternalExternalCurves() {
		this->vaoInternalPoints = new VertexArray();
		this->vaoInternalPoints->bind();

		this->internalPoints = generateVboDataCurves(this->positionCalculatedPoints, 10, true);
		VertexBuffer* vboPositionInternalCurve = new VertexBuffer(this->positionCalculatedPoints.size() * sizeof(glm::vec3), generateVboData(this->internalPoints).data());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		vboPositionInternalCurve->Unbind();

		VertexBuffer* vboColorInternalCurve = new VertexBuffer(this->positionCalculatedPoints.size() * sizeof(glm::vec3), generateVboData(this->colorCalculatedPoints).data());
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		vboColorInternalCurve->Unbind();


		this->vaoExternalPoints = new VertexArray();
		this->vaoExternalPoints->bind();

		this->externalPoints = generateVboDataCurves(this->positionCalculatedPoints, 10, false);
		VertexBuffer* vboPositionExternalCurve = new VertexBuffer(this->positionCalculatedPoints.size() * sizeof(glm::vec3), generateVboData(this->externalPoints).data());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		vboPositionExternalCurve->Unbind();

		VertexBuffer* vboColorExternalCurve = new VertexBuffer(this->positionCalculatedPoints.size() * sizeof(glm::vec3), generateVboData(this->colorCalculatedPoints).data());
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		vboColorExternalCurve->Unbind();
	}

	~PointsManager(){}



	//GETTERS
	inline VertexArray* getVaoControlPoints() { return this->vaoControlPoints; }
	inline VertexArray* getVaoCalculatedPoints() { return this->vaoCalculatedPoints; }
	inline VertexArray* getVaoInternalCurve() { return this->vaoInternalPoints; }
	inline VertexArray* getVaoExternalCurve() { return this->vaoExternalPoints; }

	inline size_t getSizeControlPoints() { return this->positionControlPoints.size(); }
	inline size_t getSizeCalculatedPoints() { return this->positionCalculatedPoints.size(); }

	inline glm::vec3* getFirstPositionControlPoints() { return this->positionControlPoints.at(0); }
	
	inline glm::vec3* getPositionControlPointAt(int index) { return this->positionControlPoints.at(index); }

	inline Point* getControlPointAt(int index) { return this->controlPoints.at(index); }

	inline std::vector<glm::vec3*> getPositionCalculatedPoints() { return this->positionCalculatedPoints; }
	inline std::vector<glm::vec3*> getInternalExternalPoints() {
		for (int i = 0; i < this->internalPoints.size(); i++) {
			this->externalPoints.push_back(this->internalPoints.at(i));
		}
		return this->externalPoints;
	}


	//SETTERS
};