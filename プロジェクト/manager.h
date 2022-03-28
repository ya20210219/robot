#pragma once

#include "Scene.h"

class Manager
{
private:
	static class Scene* m_Scene;	//����class�Ƃ���Ɠǂݍ��߂�B�O���錾�Ƃ���

public:	//���\�b�h�A�����o�֐�

	static void Init();		//����������
	static void Uninit();	//�I������
	static void Update();	//�X�V����
	static void Draw();		//�`�揈��

	static  class Scene* GetScene() { return m_Scene; }

	template<typename T>
	static void SetScene()
	{
		if (m_Scene)
		{
			m_Scene->UnInit();
			delete m_Scene;
		}

		T* scene = new T();
		m_Scene = scene;
		scene->Init();
	}
};