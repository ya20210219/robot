#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Result.h"

#include "ResultLogo.h"
#include "Input.h"
#include "GameScene.h"
#include "Score.h"
#include "Title.h"

void Result::Init()
{
	Scene* scene = Manager::GetScene();

	AddGameObject<ResultLogo>(OBJ2D);
}

void Result::UnInit()
{
	Scene::UnInit();//åpè≥å≥
}

void Result::Update()
{
	Scene::Update();
	Scene* scene = Manager::GetScene();

	if (Keyboard_IsPress(DIK_RETURN))
	{
		Manager::SetScene<Title>();
	}
}
