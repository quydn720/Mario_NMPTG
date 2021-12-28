#include "BreakableBrickPiece.h"

void BreakableBrickEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vy += 0.001 * dt;
	x += vx * dt;
	y += vy * dt;

	if (y > 500)
		this->isDeleted = true;
}

void BreakableBrickEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 8;
	b = y + 8;
}

void BreakableBrickEffect::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = ID_ANI_EFFECT_BREALABLE_BRICK;
	animations->Get(aniId)->Render(x, y);
}