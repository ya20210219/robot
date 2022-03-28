#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "Scene.h"
#include "model.h"
#include "Drone.h"
#include "Input.h"
#include "GameObject.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "OBB.h"
#include "Camera.h"
#include "Calculation.h"
#include "HPGauge.h"
#include <vector>

Drone::Propeller propeller[4];

Model* Drone::Propeller::m_Model;	//staticメンバ変数はcpp側で再度宣言する必要がある

#define VALUE_ROTATE (D3DX_PI * 0.01f)

void Drone::Init()
{
	m_Model = new Model();
	m_Model->Load(MODEL_DRONE);	//\\2個に注意「//」でもいい

	m_Position = D3DXVECTOR3(0.0f, 1.0f, -5.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.25f, 0.25f, 0.25f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

	m_DroneSpd = 0.0f;
	m_Active = false;
	m_HP = 10;
	Scene* scene = Manager::GetScene();

	m_HPGauge = scene->AddGameObject<HPGauge>(OBJ2D);
	m_HPGauge->SetPos(0.0f, -2.0f, 0.0f);
	m_HPGauge->SetScale(0.0f, 0.0f, 0.0f);
	//m_HPGauge->SetHPGaugeSize(0);
	
	propeller[0].Pos = D3DXVECTOR3(m_Position.x + sinf(m_Rotation.y + (D3DX_PI / 4)) * 3.5f,
				 				   m_Position.y + 2.0f,
		                          (m_Position.z + 5.0f + cos(m_Rotation.y + (D3DX_PI / 4)) * 3.25f));
	propeller[0].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	propeller[0].Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	
	propeller[1].Pos = D3DXVECTOR3(m_Position.x + sinf(m_Rotation.y + (-D3DX_PI / 4)) * 3.5f,
		                           m_Position.y + 2.0f,
								  (m_Position.z + 5.0f + cos(m_Rotation.y + (D3DX_PI / 4)) * 3.25f));
	propeller[1].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	propeller[1].Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	
	propeller[2].Pos = D3DXVECTOR3(m_Position.x + sinf(m_Rotation.y + (-D3DX_PI / 4)) * 3.5f,
								   m_Position.y + 2.0f,
								  (m_Position.z + 5.0f - cos(m_Rotation.y - (D3DX_PI / 4)) * 3.75f));
	propeller[2].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	propeller[2].Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	
	propeller[3].Pos = D3DXVECTOR3(m_Position.x + sinf(m_Rotation.y + (D3DX_PI / 4)) * 3.5f,
								   m_Position.y + 2.0f,
								  (m_Position.z + 5.0f - cos(m_Rotation.y - (D3DX_PI / 4)) * 3.75f));
	propeller[3].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	propeller[3].Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_obb = scene->AddGameObject<OBB>(OBJ3D);
	m_obb->SetOBBPos(D3DXVECTOR3(m_Position.x, m_Position.y + 0.25f, m_Position.z));
	m_obb->SetDirect(0, GetRight());
	m_obb->SetDirect(1, GetUp());
	m_obb->SetDirect(2, GetForward(m_Rotation));
	m_obb->SetLen_W(0, 0.85f);
	m_obb->SetLen_W(1, 0.5f);
	m_obb->SetLen_W(2, 0.85f);

	m_obb->SetAll(D3DXVECTOR3(m_obb->GetPos_W().x, m_obb->GetPos_W().y, m_obb->GetPos_W().z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_obb->GetLen_W(0), m_obb->GetLen_W(1), m_obb->GetLen_W(2)),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		0.0f);

	m_TargetDist = 1000.0f;
	m_ReTargetCnt = 300.0f;
}

void Drone::UnInit()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	m_HPGauge->SetDestroy();
	m_obb->SetDestroy();
}

void Drone::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);

	if (Keyboard_IsPress(DIK_J) && player->GetActive() == false)
	{
		Drone::m_ActiveCnt++;
	}

	if (Keyboard_IsRelease(DIK_J))
	{
		Drone::m_ActiveCnt = 0;
	}

	if (m_ActiveCnt > 60 && m_Active == true)
	{
		player->SetActive(true);
		m_Active = false;
		m_ActiveCnt = 0;
	}

	if (m_Active == false)
	{
		m_Position = D3DXVECTOR3(100.0f, -50.0f, 100.0f);
		m_HPGauge->SetScale(0.0f, 0.0f, 0.0f);
	}

	if (m_Active == true)
	{
		Scene* scene = Manager::GetScene();
		Camera* Cam = scene->GetGameObject<Camera>(SYSTEM);

		m_HPGauge->SetPos(m_Position.x - 0.5f, m_Position.y + 1.0f, m_Position.z);
		m_HPGauge->SetHPGaugeSize(m_HP);
		m_HPGauge->SetScale(0.1f, 0.1f, 0.1f);

		for (int i = 0; i < 4; i++)
		{
			propeller[i].Rot.y += 1.0f;
		}

		m_ReTargetCnt++;

		std::vector<Enemy*> EnemyList = scene->GetGameObjects<Enemy>(OBJ3D);

		if (m_ReTargetCnt > 300)
		{
			for (Enemy* enemy : EnemyList)
			{
				D3DXVECTOR3 EnemyPos = enemy->GetPos();

				float length = MeasureDist(m_Position, EnemyPos);

				if (length < m_TargetDist)
				{
					m_TargetDist = length;
					m_TargetEnemy = enemy;
				}
			}

			m_ReTargetCnt = 0;
			m_TargetDist = 100.0f;
		}

		if (m_TargetEnemy != nullptr)
		{
			m_TargetRotY = (-atan2f((m_TargetEnemy->GetPos().z - m_Position.z), (m_TargetEnemy->GetPos().x - m_Position.x)) + (D3DX_PI / 2));

			AdjustmentRadian(m_TargetRotY);

			m_Rotation.y = (float)LerpRadian(m_Rotation.y, m_TargetRotY, 0.1f);
		}

		//OBB関連の更新
		m_obb->SetOBBPos(D3DXVECTOR3(m_Position.x, m_Position.y + 0.25f, m_Position.z));
		m_obb->SetDirect(0, GetRight());
		m_obb->SetDirect(1, GetUp());
		m_obb->SetDirect(2, GetForward(m_Rotation));
		m_obb->SetPos(m_obb->GetPos_W().x, m_obb->GetPos_W().y, m_obb->GetPos_W().z);
		m_obb->SetRot(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		m_BulletCoolDownCnt++;
		if (m_BulletCoolDownCnt > 20)
		{
			m_BulletFireFlg = true;
			m_BulletCoolDownCnt = 0;
		}

		if (m_BulletFireFlg == true)
		{
			D3DXVECTOR3 a;
			a = GetForward(D3DXVECTOR3(m_Rotation.x, m_Rotation.y, m_Rotation.z));

			scene->AddGameObject<Bullet>(OBJ3D)->SetBulletAll(D3DXVECTOR3(m_Position.x, m_Position.y, m_Position.z),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),	//rot
				D3DXVECTOR3(0.1f, 0.1f, 0.1f),	//scl
				D3DXVECTOR3(a.x, a.y, a.z),	//spd
				0.2f,
				BULLET_PLAYER);	//radius	

			m_BulletFireFlg = false;
		}
	}
}

void Drone::Draw()
{
	if (m_Active)
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
		m_DroneMatrix = scale * rot * trans;

		Renderer::SetWorldMatrix(&m_DroneMatrix);

		m_Model->Draw();

		for (int i = 0; i < 4; i++)
		{
			propeller[i].Draw(i, m_DroneMatrix);
		}
	}
}

void Drone::Propeller::Draw(int index, D3DMATRIX matrix)
{
	D3DXMATRIX scale, rot, trans, DMatrix;

	//マトリクス設定
	D3DXMatrixScaling(&scale, propeller[index].Scl.x, propeller[index].Scl.y, propeller[index].Scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, propeller[index].Rot.y, propeller[index].Rot.x, propeller[index].Rot.z); //Yaw = y Pitch = x Roll = z
	D3DXMatrixTranslation(&trans, propeller[index].Pos.x, propeller[index].Pos.y, propeller[index].Pos.z);

	propeller[index].m_PropellerMatrix = scale * rot * trans * matrix;

	Renderer::SetWorldMatrix(&propeller[index].m_PropellerMatrix);

	propeller[index].m_Model->Draw();
}