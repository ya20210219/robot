#include "Debug.h"

void Debug::Init()
{
	m_DebugUse = false;
}

void Debug::Update()
{
	if (Keyboard_IsTrigger(DIK_F5))
	{
		if (m_DebugUse == false)
		{
			SetDebug(true);
		}
	}
	if (Keyboard_IsTrigger(DIK_F6))
	{
		if (m_DebugUse == true)
		{
			SetDebug(false);
		}
	}
}

void Debug::UnInit()
{
}

void Debug::Draw()
{
}
