#pragma once

#include "main.h"

struct BoxCol
{
	D3DXVECTOR3 center;
	D3DXVECTOR3 r[3];
};


bool intersectAABB(BoxCol box1, BoxCol box2)
{
	if (box1.center - box1.r[0] > box2.center + box2.r[0]) return false;
	if (box1.center + box1.r[0] > box2.center - box2.r[0]) return false;
	if (box1.center - box1.r[1] > box2.center + box2.r[1]) return false;
	if (box1.center + box1.r[1] > box2.center - box2.r[1]) return false;
	if (box1.center - box1.r[2] > box2.center + box2.r[2]) return false;
	if (box1.center + box1.r[2] > box2.center - box2.r[2]) return false;
	return true;
}
