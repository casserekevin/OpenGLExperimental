#pragma once
#include "Scene.h"
class MyScene : public Scene{

private:
	unsigned int m_ProgramID;
	unsigned int m_VertexShaderID;
	unsigned int m_FragmentShaderID;

	unsigned int m_vboPositionsID;
	unsigned int m_vboColorsID;

	unsigned int m_vaoID;

	int m_matrixLocation;

	//DADOS 
	//transla��o
	float m_matrix[16] = { 
		 1.0f, 0.0f, 0.0f, 0.0f, // 1� coluna
		 0.0f, 1.0f, 0.0f, 0.0f, // 2� coluna
		 0.0f, 0.0f, 1.0f, 0.0f, // 3� coluna
		 0.0f, 0.0f, 0.0f, 1.0f // 4� coluna
	};

	float m_speedX = 0.5f;
	float m_speedY = 0.0f;

	float m_initialPositionX = 0.0f;
	float m_initialPositionY = 0.0f;

	float m_topPosition;
	float m_bottomPosition;
	float m_leftPosition;
	float m_rightPosition;

	bool m_firstHit = true;

	/*float m_speed = 0.25f;
	float m_lastPosition = 0.0f;*/


	//Verifica erro na compila��o do shader e na linkagem do programa
	int verifyShaderCompilation(unsigned int shaderID);
	int verifyProgramLink(unsigned int programID);
	
	unsigned int createShader(std::string shaderSource, unsigned int type);	
public:
	MyScene();

	void update() override;


	~MyScene();
};
