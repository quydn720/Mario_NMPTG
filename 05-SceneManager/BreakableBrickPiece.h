#pragma once
#include "GameObject.h"
#define ID_ANI_EFFECT_BREALABLE_BRICK	10003
class BreakableBrickEffect : public CGameObject
{
public:
	float x, y, vx, vy;
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);

	BreakableBrickEffect(float X, float Y, float Vx, float Vy)
	{
		x = X;
		y = Y;
		vx = Vx;
		vy = Vy;
	}
	
	
};

