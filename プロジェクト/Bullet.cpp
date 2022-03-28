#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "Scene.h"
#include "model.h"
#include "GameObject.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"
#include "Drone.h"
#include "Input.h"
#include "Explosion.h"
#include "Score.h"
#include "Weapon.h"
#include "Calculation.h"
#include "Debug.h"
#include "OBB.h"
#include <vector>

Model* Bullet::m_Model;	//staticメンバ変数はcpp側で再度宣言する必要がある

void Bullet::Init()
{
	Scene* scene = Manager::GetScene();
	Weapon* weapon = scene->GetGameObject<Weapon>(OBJ3D);
	Player* player = scene->GetGameObject<Player>(OBJ3D);

	//m_Speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	timer = 0;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");


	//Weapon* w = player->GetPW();
	//m_Way = w->GetForward(D3DXVECTOR3(w->GetRot().x, player->GetRot().y, w->GetRot().z));

	//m_Way = m_Speed;

	m_obb = scene->AddGameObject<OBB>(OBJ3D);
	m_obb->SetOBBPos(D3DXVECTOR3(m_Position.x, m_Position.y, m_Position.z));
	m_obb->SetDirect(0, GetRight());
	m_obb->SetDirect(1, GetUp());
	m_obb->SetDirect(2, GetForward(m_Rotation));
	m_obb->SetLen_W(0, GetScale().x);
	m_obb->SetLen_W(1, GetScale().y);
	m_obb->SetLen_W(2, GetScale().z);

	m_obb->SetAll(D3DXVECTOR3(m_obb->GetPos_W().x, m_obb->GetPos_W().y, m_obb->GetPos_W().z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_obb->GetLen_W(0), m_obb->GetLen_W(1), m_obb->GetLen_W(2)),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		0.0f);
}

void Bullet::UnInit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	m_obb->SetDestroy();
}

void Bullet::Update()
{
	Scene* scene = Manager::GetScene();
	Weapon* weapon = scene->GetGameObject<Weapon>(OBJ3D);
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	Drone* drone = scene->GetGameObject<Drone>(OBJ3D);
	Score* score = scene->GetGameObject<Score>(OBJ2D);
	Debug* d = scene->GetGameObject<Debug>(SYSTEM);

	if (d->GetDebug() == false)
	{
		timer++;

		if (timer > 120)
		{
			SetDestroy();
			return;
		}

		//m_Position += m_Speed;
		//m_Dir = weapon->GetForward();
		m_Position += m_Speed * 0.1f;

		m_obb->SetOBBPos(D3DXVECTOR3(m_Position.x, m_Position.y, m_Position.z));
		m_obb->SetDirect(0, GetRight());
		m_obb->SetDirect(1, GetUp());
		m_obb->SetDirect(2, GetForward(m_Rotation));
		m_obb->SetPos(m_obb->GetPos_W().x, m_obb->GetPos_W().y, m_obb->GetPos_W().z);
		m_obb->SetScale(m_Scale.x, m_Scale.y, m_Scale.z);
		m_obb->SetRot(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		std::vector<Enemy*> EnemyList = scene->GetGameObjects<Enemy>(OBJ3D);

		//====敵と弾の判定=========================================================================================================
		if (m_Type == BULLET_PLAYER)
		{
			for (Enemy* enemy : EnemyList)
			{
				D3DXVECTOR3 EnemyPos = enemy->GetPos();

				float length = MeasureDist(m_Position, EnemyPos);
				float r = enemy->GetRadius();

				//球と球での判定
				/*
				if (length < (r + m_Radius))
				{
					scene->AddGameObject<Explosion>(OBJ3D)->SetAll(D3DXVECTOR3(EnemyPos.x, EnemyPos.y, EnemyPos.z),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),	//rot
						D3DXVECTOR3(1.0f, 1.0f, 1.0f),	//scl
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						0.0f);

					enemy->SetDestroy();
					SetDestroy();
					return;
				}
				*/

				//OBB同士での判定
				if (m_obb->ColOBBs(*m_obb, *enemy->GetOBB()))
				{
					scene->AddGameObject<Explosion>(OBJ3D)->SetAll(D3DXVECTOR3(EnemyPos.x, EnemyPos.y, EnemyPos.z),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),	//rot
						D3DXVECTOR3(1.0f, 1.0f, 1.0f),	//scl
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						0.0f);

					enemy->SetHitHP();
					//enemy->SetDestroy();
					SetDestroy();
					//score->AddScore();
					return;
				}
			}
		}

		//====プレイヤーと弾の判定=========================================================================================================
		if (m_Type == BULLET_ENEMY)
		{
			//球と球での判定
			/*
			if (length < (r + m_Radius))
			{
				scene->AddGameObject<Explosion>(OBJ3D)->SetAll(D3DXVECTOR3(EnemyPos.x, EnemyPos.y, EnemyPos.z),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),	//rot
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),	//scl
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					0.0f);

				enemy->SetDestroy();
				SetDestroy();
				return;
			}
			*/

			//OBB同士での判定
			if (m_obb->ColOBBs(*m_obb, *player->GetOBB()))
			{
				scene->AddGameObject<Explosion>(OBJ3D)->SetAll(D3DXVECTOR3(player->GetPos().x, player->GetPos().y, player->GetPos().z),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),	//rot
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),	//scl
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					0.0f);

				player->SetHitHP();
				SetDestroy();
				return;
			}		

			if (drone != nullptr)
			{
				if (m_obb->ColOBBs(*m_obb, *drone->GetOBB()))
				{
					scene->AddGameObject<Explosion>(OBJ3D)->SetAll(D3DXVECTOR3(drone->GetPos().x, drone->GetPos().y, drone->GetPos().z),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),	//rot
						D3DXVECTOR3(1.0f, 1.0f, 1.0f),	//scl
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						0.0f);

					drone->SetHitHP();
					SetDestroy();
					return;
				}
			}
		}
	}
}

void Bullet::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z); //Yaw = y Pitch = x Roll = z
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}
