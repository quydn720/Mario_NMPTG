#pragma once
#include "GameObject.h"

#define MARIO_TAIL_WIDTH  9.0f//8.0f
#define MARIO_TAIL_HEIGHT 6.0f//2.0f

#define TAIL_BBOX_WIDTH 8
#define TAIL_BBOX_HEIGHT 3

class CTail : public CGameObject
{
public:
	CTail(float x, float y) : CGameObject(x, y) {}
	virtual void Render();
	virtual int IsBlocking() { return 0; }
	virtual int IsCollidable() { return 1; };
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void OnCollisionWithBreakableBrick(LPGAMEOBJECT& obj);
};

