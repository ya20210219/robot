#pragma once

#include "GameObject.h"

class Drone : public GameObject
{
private:

	Model* m_Model;
	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

	D3DXVECTOR3 m_Speed;
	float	m_DroneSpd;
	bool	m_Active;
	int     m_ActiveCnt;


	class OBB* m_obb;
	class HPGauge* m_HPGauge;
	class Enemy* m_TargetEnemy;
	float m_TargetDist;
	float m_TargetRotY;

	bool  m_BulletFireFlg;
	int	  m_BulletCoolDownCnt;
	int   m_ReTargetCnt;
	int   m_HP;

	D3DXMATRIX m_DroneMatrix;

	float m_Spd = 1;

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();

	class Propeller
	{
	public:
		static Model* m_Model;

		D3DXVECTOR3 Pos;
		D3DXVECTOR3 Rot;
		D3DXVECTOR3 Scl;

		D3DXMATRIX m_PropellerMatrix;

		void Draw(int index, D3DMATRIX matrix);

		static void Load()
		{
			m_Model = new Model();
			m_Model->Load(MODEL_PROPELLER);
		};
		static void UnLoad()
		{
			m_Model->Unload();
			delete m_Model;
		};
	};

	class puropera
	{
	public:
		Model* model;
	};

	bool GetActive()
	{
		return m_Active;
	}

	void SetActive(bool enable)
	{
		m_Active = enable;
	}

	D3DXMATRIX GetDroneMatrix()
	{
		return m_DroneMatrix;
	}

	int GetHP()
	{
		return m_HP;
	}

	void SetHP(int num)
	{
		m_HP = num;
	}

	void SetHitHP()
	{
		m_HP = m_HP -= 1;
		if (m_HP < 0)
		{
			SetActive(false);
		}
	}
	OBB* GetOBB()
	{
		return m_obb;
	}
};
