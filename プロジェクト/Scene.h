#pragma once

#include <list>		//リスト構造
#include <vector>
#include <typeinfo>
#include "GameObject.h"

#define LAYER_NUM (3)

enum LAYER_NUMBER
{
	SYSTEM,
	OBJ3D,
	OBJ2D,

	NUM_MAX
};

class Scene
{
protected:
	std::list<GameObject*> m_GameObject[LAYER_NUM];	//STLのリスト構造

	int m_TotalScore;

public:
	Scene(){}
	virtual ~Scene(){}

	virtual void Init() = 0;	//	純粋仮想関数

	virtual void UnInit()
	{
		for (int i = 0; i < LAYER_NUM; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->UnInit();	//ポリモーフィズム(多態性：インスタンスの関数が呼び出される)
				delete object;
			}

			m_GameObject[i].clear();
		}
	}

	virtual void Update()
	{
		for (int i = 0; i < LAYER_NUM; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Update();	//ポリモーフィズム(多態性：インスタンスの関数が呼び出される)
			}

			m_GameObject[i].remove_if([](GameObject* object) { return object->Destroy(); });
		}
	}

	virtual void Draw()
	{
		for (int i = 0; i < LAYER_NUM; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Draw();	//ポリモーフィズム(多態性：インスタンスの関数が呼び出される)
			}
		}
	}

	template<typename T>
	T* AddGameObject(int layer)
	{
		T* gameObject = new T();
		m_GameObject[layer].push_back(gameObject);
		gameObject->Init();

		return gameObject;
	}

	template<typename T>
	T* GetGameObject(int layer)	//1つだけのオブジェクト取得
	{
		for (GameObject* object : m_GameObject[layer])
		{
			if (typeid(*object) == typeid(T))	//型を調べる(RTTI動的型情報)
			{
				return (T*)object;
			}
		}
		return NULL;
	}

	template<typename T>	//テンプレート関数
	std::vector<T*> GetGameObjects(int layer)	//複数のオブジェクト取得
	{
		std::vector<T*> objects;
		for (GameObject* object : m_GameObject[layer])
		{
			if (typeid(*object) == typeid(T))
			{
				objects.push_back((T*)object);
			}
		}
		return objects;
	}

	void SetTotalScore(int score)
	{
		m_TotalScore = score;
	}

	int GetTotalScore()
	{
		return m_TotalScore;
	}
};
