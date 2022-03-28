#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include "Camera.h"
#include "GameObject.h"
#include "model.h"
#include "Player.h"
#include "Input.h"
#include "Drone.h"
#include "Calculation.h"
#include "Scene.h"
#include "Manager.h"
#include "Weapon.h"

void Camera::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 3.0f, -5.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Dist = -5.0f;
}

void Camera::UnInit()
{
}

void Camera::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	Drone* drone = scene->GetGameObject<Drone>(OBJ3D);

	//プレイヤーカメラの場合
	if (player->GetActive())
	{
		if (Keyboard_IsPress(DIK_LEFT)/* && !Keyboard_IsPress(DIK_LSHIFT)*/)
		{
			m_Rotation.y -= Mouse_GetMoveX();
			m_Rotation.y = AdjustmentRadian(m_Rotation.y);
			if (player->GetWeaponActive() == true)
			{
				player->SetRot(player->GetRot().x, m_Rotation.y, player->GetRot().z);
			}
		}

		if (Keyboard_IsPress(DIK_RIGHT)/* && !Keyboard_IsPress(DIK_LSHIFT)*/)
		{
			m_Rotation.y += Mouse_GetMoveX();
			m_Rotation.y = AdjustmentRadian(m_Rotation.y);
			if (player->GetWeaponActive() == true)
			{
				player->SetRot(player->GetRot().x, m_Rotation.y, player->GetRot().z);
			}
		}

		m_CameraMove.x = Mouse_GetMoveX();
		m_Rotation.y += (m_CameraMove.x / 1000);
		m_Rotation.y = AdjustmentRadian(m_Rotation.y);

		m_CameraMove.y = Mouse_GetMoveY();
		m_Rotation.x += (m_CameraMove.y / 1000);
		m_Rotation.x = AdjustmentRadian(m_Rotation.x);

		m_Position.y += (-m_CameraMove.y / 1000);

		if (player->GetWeaponActive() == true)
		{
			player->SetRot(player->GetRot().x, m_Rotation.y, player->GetRot().z);
			Weapon* pw = player->GetPW();
			//if (m_Position.y < 2 && m_Position.y > -2)
			//{
				pw->SetRot(m_Rotation.x, pw->GetRot().y, pw->GetRot().z);
			//}
		}

		//徐々にプレイヤー正面を向く
		if (Keyboard_IsPress(DIK_RSHIFT))
		{
			//m_Rotation.y = cam->GetRot().y;
			m_Rotation.y = (float)LerpRadian(m_Rotation.y, player->GetRot().y, 0.2f);
		}

		m_Target = D3DXVECTOR3(player->GetPos().x, m_Position.y, player->GetPos().z);

		if (player->GetWeaponActive() == true)
		{
			m_Position = D3DXVECTOR3(m_Target.x + sinf(m_Rotation.y) * m_Dist, m_Position.y, m_Target.z + cosf(player->GetRot().y) * m_Dist);
		}
		if (player->GetWeaponActive() == false)
		{
			m_Position = D3DXVECTOR3(m_Target.x + sinf(m_Rotation.y) * m_Dist, m_Position.y, m_Target.z + cosf(m_Rotation.y) * m_Dist);
		}
	}

	////ドローンカメラの場合
	//if (drone->GetActive() == true)
	//{
	//	m_CameraMove.x = Mouse_GetMoveX();
	//	m_Rotation.y += (m_CameraMove.x / 1000);
	//	m_Rotation.y = AdjustmentRadian(m_Rotation.y);

	//	m_CameraMove.y = Mouse_GetMoveY();
	//	m_Rotation.x += (m_CameraMove.y / 1000);
	//	m_Rotation.x = AdjustmentRadian(m_Rotation.x);

	//	m_Target = D3DXVECTOR3(drone->GetPos().x, m_Position.y, drone->GetPos().z);

	//	m_Position = D3DXVECTOR3(m_Target.x + sinf(drone->GetRot().y) * m_Dist, drone->GetPos().y, m_Target.z + cosf(drone->GetRot().y) * m_Dist);

	//	//m_Target = D3DXVECTOR3(drone->GetPos().x, m_Position.y, drone->GetPos().z);


	//	//m_Position = D3DXVECTOR3(m_Target.x + sinf(drone->GetRot().y) * m_Dist, m_Target.y, m_Target.z + cosf(drone->GetRot().y) * m_Dist);
	//}

	/*
	//カメラの注視点と視点を主人公に追従させる
	if (m_Target.y < 0)
	{
		m_Target.y += D3DX_PI * 2.0f;
	}

	if (m_Target.y > D3DX_PI * 2.0f)
	{
		m_Target.y -= D3DX_PI * 2.0f;
	}

	m_Rotation.y = (-atan2f((m_Target.z - m_Position.z), (m_Target.x - m_Position.x)) - (D3DX_PI / 2));

	m_Rotation.y = (float)LerpRadian(m_Rotation.y, m_Target.y, 0.02f);	//0.2で8
	*/
}

void Camera::Draw()
{
	//ビューマトリクス設定
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	Renderer::SetViewMatrix(&m_ViewMatrix);

	//プロジェクションマトリクス設定
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}
