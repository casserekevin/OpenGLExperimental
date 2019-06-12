#pragma once

#include <GLEW/glew.h>
#include <GLM/glm.hpp>

#include <vector>

#include "Program.h"
#include "PointsManager.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

class BSpline {
private:
	int widthWindow;
	int heightWindow;

	Program* program;

	PointsManager* pointsManager;

	//default values
	const int MAX_HEIGHT = 20;
	const int TICK_HEIGHT = 20;
	const double RATE_HEIGHT = MAX_HEIGHT / TICK_HEIGHT;

	//flags
	bool stop = false;

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

	glm::vec3* calculatePosition(glm::vec3* P0, glm::vec3* P1, glm::vec3* P2, glm::vec3* P3, float t) {
		float x = ((B0(t)*P0->x + B1(t)*P1->x + B2(t)*P2->x + B3(t)*P3->x) / 6);
		float y = ((B0(t)*P0->y + B1(t)*P1->y + B2(t)*P2->y + B3(t)*P3->y) / 6);

		return new glm::vec3(x, y, 0.0f);
	}

public:
	BSpline(Program* program, int width, int height): program(program), widthWindow(width), heightWindow(height) {
		this->pointsManager = new PointsManager();
	}


	void draw() {
		drawCtrlPoints();
		drawCalculatedPoints();
	}
	void drawCtrlPoints() {
		program->use();
		
		this->pointsManager->getVaoControlPoints()->bind();
		if (stop) {
			glDrawArrays(GL_LINE_LOOP, 0, this->pointsManager->getSizeControlPoints());
		}
		else {
			glDrawArrays(GL_LINE_STRIP, 0, this->pointsManager->getSizeControlPoints());
		}
		this->pointsManager->getVaoControlPoints()->unbind();


		program->use();

		glPointSize(5);
		this->pointsManager->getVaoControlPoints()->bind();
		glDrawArrays(GL_POINTS, 0, this->pointsManager->getSizeControlPoints());
		this->pointsManager->getVaoControlPoints()->unbind();
	}
	void drawCalculatedPoints() {
		program->use();

		glPointSize(1);
		this->pointsManager->getVaoCalculatedPoints()->bind();
		glDrawArrays(GL_POINTS, 0, this->pointsManager->getSizeCalculatedPoints());
		this->pointsManager->getVaoCalculatedPoints()->unbind();
	
	}

	//entryPoint ClickMouse
	void clickMouseProcess(float x, float y) {
		if (this->stop == false) {
			if (closeToFirstPoint(x, y)) {
				calculatePoints(true);
				this->pointsManager->updateVbosCalculatedPoints();
			}
			else {
				this->pointsManager->addControlPoint(new glm::vec3(x, y, 0.0f), new glm::vec3(1.0f, 1.0f, 1.0f));
				this->pointsManager->updateVbosControlPoints();

				calculatePoints(false);
				this->pointsManager->updateVbosCalculatedPoints();
			}
		}
	}

	bool closeToFirstPoint(float x, float y) {
		if (this->pointsManager->getSizeControlPoints() >= 4) {
			int porcentagem = 2;
			
			int widthPixel = this->widthWindow * porcentagem / 100;
			int heightPixel = this->heightWindow * porcentagem / 100;

			glm::vec3* P = this->pointsManager->getFirstPositionControlPoints();
			if (((x >= P->x - widthPixel) && (x <= P->x + widthPixel)) && ((y >= P->y - heightPixel) && (y <= P->y + heightPixel))) {
				this->stop = true;
				return true;
			}
		}
		return false;
	}

	void calculatePoints(bool closed) {
		if (this->pointsManager->getSizeControlPoints() >= 4) {
			if (closed) {
				for (int i = 0; i < this->pointsManager->getSizeControlPoints(); i++) {
					glm::vec3* P0 = this->pointsManager->getPositionControlPointAt(i);
					glm::vec3* P1 = this->pointsManager->getPositionControlPointAt((i + 1) % this->pointsManager->getSizeControlPoints());
					glm::vec3* P2 = this->pointsManager->getPositionControlPointAt((i + 2) % this->pointsManager->getSizeControlPoints());
					glm::vec3* P3 = this->pointsManager->getPositionControlPointAt((i + 3) % this->pointsManager->getSizeControlPoints());

					for (float t = 0.0f; t < 1.0f; t += 0.01f) {
						glm::vec3* P = calculatePosition(P0, P1, P2, P3, t);
						this->pointsManager->addCalculatedPoint(P, new glm::vec3(1.0f, 1.0f, 1.0f));
					}
				}
			}
			else {
				for (int i = 0; i < this->pointsManager->getSizeControlPoints() - 3; i++) {
					glm::vec3* P0 = this->pointsManager->getPositionControlPointAt(i);
					glm::vec3* P1 = this->pointsManager->getPositionControlPointAt(i + 1);
					glm::vec3* P2 = this->pointsManager->getPositionControlPointAt(i + 2);
					glm::vec3* P3 = this->pointsManager->getPositionControlPointAt(i + 3);

					for (float t = 0.0f; t < 1.0f; t += 0.01f) {
						glm::vec3* P = calculatePosition(P0, P1, P2, P3, t);
						this->pointsManager->addCalculatedPoint(P, new glm::vec3(1.0f, 1.0f, 1.0f));
						
						
					}
				}
			}
			
		}
	}

	//entryPoint Scroll Mouse
	void scrollMouseProcess(float x, float y, double yoffset) {
		bool scroll_mouse_baixo = (yoffset == -1) ? true : false;

		vector<Point*> points = searchControlPointsAroundMouse(x, y);

		for (int i = 0; i < points.size(); i++) {
			changeHeightPoint(points.at(i), scroll_mouse_baixo);
		}
		this->pointsManager->updateVbosControlPoints();
	}

	vector<Point*> searchControlPointsAroundMouse(float x, float y) {
		vector<Point*> points;
		for (int i = 0; i < this->pointsManager->getSizeControlPoints(); i++) {
			Point* point = this->pointsManager->getControlPointAt(i);
			if (isPointAroundMouse(x, y, point)) {
				points.push_back(point);
			}
		}
		return points;
	}

	bool isPointAroundMouse(float x, float y, Point* point) {
		int porcentagem = 2;

		int widthPixel = this->widthWindow * porcentagem / 100;
		int heightPixel = this->heightWindow * porcentagem / 100;

		if (((point->getPosition()->x >= x - widthPixel) && (point->getPosition()->x <= x + widthPixel)) && ((point->getPosition()->y >= y - heightPixel) && (point->getPosition()->y <= y + heightPixel))) {
			return true;
		}
		return false;
	}

	void changeHeightPoint(Point* point, bool scroll_mouse_baixo) {
		(scroll_mouse_baixo) ? point->increaseHeight(RATE_HEIGHT, MAX_HEIGHT, true) : point->decreaseHeight(RATE_HEIGHT, MAX_HEIGHT, true);
	}

	//GETTERS

	//SETTERS
	inline void setWidth(int width) { this->widthWindow = width; }
	inline void setHeight(int height) { this->heightWindow = height; }
	
	~BSpline(){}
};
