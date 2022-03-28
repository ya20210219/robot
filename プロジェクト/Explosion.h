#pragma once

#include "GameObject.h"

#define MAX_EXPLOSION_ANIME_W (8)   //横アニメーション数(アニメーションの絵(画像)の数)
#define MAX_EXPLOSION_ANIME_H (1)   //縦アニメーション数(アニメーションの絵(画像)の数)

class Explosion : public GameObject
{
private:

	ID3D11Buffer*				m_VertexBuffer = NULL;	//リリースするタイミングで中身が無いことを確認してる
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;

	int	m_Cnt;
	int m_Frame;
public:

	void Init();
	void UnInit();
	void Update();
	void Draw();
};