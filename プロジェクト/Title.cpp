#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Title.h"

#include "TitleLogo.h"
#include "Input.h"
#include "Fade.h"
#include "Tutorial.h"

void Title::Init()
{
	Scene* scene = Manager::GetScene();
	AddGameObject<TitleLogo>(OBJ2D);
	m_Fade = scene->AddGameObject<Fade>(OBJ2D);

}

void Title::UnInit()
{
	Scene::UnInit();//Œp³Œ³
}

void Title::Update()
{
	Scene::Update();

	Scene* scene = Manager::GetScene();

	if (Keyboard_IsPress(DIK_RETURN) || Keyboard_IsPress(DIK_SPACE))
	{
		m_Fade->SetFade(true);
		m_Fade->SetFadeOut(true, 0.0f);
	}
	if (m_Fade->GetFadeStart() == true)
	{
		Fade* fade = scene->GetGameObject<Fade>(OBJ3D);
		if (m_Fade->GetFadeEnd() == true)
		{
			Manager::SetScene<Tutorial>();
		}
	}
}
