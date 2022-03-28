#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "Scene.h"
#include "model.h"
#include "Player.h"
#include "Drone.h"
#include "Input.h"
#include "GameObject.h"
#include "Bullet.h"
#include "Camera.h"
#include "Field.h"
#include "Calculation.h"
#include "Shadow.h"
#include "Enemy.h"
#include "Weapon.h"
#include "audio.h"
#include "HPGauge.h"
#include "OBB.h"
#include <vector>

Player::PlayerLeg playerleg[2];
//Player::PlayerWeapon playerweapon;

Model* Player::PlayerLeg::m_Model;	//staticメンバ変数はcpp側で再度宣言する必要がある
//Model* Player::PlayerWeapon::m_Model1;	//staticメンバ変数はcpp側で再度宣言する必要がある
//Model* Player::PlayerWeapon::m_Model2;	//staticメンバ変数はcpp側で再度宣言する必要がある

void Player::Init()
{
	m_Model = new Model();
	m_Model->Load(MODEL_PLAYER);	//\\2個に注意「//」でもいい

	m_Position	= D3DXVECTOR3(0.0f, 0.75f, -5.0f);
	m_Rotation	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale		= D3DXVECTOR3(0.25f, 0.25f, 0.25f);
	m_ActiveCnt = 0;
	m_JumpPower = 0.0f;
	m_JumpActive = false;
	m_WeaponTypeNum = 1;
	m_WeaponChenged = false;
	m_HP = 10;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

	m_PlayerSpd = 0.0f;
	m_Active = true;
	m_WeaponActive = false;

	playerleg[0].Pos = D3DXVECTOR3(m_Position.x - 0.15f, m_Position.y - 1.0f, m_Position.z + 5.5f);
	playerleg[0].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);										 
	playerleg[0].Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);										 
	playerleg[0].m_Right = true;															 
	playerleg[0].m_WalkValueForward = 0.0f;													 
	playerleg[0].m_WalkValueSide = 0.0f;													 
																							 
																							 
	playerleg[1].Pos = D3DXVECTOR3(m_Position.x + 0.15f, m_Position.y - 1.0f, m_Position.z + 5.5f);
	playerleg[1].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerleg[1].Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerleg[1].m_Right = false;
	playerleg[1].m_WalkValueForward = 0.0f;
	playerleg[1].m_WalkValueSide = 0.0f;

	//playerweapon.Pos = D3DXVECTOR3(m_Position.x - 0.15f, m_Position.y - 0.5f, m_Position.z + 3.25f);
	//playerweapon.Rot = D3DXVECTOR3(0.0f, m_Rotation.y + D3DX_PI, 0.0f);
	//playerweapon.Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//playerweapon.m_Right = true;

	Scene* scene = Manager::GetScene();
	m_Shadow = scene->AddGameObject<Shadow>(OBJ3D);
	m_Shadow->SetPos(m_Position.x, m_Position.y - 1.0f, m_Position.z);
	m_Shadow->SetScale(2.0f, 1.0f, 2.0f);

	m_ShotSE = scene->AddGameObject<Audio>(OBJ2D);
	m_ShotSE->Load("asset\\audio\\shoot1.wav");

	m_HPGauge = scene->AddGameObject<HPGauge>(OBJ2D);
	m_HPGauge->SetPos(m_Position.x - (m_HP / 2), m_Position.y + 1.0f, m_Position.z);
	m_HPGauge->SetHPGaugeSize(m_HP);

	m_PlayerWeapon = scene->AddGameObject<Weapon>(OBJ3D);
	m_PlayerWeapon->SetAll(D3DXVECTOR3(m_Position.x, m_Position.y + 2.0f, m_Position.z + 6.0f),
		D3DXVECTOR3(-D3DX_PI / 2, m_Rotation.y, 0.0f),	//rot
		D3DXVECTOR3(1.0f, 1.0f, 1.0f),	//scl
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		0.0f);	//radius

	m_PlayerWeapon->SetBool(true, PLAYER1);
	m_PlayerWeapon->SetType(PLAYER1);

	m_obb = scene->AddGameObject<OBB>(OBJ3D);
	m_obb->SetOBBPos(D3DXVECTOR3(m_Position.x, m_Position.y + 0.25f, m_Position.z));
	m_obb->SetDirect(0, GetRight());
	m_obb->SetDirect(1, GetUp());
	m_obb->SetDirect(2, GetForward(m_Rotation));
	m_obb->SetLen_W(0, 0.75f);
	m_obb->SetLen_W(1, 0.8f);
	m_obb->SetLen_W(2, 0.4f);

	m_obb->SetAll(D3DXVECTOR3(m_obb->GetPos_W().x, m_obb->GetPos_W().y, m_obb->GetPos_W().z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_obb->GetLen_W(0), m_obb->GetLen_W(1), m_obb->GetLen_W(2)),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		0.0f);
}

void Player::UnInit()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Player::Update()
{
	Scene* scene = Manager::GetScene();
	Drone* drone = scene->GetGameObject<Drone>(OBJ3D);
	Camera* cam = scene->GetGameObject<Camera>(SYSTEM);
	Field* field = scene->GetGameObject<Field>(OBJ3D);
	Weapon* weapon = scene->GetGameObject<Weapon>(OBJ3D);
	
	//特定のキーを押したらドローンへ切り替えするカウント
	if (Keyboard_IsPress(DIK_Q) && drone->GetActive() == false)
	{
		Player::m_ActiveCnt++;
	}

	//キーを離したらドローン切り替えカウント初期化
	if (Keyboard_IsRelease(DIK_Q))
	{
		Player::m_ActiveCnt = 0;
	}

	//ドローンへの切り替えが特定の数値を超えたらドローンに切り替え。プレイヤーを使用できなくする
	if (m_ActiveCnt > 20)
	{
		drone->SetActive(true);
		drone->SetPos(m_Position.x + sinf(m_Rotation.y + D3DX_PI * 0.5f) * 1, m_Position.y, m_Position.z + cosf(m_Rotation.y + D3DX_PI * 0.5f) * 1);
		//m_Active = false;
		m_ActiveCnt = 0;
	}

	//今プレイヤーが使用できる状態の場合
	if (m_Active == true)
	{
		//OBB更新
		std::vector<Enemy*> EnemyList = scene->GetGameObjects<Enemy>(OBJ3D);
		m_obb->SetOBBPos(D3DXVECTOR3(m_Position.x, m_Position.y + 0.25f, m_Position.z));
		m_obb->SetDirect(0, GetRight());
		m_obb->SetDirect(1, GetUp());
		m_obb->SetDirect(2, GetForward(m_Rotation));
		m_obb->SetPos(m_obb->GetPos_W().x, m_obb->GetPos_W().y, m_obb->GetPos_W().z);
		m_obb->SetRot(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		m_HPGauge->SetPos(m_Position.x - 0.5f, m_Position.y + 1.0f, m_Position.z);
		m_HPGauge->SetHPGaugeSize(m_HP);

		//各種数値チェック用
		//==============================================================
		char str1[50];
		sprintf(str1, "あたってるよん");
		char str2[50];
		sprintf(str2, "あたってないよん");
		bool h = false;

		for (Enemy* enemy : EnemyList)
		{
			if (m_obb->ColOBBs(*m_obb, *enemy->GetOBB()))
			{
				//SetWindowText(GetWindow(), str1);
				h = true;
			}
		}

		if (h == false)
		{
			//SetWindowText(GetWindow(), str2);
		}

		//==============================================================
		m_Shadow->SetPos(m_Position.x, 0.001f, m_Position.z);
		m_Shadow->SetScale(1.5f - ((m_Position.y - 1.0f) / 2), 1.0f, 1.5f - ((m_Position.y - 1.0f) / 2));

		//武器の種類を変える
		if (Keyboard_IsTrigger(DIK_1))
		{
			for (int i = 0; i < 5; i++)
			{
				m_PlayerWeapon->SetBool(false, i);
			}
			m_PlayerWeapon->SetBool(true, 0);
			m_WeaponTypeNum = 1;
			//m_WeaponChenged = true;
		}

		if (Keyboard_IsTrigger(DIK_2))
		{
			m_WeaponTypeNum = 2;
			for (int i = 0; i < 5; i++)
			{
				m_PlayerWeapon->SetBool(false, i);
			}
			m_PlayerWeapon->SetBool(true, 1);
			//m_WeaponChenged = true;
		}

		//武器を構える(武器の位置、角度を調整している)
		if (Keyboard_IsTrigger(DIK_LSHIFT))
		{
			m_WeaponActive = true;
			m_PlayerWeapon->SetPos(0.0f, 0.75f, -1.0f);
			m_PlayerWeapon->SetRot(D3DX_PI / 2, 0.0f, 0.0f);
		}

		//武器をしまう
		if (Keyboard_IsRelease(DIK_LSHIFT) || m_WeaponChenged == true)
		{
			m_WeaponActive = false;
			m_PlayerWeapon->SetPos(0.0f, 3.0f, 1.0f);
			m_PlayerWeapon->SetRot(-D3DX_PI / 2, 0.0f, 0.0f);
			m_WeaponChenged = false;
		}

		//マトリクスの更新
		m_PlayerWeapon->SetParentMatrix(m_PlayerMatrix);

		//前進キーが入力された場合
		if (Keyboard_IsPress(DIK_W))
		{
			playerleg[0].m_WalkValueForward += 0.1f;
			playerleg[0].m_WalkValueForward = AdjustmentRadian(playerleg[0].m_WalkValueForward);

			playerleg[1].m_WalkValueForward = playerleg[0].m_WalkValueForward + D3DX_PI;
			playerleg[1].m_WalkValueForward = AdjustmentRadian(playerleg[1].m_WalkValueForward);

			playerleg[0].Pos = D3DXVECTOR3(playerleg[0].Pos.x, cosf(playerleg[0].m_WalkValueForward) * WALK_SPEED_FORWARD, sinf(playerleg[0].m_WalkValueForward) * WALK_SPEED_FORWARD/* - 0.15f*/);
			playerleg[1].Pos = D3DXVECTOR3(playerleg[1].Pos.x, cosf(playerleg[1].m_WalkValueForward) * WALK_SPEED_FORWARD, sinf(playerleg[1].m_WalkValueForward) * WALK_SPEED_FORWARD/* - 0.15f*/);

			m_PlayerSpd = 0.05f;

			m_DirAngle = 0.0f + cam->GetRot().y;

			m_Speed.x = sinf(D3DX_PI * 0.0f + -cam->GetRot().y) * m_PlayerSpd;
			m_Speed.z = cosf(D3DX_PI * 0.0f + -cam->GetRot().y) * m_PlayerSpd;

			if (Keyboard_IsPress(DIK_A))
			{
				m_DirAngle = D3DX_PI * -0.25f + cam->GetRot().y;
				m_Speed.x = sinf(D3DX_PI * 0.25f + -cam->GetRot().y) * m_PlayerSpd;
				m_Speed.z = cosf(D3DX_PI * 0.25f + -cam->GetRot().y) * m_PlayerSpd;
			}

			if (Keyboard_IsPress(DIK_D))
			{
				m_DirAngle = D3DX_PI * 0.25f + cam->GetRot().y;
				m_Speed.x = sinf(D3DX_PI * -0.25f + -cam->GetRot().y) * m_PlayerSpd;
				m_Speed.z = cosf(D3DX_PI * -0.25f + -cam->GetRot().y) * m_PlayerSpd;
			}

			m_Rotation.y = (float)LerpRadian(m_Rotation.y, m_DirAngle, 0.02f);
		}

		if (Keyboard_IsPress(DIK_S))
		{
			playerleg[0].m_WalkValueForward += 0.1f;
			playerleg[0].m_WalkValueForward = AdjustmentRadian(playerleg[0].m_WalkValueForward);

			playerleg[1].m_WalkValueForward = playerleg[0].m_WalkValueForward + D3DX_PI;
			playerleg[1].m_WalkValueForward = AdjustmentRadian(playerleg[1].m_WalkValueForward);

			playerleg[0].Pos = D3DXVECTOR3(playerleg[0].Pos.x, cosf(playerleg[0].m_WalkValueForward) * WALK_SPEED_FORWARD, sinf(playerleg[0].m_WalkValueForward) * WALK_SPEED_FORWARD/* - 0.15f*/);
			playerleg[1].Pos = D3DXVECTOR3(playerleg[1].Pos.x, cosf(playerleg[1].m_WalkValueForward) * WALK_SPEED_FORWARD, sinf(playerleg[1].m_WalkValueForward) * WALK_SPEED_FORWARD/* - 0.15f*/);

			m_PlayerSpd = -0.05f;

			if (m_WeaponActive == false)
			{
				m_DirAngle = D3DX_PI + cam->GetRot().y;
			}

			m_Speed.x = sinf(D3DX_PI * 0.0f + -cam->GetRot().y) * m_PlayerSpd;
			m_Speed.z = cosf(D3DX_PI * 0.0f + -cam->GetRot().y) * m_PlayerSpd;

			if (Keyboard_IsPress(DIK_A))
			{
				if (m_WeaponActive == false)
				{
					m_DirAngle = D3DX_PI * -0.75f + cam->GetRot().y;
				}

				m_Speed.x = sinf(D3DX_PI * -0.25f + -cam->GetRot().y) * m_PlayerSpd;
				m_Speed.z = cosf(D3DX_PI * -0.25f + -cam->GetRot().y) * m_PlayerSpd;
			}

			if (Keyboard_IsPress(DIK_D))
			{
				if (m_WeaponActive == false)
				{
					m_DirAngle = D3DX_PI * 0.75f + cam->GetRot().y;
				}

				m_Speed.x = sinf(D3DX_PI * 0.25f + -cam->GetRot().y) * m_PlayerSpd;
				m_Speed.z = cosf(D3DX_PI * 0.25f + -cam->GetRot().y) * m_PlayerSpd;
			}

			if (m_WeaponActive == false)
			{
				m_Rotation.y = (float)LerpRadian(m_Rotation.y, m_DirAngle, 0.02f);
			}
		}

		if (Keyboard_IsPress(DIK_A) && !Keyboard_IsPress(DIK_W) && !Keyboard_IsPress(DIK_S))
		{
			m_PlayerSpd = 0.05f;

			if (m_WeaponActive == false)
			{
				playerleg[0].m_WalkValueForward += 0.1f;
				playerleg[0].m_WalkValueForward = AdjustmentRadian(playerleg[0].m_WalkValueForward);

				playerleg[1].m_WalkValueForward = playerleg[0].m_WalkValueForward + D3DX_PI;
				playerleg[1].m_WalkValueForward = AdjustmentRadian(playerleg[1].m_WalkValueForward);

				playerleg[0].Pos = D3DXVECTOR3(playerleg[0].Pos.x, cosf(playerleg[0].m_WalkValueForward) * WALK_SPEED_FORWARD, sinf(playerleg[0].m_WalkValueForward) * WALK_SPEED_FORWARD/* - 0.125f*/);
				playerleg[1].Pos = D3DXVECTOR3(playerleg[1].Pos.x, cosf(playerleg[1].m_WalkValueForward) * WALK_SPEED_FORWARD, sinf(playerleg[1].m_WalkValueForward) * WALK_SPEED_FORWARD/* - 0.125f*/);

				m_DirAngle = D3DX_PI * -0.5f + cam->GetRot().y;
				m_Rotation.y = (float)LerpRadian(m_Rotation.y, m_DirAngle, 0.02f);

				m_Speed.x = sinf(D3DX_PI * 0.5f + -cam->GetRot().y) * m_PlayerSpd;
				m_Speed.z = cosf(D3DX_PI * 0.5f + -cam->GetRot().y) * m_PlayerSpd;
			}

			if (m_WeaponActive == true)
			{
				playerleg[0].m_WalkValueSide += 0.1f;
				playerleg[0].m_WalkValueSide = AdjustmentRadian(playerleg[0].m_WalkValueSide);

				playerleg[1].m_WalkValueSide = playerleg[0].m_WalkValueSide + D3DX_PI;
				playerleg[1].m_WalkValueSide = AdjustmentRadian(playerleg[1].m_WalkValueSide);

				playerleg[0].Pos = D3DXVECTOR3(cosf(playerleg[0].m_WalkValueSide) * WALK_SPEED_SIDE, sinf(playerleg[0].m_WalkValueSide) * WALK_SPEED_SIDE, playerleg[0].Pos.z);
				playerleg[1].Pos = D3DXVECTOR3(cosf(playerleg[1].m_WalkValueSide) * WALK_SPEED_SIDE, sinf(playerleg[1].m_WalkValueSide) * WALK_SPEED_SIDE, playerleg[1].Pos.z);

				m_Speed.x = sinf(D3DX_PI * 0.5f - m_Rotation.y) * m_PlayerSpd;
				m_Speed.z = cosf(D3DX_PI * 0.5f - m_Rotation.y) * m_PlayerSpd;
			}
		}

		if (Keyboard_IsPress(DIK_D) && !Keyboard_IsPress(DIK_W) && !Keyboard_IsPress(DIK_S))
		{
			m_PlayerSpd = -0.05f;

			if (m_WeaponActive == false)
			{
				playerleg[0].m_WalkValueForward += 0.1f;
				playerleg[0].m_WalkValueForward = AdjustmentRadian(playerleg[0].m_WalkValueForward);

				playerleg[1].m_WalkValueForward = playerleg[0].m_WalkValueForward + D3DX_PI;
				playerleg[1].m_WalkValueForward = AdjustmentRadian(playerleg[1].m_WalkValueForward);

				playerleg[0].Pos = D3DXVECTOR3(playerleg[0].Pos.x, cosf(playerleg[0].m_WalkValueForward) * WALK_SPEED_FORWARD, sinf(playerleg[0].m_WalkValueForward) * WALK_SPEED_FORWARD/* - 0.125f*/);
				playerleg[1].Pos = D3DXVECTOR3(playerleg[1].Pos.x, cosf(playerleg[1].m_WalkValueForward) * WALK_SPEED_FORWARD, sinf(playerleg[1].m_WalkValueForward) * WALK_SPEED_FORWARD/* - 0.125f*/);

				m_DirAngle = D3DX_PI * 0.5f + cam->GetRot().y;
				m_Rotation.y = (float)LerpRadian(m_Rotation.y, m_DirAngle, 0.02f);

				m_Speed.x = sinf(D3DX_PI * 0.5f + -cam->GetRot().y) * m_PlayerSpd;
				m_Speed.z = cosf(D3DX_PI * 0.5f + -cam->GetRot().y) * m_PlayerSpd;
			}

			if (m_WeaponActive == true)
			{
				playerleg[0].m_WalkValueSide -= 0.1f;
				playerleg[0].m_WalkValueSide = AdjustmentRadian(playerleg[0].m_WalkValueSide);
				
				playerleg[1].m_WalkValueSide = playerleg[0].m_WalkValueSide - D3DX_PI;
				playerleg[1].m_WalkValueSide = AdjustmentRadian(playerleg[1].m_WalkValueSide);

				playerleg[0].Pos = D3DXVECTOR3(cosf(playerleg[0].m_WalkValueSide) * WALK_SPEED_SIDE, sinf(playerleg[0].m_WalkValueSide) * WALK_SPEED_SIDE, playerleg[0].Pos.z);
				playerleg[1].Pos = D3DXVECTOR3(cosf(playerleg[1].m_WalkValueSide) * WALK_SPEED_SIDE, sinf(playerleg[1].m_WalkValueSide) * WALK_SPEED_SIDE, playerleg[1].Pos.z);

				m_Speed.x = sinf(D3DX_PI * 0.5f - m_Rotation.y) * m_PlayerSpd;
				m_Speed.z = cosf(D3DX_PI * 0.5f - m_Rotation.y) * m_PlayerSpd;
			}
		}

		//ジャンプキーが入力されたら
		if (Keyboard_IsTrigger(DIK_LCONTROL) && m_JumpActive == false)
		{
			m_JumpPower = JUMP_VALUE;
			m_JumpActive = true;
		}

		//ジャンプ中なら
		if (m_JumpActive == true)
		{
			m_Position.y += m_JumpPower;
			m_JumpPower *= 0.9f;
			if (m_Position.y < field->GetPos().y + m_obb->GetLen_W(1))
			{
				m_JumpPower = 0.0f;
				m_Position.y = field->GetPos().y + m_obb->GetLen_W(1);
				field->SetGravityValue(0.0f);
				m_JumpActive = false;
			}
		}

		//床より上であれば重力の影響を受ける
		if (m_Position.y > field->GetPos().y + m_obb->GetLen_W(1))
		{
			field->SetGravityValue(GRAVITY);
			m_Position.y -= field->GetGravityValue();
			field->SetGravityValue(field->GetGravityValue() * 1.05f);
		}

		//入力が無い場合、両足を所定位置(初期位置)まで移動させる		
		if (!Keyboard_IsPress(DIK_W) && !Keyboard_IsPress(DIK_S) && !Keyboard_IsPress(DIK_A) && !Keyboard_IsPress(DIK_D))	
		{
			D3DXVec3Lerp(&playerleg[0].Pos, 
				         &playerleg[0].Pos, 
				         &D3DXVECTOR3(-0.15f, 0.0f, 0.0f),
				         0.01f);
			D3DXVec3Lerp(&playerleg[1].Pos,
				         &playerleg[1].Pos,
				         &D3DXVECTOR3(0.15f, 0.0f, 0.0f),
				         0.01f);
		}		

		Player::m_Position.x -= Player::m_Speed.x;
		Player::m_Position.z += Player::m_Speed.z;

		if (Keyboard_IsRelease(DIK_W))
		{
			m_PlayerSpd = 0.0f;
		}

		if (Keyboard_IsRelease(DIK_S))
		{
			m_PlayerSpd = 0.0f;
		}

		if (Keyboard_IsRelease(DIK_A))
		{
			m_PlayerSpd = 0.0f;
		}

		if (Keyboard_IsRelease(DIK_D))
		{
			m_PlayerSpd = 0.0f;
		}

		//弾生成(攻撃)
		if (m_WeaponActive == true && Mouse_IsLeftTrigger())
		{
			cam->SetRot(cam->GetRot().x, m_Rotation.y, cam->GetRot().z);

			D3DXVECTOR3 a;
			a = m_PlayerWeapon->GetForward(D3DXVECTOR3(m_PlayerWeapon->GetRot().x, m_Rotation.y, m_PlayerWeapon->GetRot().z));

			scene->AddGameObject<Bullet>(OBJ3D)->SetBulletAll(D3DXVECTOR3(m_Position.x, m_Position.y + 1.0f, m_Position.z),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),	//rot
				BULLET_02,
				D3DXVECTOR3(a.x, a.y, a.z),	//spd
				0.2f,
				BULLET_PLAYER);	//radius	

			scene->GetGameObject<Bullet>(OBJ3D)->SetWay(GetForward(GetRot()));

			m_ShotSE->Play(false);
		}

		//座標の線形補間テスト
		if (Keyboard_IsPress(DIK_V))
		{
			D3DXVec3Lerp(&m_Position, &m_Position, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.01f);
		}

		if (Keyboard_IsPress(DIK_C))
		{
			//カメラ反転
			cam->SetPos(cam->GetTargetPos().x + sinf(Player::m_Rotation.y + D3DX_PI) * cam->GetCameraDist(),
						cam->GetTargetPos().y,
						cam->GetTargetPos().z + cosf(Player::m_Rotation.y + D3DX_PI) * cam->GetCameraDist());
		}

		//速度減衰
		m_Speed *= 0.9f;
	}

}

void Player::Draw()
{	
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z); //Yaw = y Pitch = x Roll = z
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	m_PlayerMatrix = scale * rot * trans;

	Renderer::SetWorldMatrix(&m_PlayerMatrix);

	m_Model->Draw();

	for (int i = 0; i < 2; i++)
	{
		playerleg[i].Draw(i, m_PlayerMatrix);
	}
}

void Player::PlayerLeg::Draw(int index, D3DMATRIX matrix)
{
	D3DXMATRIX scale, rot, trans, DMatrix;

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);

	if (playerleg[index].m_Right == true)
	{
		//マトリクス設定
		D3DXMatrixScaling(&scale, playerleg[index].Scl.x, playerleg[index].Scl.y, playerleg[index].Scl.z);
		D3DXMatrixRotationYawPitchRoll(&rot, 0.0f, playerleg[index].Rot.x, playerleg[index].Rot.z); //Yaw = y Pitch = x Roll = z
		D3DXMatrixTranslation(&trans, playerleg[index].Pos.x, playerleg[index].Pos.y, playerleg[index].Pos.z - 0.25f);

		playerleg[index].m_PlayerLegMatrix = scale * rot * trans * matrix;

		Renderer::SetWorldMatrix(&playerleg[index].m_PlayerLegMatrix);

		playerleg[index].m_Model->Draw();
	}

	if (playerleg[index].m_Right == false)
	{
		//マトリクス設定
		D3DXMatrixScaling(&scale, playerleg[index].Scl.x, playerleg[index].Scl.y, playerleg[index].Scl.z);
		D3DXMatrixRotationYawPitchRoll(&rot, D3DX_PI, playerleg[index].Rot.x, playerleg[index].Rot.z); //Yaw = y Pitch = x Roll = z
		D3DXMatrixTranslation(&trans, playerleg[index].Pos.x, playerleg[index].Pos.y, playerleg[index].Pos.z - 0.25f);

		playerleg[index].m_PlayerLegMatrix = scale * rot * trans * matrix;

		Renderer::SetWorldMatrix(&playerleg[index].m_PlayerLegMatrix);

		playerleg[index].m_Model->Draw();
	}
}

/*
void Player::PlayerWeapon::Draw(D3DMATRIX matrix, int num)
{
	D3DXMATRIX scale, rot, trans, DMatrix;

		//マトリクス設定
		D3DXMatrixScaling(&scale, playerweapon.Scl.x, playerweapon.Scl.y, playerweapon.Scl.z);
		D3DXMatrixRotationYawPitchRoll(&rot, playerweapon.Rot.y, playerweapon.Rot.x, playerweapon.Rot.z); //Yaw = y Pitch = x Roll = z
		D3DXMatrixTranslation(&trans, playerweapon.Pos.x, playerweapon.Pos.y, playerweapon.Pos.z);

		playerweapon.m_PlayerWeaponMatrix = scale * rot * trans * matrix;

		Renderer::SetWorldMatrix(&playerweapon.m_PlayerWeaponMatrix);

		if (num == 1)
		{
			playerweapon.m_Model1->Draw();
		}
		if (num == 2)
		{
			playerweapon.m_Model2->Draw();
		}
}
*/