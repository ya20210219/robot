#pragma once

#include "GameObject.h"

#define WALK_SPEED_FORWARD (0.4f)
#define WALK_SPEED_SIDE (0.25f)
#define JUMP_VALUE (0.25f)
#define BULLET1_SPEED (1.0f)
#define BULLET2_SPEED (0.25f)

class Player : public GameObject
{
private: 

	Model* m_Model;
	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

	float	m_JumpPower;
	bool	m_JumpActive;
	float	m_PlayerSpd;
	bool	m_Active;
	int     m_ActiveCnt;
	bool	m_WeaponActive;
	int		m_WeaponTypeNum;
	bool    m_WeaponChenged;
	bool    m_Dash;
	float   m_DashPower;

	int     m_HP;

	float	m_DirAngle;

	class Audio* m_ShotSE;

	class Shadow* m_Shadow;

	class Weapon* m_PlayerWeapon;

	class HPGauge* m_HPGauge;

	class OBB* m_obb;

	float m_Spd = 1;

	D3DXMATRIX m_PlayerMatrix;

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();

	class PlayerLeg
	{
	public:
		static Model* m_Model;

		D3DXVECTOR3 Pos;
		D3DXVECTOR3 Rot;
		D3DXVECTOR3 Scl;

		float		m_WalkValueForward;
		float		m_WalkValueSide;

		D3DXMATRIX m_PlayerLegMatrix;
		bool	   m_Right;

		void Draw(int index, D3DMATRIX matrix);

		static void Load()
		{
			m_Model = new Model();
			m_Model->Load(MODEL_PLAYER_LEG);
		};
		static void UnLoad()
		{
			m_Model->Unload();
			delete m_Model;
		};
	};

	/*class PlayerWeapon
	{
	public:
		static Model* m_Model1;
		static Model* m_Model2;

		D3DXVECTOR3 Pos;
		D3DXVECTOR3 Rot;
		D3DXVECTOR3 Scl;


		D3DXMATRIX m_PlayerWeaponMatrix;
		bool	   m_Right;

		void Draw(D3DMATRIX matrix, int num);

		static void Load()
		{
			m_Model1 = new Model();
			m_Model1->Load(MODEL_PLAYER_WEAPON1);
			m_Model2 = new Model();
			m_Model2->Load(MODEL_PLAYER_WEAPON2);
		};
		static void UnLoad()
		{
			m_Model2->Unload();
			delete m_Model2;
			m_Model1->Unload();
			delete m_Model1;
		};

		void SetWRot(D3DXVECTOR3 rot)
		{
			Rot = rot;
		}
	};*/

	bool GetActive()
	{
		return m_Active;
	}

	bool GetWeaponActive()
	{
		return m_WeaponActive;
	}

	float GetPlayerSpeed()
	{
		return m_PlayerSpd;
	}

	void SetActive(bool enable)
	{
		m_Active = enable;
	}

	int GetWeaponTypeNum(int num)
	{
		return m_WeaponTypeNum;
	}

	D3DXMATRIX GetPlayerMatrix()
	{
		return m_PlayerMatrix;
	}

	Weapon* GetPW()
	{
		return m_PlayerWeapon;
	}

	float SetBulletSpeed(int num)
	{
		if (num == 1)
		{
			return BULLET1_SPEED;
		}
		if (num == 2)
		{
			return BULLET2_SPEED;
		}
	}

	void SetHitHP()
	{
		m_HP = m_HP -= 1;
		if (m_HP < 0)
		{
			m_Active = false;
		}
	}

	OBB* GetOBB()
	{
		return m_obb;
	}
};