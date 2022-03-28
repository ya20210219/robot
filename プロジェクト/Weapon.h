#pragma once

//モデルリスト
enum WEAPON_TYPELIST
{
	PLAYER1,
	PLAYER2,
	ENEMY,

	TYPE_MAX,
};

class Weapon : public GameObject
{
private:

	static Model* m_Model[5];

	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

	D3DXVECTOR3 m_Dir;

	D3DXMATRIX m_WeaponMatrix;
	WEAPON_TYPELIST	m_Type;
	D3DXMATRIX m_ParentMatrix;

	bool	bUse[5];

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();

	static void Load()
	{
		m_Model[0] = new Model();
		m_Model[0]->Load(MODEL_PLAYER_WEAPON1);	//\\2個に注意「//」でもいい
		m_Model[1] = new Model();
		m_Model[1]->Load(MODEL_PLAYER_WEAPON2);	//\\2個に注意「//」でもいい
		m_Model[2] = new Model();
		m_Model[2]->Load(MODEL_PLAYER_WEAPON2);	//\\2個に注意「//」でもいい
		m_Model[3] = new Model();
		m_Model[3]->Load(MODEL_PLAYER_WEAPON2);	//\\2個に注意「//」でもいい
		m_Model[4] = new Model();
		m_Model[4]->Load(MODEL_PLAYER_WEAPON2);	//\\2個に注意「//」でもいい
	};
	static void UnLoad()
	{
		for (int i = 0; i < 5; i++)
		{
			m_Model[i]->Unload();
			delete m_Model[i];
		}
	};

	void SetBool(bool active, int index)
	{
		bUse[index] = active;
	}

	void SetParentMatrix(D3DXMATRIX mat)
	{
		m_ParentMatrix = mat;
	}

	WEAPON_TYPELIST GetType()
	{
		return m_Type;
	}

	void SetType(WEAPON_TYPELIST type)
	{
		m_Type = type;
	}
};
