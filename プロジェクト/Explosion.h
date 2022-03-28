#pragma once

#include "GameObject.h"

#define MAX_EXPLOSION_ANIME_W (8)   //���A�j���[�V������(�A�j���[�V�����̊G(�摜)�̐�)
#define MAX_EXPLOSION_ANIME_H (1)   //�c�A�j���[�V������(�A�j���[�V�����̊G(�摜)�̐�)

class Explosion : public GameObject
{
private:

	ID3D11Buffer*				m_VertexBuffer = NULL;	//�����[�X����^�C�~���O�Œ��g���������Ƃ��m�F���Ă�
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