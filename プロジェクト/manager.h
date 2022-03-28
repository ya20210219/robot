#pragma once

#include "Scene.h"

class Manager
{
private:
	static class Scene* m_Scene;	//頭にclassとつけると読み込める。前方宣言という

public:	//メソッド、メンバ関数

	static void Init();		//初期化処理
	static void Uninit();	//終了処理
	static void Update();	//更新処理
	static void Draw();		//描画処理

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