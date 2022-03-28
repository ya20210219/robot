#pragma once

#include "GameObject.h"

#define SCORE_WIDTH (30)
#define SCORE_HEIGHT (30)
#define MAX_SCORE_TEX_WIDTH (5)
#define MAX_SCORE_TEX_HEIGHT (2)
#define MAX_SCORE_DIGIT (6)

class Score : public GameObject
{
private:
	ID3D11Buffer*				m_VertexBuffer = NULL;	//�����[�X����^�C�~���O�Œ��g���������Ƃ��m�F���Ă�
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;

	int m_Score;

public:
	//Polygon2D();	//����͕s�v
	//~Polygon2D();

	void Init();
	void UnInit();
	void Update();
	void Draw();

	int GetScore()
	{
		return m_Score;
	}

	void SetScore(int value)
	{
		m_Score = value;
	}

	void AddScore()
	{
		m_Score = m_Score + 100;
	}
};