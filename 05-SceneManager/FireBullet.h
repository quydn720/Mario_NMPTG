#pragma once
#include "GameObject.h"
class FireBullet : public CGameObject
{
private:
	float vx, vy;
public:
	FireBullet(float x, float y, float vx, float vy) {
		this->x = x; this->y = y;
		this->vy = vy; this->vx = vx;
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

