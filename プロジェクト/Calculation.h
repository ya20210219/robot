#pragma once

#define VALUE_ROTATE (D3DX_PI * 0.01f)


double LerpRadian(double a, double b, float t);
float AdjustmentRadian(float value);
float MeasureDist(D3DXVECTOR3 my, D3DXVECTOR3 target);