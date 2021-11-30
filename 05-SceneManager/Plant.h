#pragma once
#include "GameObject.h"

#define PLANT_STATE_INVISIBLE	357
#define PLANT_STATE_REST		356
#define PLANT_STATE_VISIBLE		355

#define PLANT_BBOX_WIDTH	16
#define PLANT_BBOX_HEIGHT	32


class CPlant : public CGameObject
{
	float baseY;	
	ULONGLONG timer;	

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	
	void RenderBoundingBox();

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void SetState(int state);
public:
	CPlant(float x, float y);
};

