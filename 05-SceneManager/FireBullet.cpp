#include "FireBullet.h"
#include "debug.h"

#define BULLET_ANI 11504

void FireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;
}

void FireBullet::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(BULLET_ANI)->Render(x, y);
}

void FireBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - 16 / 2;
	t = y - 16 / 2;
	r = l + 16;
	b = t + 16 - 1;
}
