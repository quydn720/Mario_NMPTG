#pragma once
#include "GameObject.h"

#define PLANT_STATE_INVISIBLE	357
#define PLANT_STATE_UP		356
#define PLANT_STATE_DOWN		355

#define PLANT_BBOX_WIDTH	16
#define PLANT_BBOX_HEIGHT	32
#define PLANT_BBOX_HEIGHT_OFFSET	8

#define PLANT_ANI_BOTTOM_LEFT	11500
#define PLANT_ANI_BOTTOM_RIGHT	11501
#define PLANT_ANI_UP_LEFT	11502
#define PLANT_ANI_UP_RIGHT	11503

#define PLANT_VY	0.04f


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

