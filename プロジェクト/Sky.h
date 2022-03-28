#pragma once

#include "GameObject.h"

class Sky : public GameObject
{
private:

	Model* m_Model;
	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

	D3DXMATRIX m_SkyMatrix;

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();
};
