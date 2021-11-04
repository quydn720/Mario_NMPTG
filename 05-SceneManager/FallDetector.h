#pragma once
#include "GameObject.h"

#define BBOX_WIDTH 10
#define FALL_DETECTOR_STATE_INACTIVE 11
#define FALL_DETECTOR_STATE_ACTIVE 12

class FallDetector : public CGameObject
{
protected:
	float ax, ay;
public:
	int height;
	bool willFall;
	FallDetector(float x, float y) : CGameObject(x, y) {
		height = 0;
		vx = 0;
		this->ax = 0;
		this->ay = 0.002f;
		willFall = false;
		SetState(FALL_DETECTOR_STATE_ACTIVE);
	}
	virtual void Render();
	virtual int IsBlocking() { return 0; }
	virtual int IsCollidable() { return 1; };
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};

