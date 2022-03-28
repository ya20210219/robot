#include "main.h"
#include "renderer.h"
#include "Scene.h"
#include "Manager.h"
#include "model.h"
#include "Enemy.h"
#include "Player.h"
#include "Drone.h"
#include "Input.h"
#include "GameObject.h"
#include "Shadow.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Camera.h"
#include "Calculation.h"
#include "HPGauge.h"
#include "OBB.h"

Model* Enemy::m_Model;	//staticメンバ変数はcpp側で再度宣言する必要がある

void Enemy::Init()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_Scale = D3DXVECTOR3(0.25f, 0.25f, 0.25f);
	m_Radius = 0.1f;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

	m_EnemySpd = 0.025f;
	
	m_EnemyWeapon01 = scene->AddGameObject<Weapon>(OBJ3D);
	m_EnemyWeapon01->SetAll(D3DXVECTOR3(m_Position.x - 1.25f, m_Position.y - 1.5f, m_Position.z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	//rot
		D3DXVECTOR3(1.0f, 1.0f, 1.0f),	//scl
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		0.0f);	//radius

	m_EnemyWeapon01->SetBool(true, ENEMY);
	m_EnemyWeapon01->SetType(ENEMY);

	m_EnemyWeapon02 = scene->AddGameObject<Weapon>(OBJ3D);
	m_EnemyWeapon02->SetAll(D3DXVECTOR3(m_Position.x + 1.25f, m_Position.y - 1.5f, m_Position.z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	//rot
		D3DXVECTOR3(-1.0f, 1.0f, 1.0f),	//scl
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		0.0f);	//radius

	m_EnemyWeapon02->SetBool(true, ENEMY);
	m_EnemyWeapon02->SetType(ENEMY);

	m_HP = 10;
	m_HPGauge = scene->AddGameObject<HPGauge>(OBJ2D);
	m_HPGauge->SetPos(m_Position.x - (m_HP / 2), m_Position.y + 1.0f, m_Position.z);
	m_HPGauge->SetScale(0.1f, 0.1f, 0.1f);
	m_HPGauge->SetHPGaugeSize(0);
	
	m_obb = scene->AddGameObject<OBB>(OBJ3D);
	m_obb->SetOBBPos(D3DXVECTOR3(m_Position.x, m_Position.y + 0.25f, m_Position.z));
	m_obb->SetDirect(0, GetRight());
	m_obb->SetDirect(1, GetUp());
	m_obb->SetDirect(2, GetForward(m_Rotation));
	m_obb->SetLen_W(0, 0.5f);
	m_obb->SetLen_W(1, 0.5f);
	m_obb->SetLen_W(2, 0.4f);

	m_obb->SetAll(D3DXVECTOR3(m_obb->GetPos_W().x, m_obb->GetPos_W().y, m_obb->GetPos_W().z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_obb->GetLen_W(0), m_obb->GetLen_W(1), m_obb->GetLen_W(2)),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		0.0f);

	m_BulletFireFlg = true;
	m_BulletCoolDownCnt = 0;

	m_HP = 10;

	m_ReTargetCnt = 299;

	m_TargetY = (-atan2f((player->GetPos().z - m_Position.z), (player->GetPos().x - m_Position.x)) + (D3DX_PI / 2));
	m_EnemyToDrone = 1000.0f;
	m_EnemyToPlayer = 0.0f;

	m_MinDist = MIN_MYSELF_TO_TARGET_DIST;
	m_MaxDist = MAX_MYSELF_TO_TARGET_DIST;
	m_Inversion = false;
}

void Enemy::UnInit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_obb->SetDestroy();
	m_EnemyWeapon01->SetDestroy();
	m_EnemyWeapon02->SetDestroy();
	m_HPGauge->SetDestroy();
}

void Enemy::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	Drone* drone = scene->GetGameObject<Drone>(OBJ3D);

	//m_Dist = MeasureDist(m_Position, player->GetPos());
	m_HPGauge->SetPos(m_Position.x - 0.5f, m_Position.y + 1.0f, m_Position.z);
	m_HPGauge->SetHPGaugeSize(0);
	m_HPGauge->SetScale(0.0f, 0.0f, 0.0f);

	m_ReTargetCnt++;

	//ターゲットを再捜索
	if (m_ReTargetCnt > 150)
	{
		if (drone != nullptr)
		{
			m_EnemyToDrone = MeasureDist(m_Position, drone->GetPos());
		}
		m_EnemyToPlayer = MeasureDist(m_Position, player->GetPos());
		m_ReTargetCnt = 0;
		if (m_EnemyToDrone > m_EnemyToPlayer)
		{
			m_Target = NUM_TARGET_PLAYER;
		}
		if (m_EnemyToDrone < m_EnemyToPlayer)
		{
			m_Target = NUM_TARGET_DRONE;
		}
	}

	//プレイヤーのほうが近かったら
	if (m_Target == NUM_TARGET_PLAYER)
	{
		m_Dist = MeasureDist(m_Position, player->GetPos());
		m_TargetY = ((-atan2f((player->GetPos().z - m_Position.z), (player->GetPos().x - m_Position.x)) + (D3DX_PI / 2)));
		AdjustmentRadian(m_TargetY);
		//プレイヤーとの距離が最短距離を下回っていたら
		if (m_Dist < m_MinDist)
		{
			m_Inversion = true;
		}

		//プレイヤーとの距離が最長距離を上回っていたら
		if (m_Dist > m_MaxDist)
		{
			//m_TargetY = ((-atan2f((player->GetPos().z - m_Position.z), (player->GetPos().x - m_Position.x)) + (D3DX_PI / 2)));
			m_Inversion = false;
			//AdjustmentRadian(m_TargetY);
		}

		m_Rotation.y = (float)LerpRadian(m_Rotation.y, m_TargetY, 0.02f);
	}
	//ドローンのほうが近かったら
	if (m_Target == NUM_TARGET_DRONE && drone != nullptr)
	{
		m_Dist = MeasureDist(m_Position, drone->GetPos());
		m_TargetY = ((-atan2f((drone->GetPos().z - m_Position.z), (drone->GetPos().x - m_Position.x)) + (D3DX_PI / 2)));

		AdjustmentRadian(m_TargetY);
		//プレイヤーとの距離が最短距離を下回っていたら
		if (m_Dist < m_MinDist)
		{
			m_Inversion = true;
		}

		//プレイヤーとの距離が最長距離を上回っていたら
		if (m_Dist > m_MaxDist)
		{
			m_Inversion = false;
		}

		m_Rotation.y = (float)LerpRadian(m_Rotation.y, m_TargetY, 0.02f);
	}

	//座標更新
	if (!m_Inversion)
	{
		m_Position += GetForward(m_Rotation) * 0.025f;
	}
	if (m_Inversion)
	{
		m_Position -= GetForward(m_Rotation) * 0.025f;
	}


	//OBB関連の更新
	m_obb->SetOBBPos(D3DXVECTOR3(m_Position.x, m_Position.y + 0.25f, m_Position.z));
	m_obb->SetDirect(0, GetRight());
	m_obb->SetDirect(1, GetUp());
	m_obb->SetDirect(2, GetForward(m_Rotation));
	m_obb->SetPos(m_obb->GetPos_W().x, m_obb->GetPos_W().y, m_obb->GetPos_W().z);
	m_obb->SetRot(m_Rotation.x, m_Rotation.y, m_Rotation.z);

	//足に親(本体)のマトリクスを与えている
	m_EnemyWeapon01->SetParentMatrix(m_EnemyMatrix);
	m_EnemyWeapon02->SetParentMatrix(m_EnemyMatrix);

	m_BulletCoolDownCnt++;
	if (m_BulletCoolDownCnt > 20)
	{
		m_BulletFireFlg = true;
		m_BulletCoolDownCnt = 0;
	}

	if (m_BulletFireFlg == true && m_Dist > m_MinDist)
	{
		D3DXVECTOR3 a;
		a = m_EnemyWeapon01->GetForward(D3DXVECTOR3(m_EnemyWeapon01->GetRot().x, m_Rotation.y, m_EnemyWeapon01->GetRot().z));

		scene->AddGameObject<Bullet>(OBJ3D)->SetBulletAll(D3DXVECTOR3(m_Position.x, m_Position.y, m_Position.z),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),	//rot
			D3DXVECTOR3(0.1f, 0.1f, 0.1f),	//scl
			D3DXVECTOR3(a.x, a.y, a.z),	//spd
			0.2f,
			BULLET_ENEMY);	//radius	

		m_BulletFireFlg = false;
	}
}

void Enemy::Draw()
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
	m_EnemyMatrix = scale * rot * trans;

	Renderer::SetWorldMatrix(&m_EnemyMatrix);

	m_Model->Draw();
	//m_EnemyWeapon[0]->Draw(m_EnemyMatrix);
}
