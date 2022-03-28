#include "main.h"
#include "Calculation.h"


#define	D3DX_PI2	(D3DX_PI * 2.0f)

double NormalizeRadian(double rad);

double LerpRadian(double a, double b, float t)
{
	//t‚Ì•â³
	if (t < 0.0f)
		t = 0.0f;
	if (t > 1.0f)
		t = 1.0f;

	return NormalizeRadian(a + t * NormalizeRadian(b - a));
}

double NormalizeRadian(double rad)
{
	//2ƒÎ‚ğí‚é
	int num = (int)(rad / D3DX_PI2);
	double temp = rad - (D3DX_PI2 * num);

	//-ƒÎ`+ƒÎ‚Ì’l‚É‚È‚é—l‚É•â³‚·‚é
	AdjustmentRadian(temp);

	return temp;
}

float AdjustmentRadian(float value)
{
	if (value < -D3DX_PI)
	{
		value += D3DX_PI2;
	}

	if (value > D3DX_PI)
	{
		value -= D3DX_PI2;
	}

	return value;
}

float MeasureDist(D3DXVECTOR3 my, D3DXVECTOR3 target)
{
	D3DXVECTOR3 dir;
	dir = my - target;
	float length = D3DXVec3Length(&dir);
	return length;
}
