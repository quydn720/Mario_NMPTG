#pragma once
#include "GameObject.h"

#define TAIL_ACTIVE 1
#define TAIL_INACTIVE 2
#define TAIL_ATTACK 3

#define TAIL_LENGTH 8
#define TAIL_HEIGHT 4
#define TAIL_OFFSET 22
#define TAIL_VELOCITY 0.1f

#define TAIL_BBOX_WIDTH 8
#define TAIL_BBOX_HEIGHT 3

class CTail : public CGameObject
{
protected:
	float ax, ay;
	float baseX;
public:
	bool IsActive;
	CTail(float x, float y) : CGameObject(x, y) {
		baseX = x;
		vx = 0;
		vy = 0;
		this->ay = 0;
		SetState(TAIL_ACTIVE);
	}
	virtual void Render();
	virtual int IsBlocking() { return 0; }
	virtual int IsCollidable() {
		if (state == TAIL_ATTACK) return 1;
		else return 0;
	};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void RenderBoundingBox();
	void SetState(int state);
	void OnCollisionWithBreakableBrick(LPGAMEOBJECT& obj);
};

