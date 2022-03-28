#pragma once

#include "GameObject.h"
#define GRAVITY (0.095f)

class Field : public GameObject
{
private:

	ID3D11Buffer*				m_VertexBuffer = NULL;	//�����[�X����^�C�~���O�Œ��g���������Ƃ��m�F���Ă�
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;

	float	m_GravityValue;
	float	m_GravityActive;
public:

	void Init();
	void UnInit();
	void Update();
	void Draw();

	bool GetGravityActive()
	{
		return m_GravityActive;
	}

	float GetGravityValue()
	{
		return m_GravityValue;
	}

	void SetGravityValue(float value)
	{
		m_GravityValue = value;
	}
};