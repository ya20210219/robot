#pragma once

#include "GameObject.h"
#include "Weapon.h"

#define BULLET_01 (D3DXVECTOR3(0.1f, 0.1f, 0.1f))
#define BULLET_02 (D3DXVECTOR3(0.3f, 0.3f, 0.3f))

//’e‚Ìƒ^ƒCƒv
enum BULLET_TYPE
{
	BULLET_PLAYER,
	BULLET_ENEMY,

	BULLET_TYPE_MAX,
};

class Bullet : public GameObject
{
private:

	static Model* m_Model;

	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

	D3DXVECTOR3 m_Dir;
	D3DXVECTOR3 m_Way;
	BULLET_TYPE	m_Type;
	int	timer;

	class OBB* m_obb;

	bool	bUse;

public:
	static void Load()
	{
		m_Model = new Model();
		m_Model->Load("asset\\model\\sphere.obj");
	};
	static void UnLoad()
	{
		m_Model->Unload();
		delete m_Model;
	};
	void Init();
	void UnInit();
	void Update();
	void Draw();

	void SetWay(D3DXVECTOR3 way)
	{
		m_Way = way;
	}

	void SetBulletAll(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, D3DXVECTOR3 spd, float r, BULLET_TYPE type)
	{
		m_Position = pos;
		m_Rotation = rot;
		m_Scale = scl;
		m_Speed = spd;
		m_Radius = r;
		m_Type = type;
	}

};