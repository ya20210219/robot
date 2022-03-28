#include "main.h"
#include "GameObject.h"
#include "manager.h"
#include "Input.h"
#include "renderer.h"
#include "Scene.h"
#include "GameScene.h"
#include "Title.h"
#include "Result.h"
#include "Fade.h"
#include "audio.h"

Scene* Manager::m_Scene = nullptr;

void Manager::Init()
{
	Renderer::Init();
	Audio::InitMaster();

	SetScene<Title>();
}


void Manager::Uninit()
{
	m_Scene->UnInit();
	delete m_Scene;

	Audio::UninitMaster();
	UninitInput();
	Renderer::Uninit();
}

void Manager::Update()
{
	UpdateInput();

	m_Scene->Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();

	Renderer::End();
}
