#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Tutorial.h"
#include "Input.h"
#include "Result.h"
#include "GameScene.h"

#include "Camera.h"
#include "Field.h"
#include "Explosion.h"
#include "model.h"
#include "Player.h"
#include "Drone.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Weapon.h"
#include "Sky.h"
#include "polygon2D.h"
#include "Score.h"
#include "Pause.h"
#include "audio.h"
#include "OBB.h"
#include "Debug.h"
#include "Fade.h"
#include "Aim.h"

void GameScene::Init()
{
	Bullet::Load();
	Enemy::Load();
	Drone::Propeller::Load();
	Player::PlayerLeg::Load();
	OBB::Load();
	Weapon::Load();

	SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	Scene* scene = Manager::GetScene();

	AddGameObject<Camera>(SYSTEM);
	AddGameObject<Debug>(SYSTEM);

	AddGameObject<Sky>(OBJ3D);

	AddGameObject<Field>(OBJ3D);

	AddGameObject<Player>(OBJ3D);

	AddGameObject<Drone>(OBJ3D)->SetPos(200.0f, -5.0f, 200.0f);

	AddGameObject<Enemy>(OBJ3D)->SetPos(-5.0f, 1.0f, 5.0f);
	AddGameObject<Enemy>(OBJ3D)->SetPos(0.0f, 1.0f, 5.0f);
	AddGameObject<Enemy>(OBJ3D)->SetPos(5.0f, 1.0f, 5.0f);

	//AddGameObject<Polygon2D>(OBJ2D);

	AddGameObject<Score>(OBJ2D);

	AddGameObject<Aim>(OBJ2D);

	AddGameObject<Pause>(OBJ2D);

	m_Fade = scene->AddGameObject<Fade>(OBJ2D);

	Audio* bgm = AddGameObject<Audio>(OBJ2D);
	bgm->Load("asset\\audio\\Select.wav");
	bgm->Play(true);

	m_Fade->SetFade(true);
	m_Fade->SetFadeIn(true, 1.0f);
}

void GameScene::UnInit()
{
	Scene::UnInit();//継承元

	//Player::PlayerWeapon::UnLoad();
	Weapon::UnLoad();
	OBB::UnLoad();
	Player::PlayerLeg::UnLoad();
	Drone::Propeller::UnLoad();
	Enemy::UnLoad();
	Bullet::UnLoad();
}

void GameScene::Update()
{
	if (m_BirthCnt % 180 == 179)
	{
		AddGameObject<Enemy>(OBJ3D)->SetPos((float)(rand() % 11 - 5), 1.0f, (float)(rand() % 11 - 5));
		m_BirthCnt = 1;
	}
	Scene::Update();
	Scene* scene = Manager::GetScene();
	std::vector<Enemy*> EnemyList = scene->GetGameObjects<Enemy>(OBJ3D);
	Score* score = scene->GetGameObject<Score>(OBJ2D);
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	scene->SetTotalScore(score->GetScore());
	m_BirthCnt++;

	
	if (player->GetActive() == false)
	{
		if (m_Fade->GetFadeStart() == false)
		{
			scene->SetTotalScore(score->GetScore());
			m_Fade->SetFade(false);
			m_Fade->SetFade(true);
			m_Fade->SetFadeOut(true, 0.0f);
		}
	}
	if (m_Fade->GetFadeStart() == true)
	{
		if (m_Fade->GetFadeEnd() == true)
		{
			Manager::SetScene<Result>();
			scene->SetTotalScore(score->GetScore());
		}
	}

	//デバッグ用
	if (Keyboard_IsPress(DIK_0))
	{
		Manager::SetScene<Result>();
	}
}

