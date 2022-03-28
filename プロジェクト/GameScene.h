#pragma once

#include "Scene.h"


class GameScene : public Scene
{
private:
	class Fade* m_Fade;
	int m_BirthCnt;
public:
	void Init();
	void UnInit();
	void Update();
};