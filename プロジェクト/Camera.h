#pragma once

#include "GameObject.h"

class Camera : public GameObject
{
private:

	static class Camera* m_Camera;	//頭にclassとつけると読み込める。前方宣言という
	D3DXVECTOR3 m_Target;		//注視点

	D3DXMATRIX m_ViewMatrix;

	float	m_Dist;				//カメラと注視点の距離
	D3DXVECTOR3   m_CameraMove;

public:
	//Polygon2D();	//今回は不要
	//~Polygon2D();

	void Init();
	void UnInit();
	void Update();
	void Draw();

	void SetTargetPos(float posX, float posY, float posZ)
	{
		m_Target.x = posX;
		m_Target.y = posY;
		m_Target.z = posZ;
	}

	D3DXVECTOR3 GetTargetPos()
	{
		return m_Target;
	}


	D3DXVECTOR3 GetCameraRot()
	{
		return Camera::m_Rotation;
	}

	float GetCameraDist()
	{
		return m_Dist;
	}

	float GetCameraR()
	{
		return Camera::m_Rotation.y;
	}

	Camera* GetCamera()
	{
		return m_Camera;
	}

	D3DXMATRIX GetViewMatrix()
	{
		return m_ViewMatrix;
	}

	D3DXVECTOR3 GetCameraMove()
	{
		return m_CameraMove;
	}
};