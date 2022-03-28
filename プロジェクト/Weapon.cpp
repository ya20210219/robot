#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "Scene.h"
#include "model.h"
#include "GameObject.h"
#include "Weapon.h"
#include "Player.h"
#include "Enemy.h"

Model* Weapon::m_Model[5];	//staticメンバ変数はcpp側で再度宣言する必要がある

void Weapon::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

	for (int i = 0; i < 5; i++)
	{
		bUse[i] = false;
	}
}

void Weapon::UnInit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Weapon::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);
}

void Weapon::Draw()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	std::vector<Enemy*> EnemyList = scene->GetGameObjects<Enemy>(OBJ3D);

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

	world = scale * rot * trans * m_ParentMatrix;

	Renderer::SetWorldMatrix(&world);

	for (int i = 0; i < 5; i++)
	{
		if (bUse[i] == true)
		{
			m_Model[i]->Draw();
		}
	}
}