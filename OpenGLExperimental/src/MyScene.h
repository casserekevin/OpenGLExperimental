#pragma once

#include "Scene.h"
#include "Speed.h"

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
	Speed* m_speed = new Speed(1.0f, 0.2f);

	float m_posicaoAtualX = 0.0f;
	float m_posicaoAtualY = 0.0f;

	float m_topPosition;
	float m_bottomPosition;
	float m_leftPosition;
	float m_rightPosition;

	/*float m_speed = 0.25f;
	float m_lastPosition = 0.0f;*/


	//Funcoes:
	//Verifica erro na compilação do shader e na linkagem do programa
	int verifyShaderCompilation(unsigned int shaderID);
	int verifyProgramLink(unsigned int programID);
	
	unsigned int createShader(std::string shaderSource, unsigned int type);	

public:
	MyScene();

	void update() override;


	~MyScene();
};

