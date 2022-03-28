#pragma once

#define MODEL_DRONE ("asset\\model\\Drone.obj")
#define MODEL_PROPELLER ("asset\\model\\Propeller.obj")
#define MODEL_PLAYER ("asset\\model\\Player.obj")
#define MODEL_ENEMY1 ("asset\\model\\Enemy1.obj")
#define MODEL_PLAYER_LEG ("asset\\model\\Player_Leg.obj")
#define MODEL_SKY ("asset\\model\\Sky.obj")
#define MODEL_PLAYER_WEAPON1 ("asset\\model\\PlayerWeapon1.obj")
#define MODEL_PLAYER_WEAPON2 ("asset\\model\\Weapon2.obj")
#define MODEL_CUBE ("asset\\model\\DrawOBB.obj")


// マテリアル構造体
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView*	Texture;

};


// 描画サブセット構造体
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};


// モデル構造体
struct MODEL
{
	VERTEX_3D		*VertexArray;
	unsigned int	VertexNum;

	unsigned int	*IndexArray;
	unsigned int	IndexNum;

	SUBSET			*SubsetArray;
	unsigned int	SubsetNum;
};

//モデルリスト
enum MODEL_LISTNUM
{
	NUM_NONE,

	NUM_MODEL_DRONE,
	NUM_MODEL_PROPELLER,
	NUM_MODEL_PLAYER,
	NUM_MODEL_ENEMY1,
	NUM_MODEL_PLAYER_LEG,
	NUM_MODEL_SKY,
	NUM_MODEL_PLAYER_WEAPON1,
	NUM_MODEL_PLAYER_WEAPON2,
};	




class Model
{
private:

	ID3D11Buffer*	m_VertexBuffer;
	ID3D11Buffer*	m_IndexBuffer;

	SUBSET*	m_SubsetArray;
	unsigned int	m_SubsetNum;

	void LoadObj( const char *FileName, MODEL *Model );
	void LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum );

public:

	void Draw();

	void Load( const char *FileName );
	void Unload();

};