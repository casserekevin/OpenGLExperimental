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
	const int MAX_HEIGHT = 40;
	const int TICK_HEIGHT = 20;
	const double RATE_HEIGHT = MAX_HEIGHT / TICK_HEIGHT;

	//flags
	bool stop_clicks = false;
	bool g_pressed = false;

	float B0(double t) {
		return (-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1);
	}
	float B1(double t) {
		return (3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4);
	}
	float B2(double t) {
		return (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1);
	}
	float B3(double t) {
		return (1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0);
	}

	glm::vec3* calculatePosition(glm::vec3* P0, glm::vec3* P1, glm::vec3* P2, glm::vec3* P3, double t) {
		float x = ((B0(t)*P0->x + B1(t)*P1->x + B2(t)*P2->x + B3(t)*P3->x) / 6);
		float y = ((B0(t)*P0->y + B1(t)*P1->y + B2(t)*P2->y + B3(t)*P3->y) / 6);

		return new glm::vec3(x, y, 0.0f);
	}

	glm::vec3* recalculatePosition(glm::vec3* P0, glm::vec3* P1, glm::vec3* P2, glm::vec3* P3, double t) {
		float x = ((B0(t)*P0->x + B1(t)*P1->x + B2(t)*P2->x + B3(t)*P3->x) / 6);
		float y = ((B0(t)*P0->y + B1(t)*P1->y + B2(t)*P2->y + B3(t)*P3->y) / 6);
		float z = ((B0(t)*P0->z + B1(t)*P1->z + B2(t)*P2->z + B3(t)*P3->z) / 6);

		return new glm::vec3(x, y, z);
	}

public:
	BSpline(Program* program, int width, int height): program(program), widthWindow(width), heightWindow(height) {
		this->pointsManager = new PointsManager();
	}


	void draw() {
		if (g_pressed) {
			drawInternalExternalCurve();
		}
		else {
			drawCtrlPoints();
			drawCalculatedPoints();
		}
	}
	void drawCtrlPoints() {
		program->use();
		
		this->pointsManager->getVaoControlPoints()->bind();
		if (stop_clicks) {
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

		glPointSize(2);
		this->pointsManager->getVaoCalculatedPoints()->bind();
		glDrawArrays(GL_LINE_STRIP, 0, this->pointsManager->getSizeCalculatedPoints());
		this->pointsManager->getVaoCalculatedPoints()->unbind();
	
	}

	void drawInternalExternalCurve() {
		program->use();

		glPointSize(4);
		this->pointsManager->getVaoInternalCurve()->bind();
		glDrawArrays(GL_LINE_LOOP, 0, this->pointsManager->getSizeCalculatedPoints());
		this->pointsManager->getVaoInternalCurve()->unbind();

		program->use();

		this->pointsManager->getVaoExternalCurve()->bind();
		glDrawArrays(GL_LINE_LOOP, 0, this->pointsManager->getSizeCalculatedPoints());
		this->pointsManager->getVaoExternalCurve()->unbind();
	}

	//entryPoint ClickMouse
	void clickMouseProcess(float x, float y) {
		if (this->stop_clicks == false) {
			if (closeToFirstPoint(x, y)) {
				this->pointsManager->eraseAllCalculatedPoints();
				calculatePoints(true);
				this->pointsManager->updateVbosCalculatedPoints();
			}
			else {
				this->pointsManager->addControlPoint(new glm::vec3(x, y, 0.0f), new glm::vec3(1.0f, 1.0f, 1.0f));
				this->pointsManager->updateVbosControlPoints();

				this->pointsManager->eraseAllCalculatedPoints();
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
				this->stop_clicks = true;
				return true;
			}
		}
		return false;
	}
	void calculatePoints(bool closed) {
		if (this->pointsManager->getSizeControlPoints() >= 4) {
			double rate = 1.0 / 100;
			if (closed) {
				for (int i = 0; i < this->pointsManager->getSizeControlPoints(); i++) {
					glm::vec3* P0 = this->pointsManager->getPositionControlPointAt(i);
					glm::vec3* P1 = this->pointsManager->getPositionControlPointAt((i + 1) % this->pointsManager->getSizeControlPoints());
					glm::vec3* P2 = this->pointsManager->getPositionControlPointAt((i + 2) % this->pointsManager->getSizeControlPoints());
					glm::vec3* P3 = this->pointsManager->getPositionControlPointAt((i + 3) % this->pointsManager->getSizeControlPoints());

					for (double t = 0.0; t < (1.0 - (rate/2)); t += rate) {
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

					for (double t = 0.0; t < (1 - (rate/2)); t += rate) {
						glm::vec3* P = calculatePosition(P0, P1, P2, P3, t);
						this->pointsManager->addCalculatedPoint(P, new glm::vec3(1.0f, 1.0f, 1.0f));
						
						
					}
				}
			}
			
		}
	}

	//entryPoint Scroll Mouse
	void scrollMouseProcess(float x, float y, double yoffset) {
		if (stop_clicks && !g_pressed) {
			bool scroll_mouse_baixo = (yoffset == -1) ? true : false;

			vector<Point*> points = searchControlPointsAroundMouse(x, y);

			for (int i = 0; i < points.size(); i++) {
				changeHeightPoint(points.at(i), scroll_mouse_baixo);
			}
			this->pointsManager->eraseAllCalculatedPoints();
			recalculatePoints();
			this->pointsManager->updateVbosControlPoints();
			this->pointsManager->updateVbosCalculatedPoints();
		}
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
	void recalculatePoints() {
		double rate = 1.0 / 100;
		for (int i = 0; i < this->pointsManager->getSizeControlPoints(); i++) {
			glm::vec3* P0 = this->pointsManager->getPositionControlPointAt(i);
			glm::vec3* P1 = this->pointsManager->getPositionControlPointAt((i + 1) % this->pointsManager->getSizeControlPoints());
			glm::vec3* P2 = this->pointsManager->getPositionControlPointAt((i + 2) % this->pointsManager->getSizeControlPoints());
			glm::vec3* P3 = this->pointsManager->getPositionControlPointAt((i + 3) % this->pointsManager->getSizeControlPoints());

			for (double t = 0.0; t < (1 - (rate/2)); t += rate) {
				glm::vec3* P = recalculatePosition(P0, P1, P2, P3, t);
				glm::vec3* C = calculateColor(P);
				this->pointsManager->addCalculatedPoint(P, C);
			}
		}
	}
	glm::vec3* calculateColor(glm::vec3* position) {
		if (position->z <= 0) {
			return new glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else {
			double porcentagem = position->z / MAX_HEIGHT;
			return new glm::vec3(1.0f, 1.0f - porcentagem, 1.0f - porcentagem);
		}
	}

	//entryPoint G Button
	void gButtonProcess() {
		if (stop_clicks) {
			this->pointsManager->generateInternalExternalCurves();
			this->g_pressed = true;
		}
	}



	//GETTERS
	inline std::vector<glm::vec3*> getRoute() { return this->pointsManager->getPositionCalculatedPoints(); }
	inline std::vector<glm::vec3*> getInternalExternalCurve() { return this->pointsManager->getInternalExternalPoints(); }

	//SETTERS
	inline void setWidth(int width) { this->widthWindow = width; }
	inline void setHeight(int height) { this->heightWindow = height; }

	
	~BSpline(){}
};
