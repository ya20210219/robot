#pragma once

#include "Scene.h"


class Title : public Scene
{
private:
	class Fade* m_Fade;
public:
	void Init();
	void UnInit();
	void Update();
};