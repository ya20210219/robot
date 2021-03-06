#pragma once

#include "GameObject.h"

class Fade : public GameObject
{
private:
	ID3D11Buffer*				m_VertexBuffer = NULL;	//リリースするタイミングで中身が無いことを確認してる
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;

	bool	m_FadeIn;
	bool	m_FadeOut;
	bool	m_FadeStart;
	float   m_FadeValue;
	bool    m_FadeEnd;


public:
	//Polygon2D();	//今回は不要
	//~Polygon2D();

	void Init();
	void UnInit();
	void Update();
	void Draw();

	void SetFadeIn(bool active, float val)
	{
		m_FadeIn = active;
		m_FadeValue = val;
	}

	void SetFadeOut(bool active, float val)
	{
		m_FadeOut = active;
		m_FadeValue = val;
	}

	bool GetFadeEnd()
	{
		return m_FadeEnd;
	}

	bool GetFadeStart()
	{
		return m_FadeStart;
	}

	void SetFade(bool active)
	{
		m_FadeStart = active;
		m_FadeEnd = false;
	}
};