#pragma once
#include "GameObject.h"
#include "Mario.h"
#define BULLET_SPEED -0.001f

class FireBullet : public CGameObject
{
private:
	float vx, vy;
public:
	FireBullet(float x, float y, float vx, float vy) {
		this->x = x; this->y = y;

		float mx, my;
		CMario::GetInstance()->GetPosition(mx, my);
		this->vx = (x - mx) * BULLET_SPEED;
		this->vy = (y - my) * BULLET_SPEED;
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

