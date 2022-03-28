#pragma once

#include "GameObject.h"
#include "Score.h"

#define MAX_ENEMY_HP (10)
#define MAX_MYSELF_TO_TARGET_DIST (10.0f)
#define MIN_MYSELF_TO_TARGET_DIST (5.0f)

enum TARGET_TYPE
{
	NUM_TARGET_NONE,

	NUM_TARGET_PLAYER,
	NUM_TARGET_DRONE,
};


class Enemy : public GameObject
{
private:

	static Model* m_Model;
	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

	float	m_EnemySpd;

	int     m_HP = 10;

	class Weapon* m_EnemyWeapon01;
	class Weapon* m_EnemyWeapon02;

	class HPGauge* m_HPGauge;

	float m_TargetY;

	TARGET_TYPE m_Target;

	bool  m_BulletFireFlg;
	int	  m_BulletCoolDownCnt;
	int   m_ReTargetCnt;
	float m_EnemyToDrone;
	float m_EnemyToPlayer;
	float m_MinDist;
	float m_MaxDist;
	bool  m_Inversion;

	float m_Dist;

	D3DXVECTOR3 m_TargetPos;

	class OBB* m_obb;

	D3DXMATRIX m_EnemyMatrix;

	float m_Spd = 1;

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();

	static void Load()
	{
		m_Model = new Model();
		m_Model->Load(MODEL_ENEMY1);	//\\2ŒÂ‚É’ˆÓu//v‚Å‚à‚¢‚¢
	};
	static void UnLoad()
	{
		m_Model->Unload();
		delete m_Model;
	};

	void SetEnemySpd(float x, float y, float z)
	{
		m_Speed.x = x;
		m_Speed.y = y;
		m_Speed.z = z;
	}

	OBB* GetOBB()
	{
		return m_obb;
	}

	D3DXMATRIX GetEnemyMatrix()
	{
		return m_EnemyMatrix;
	}

	void SetHitHP()
	{
		m_HP = m_HP -= 1;
		if (m_HP < 0)
		{
			Scene* scene = Manager::GetScene();
			Score* score = scene->GetGameObject<Score>(OBJ2D);
			SetDestroy();
			score->AddScore();
		}
	}
};

