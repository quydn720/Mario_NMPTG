#include "Plant.h"
#include "Textures.h"
#include "Mario.h"
#include "debug.h"

#define PLANT_SHOOTING_RANGE 160 // half of the screen

CPlant::CPlant(float x, float y)
{
	SetState(PLANT_STATE_REST);
	baseY = y;
}

void CPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	DebugOut(L"%d\n", state);
	y += vy * dt;

	float mx = CMario::GetInstance()->getX();
	bool marioInRange = abs(x - mx) <= PLANT_SHOOTING_RANGE;

	if (state == PLANT_STATE_VISIBLE) {
		if (y > baseY) {
			y = baseY;
			if (GetTickCount64() - timer >= 2000) {
				SetState(PLANT_STATE_REST);
			}
		}
	}
	//TODO: Get position of mario
	//TODO: if (state == rest && marioX in range [150-150] change to state visible
	//TODO: after a period of time, shoot, then go rest state.
	else if (state == PLANT_STATE_REST && marioInRange) {
		if (y < baseY - 30) {
			y = baseY - 30;
			if (GetTickCount64() - timer >= 5000) {
				SetState(PLANT_STATE_VISIBLE); // shoot
			}
		}
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CPlant::Render() {
	int aniId = -1;

	// TODO: Can reduce the animation, when fully behind the [pipe]
	// (at the waiting time)
	aniId = 11500;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y + 8);

	RenderBoundingBox();
}
void CPlant::OnNoCollision(DWORD dt) {
	x += vx * dt;
	y += vy * dt;
}

void CPlant::OnCollisionWith(LPCOLLISIONEVENT e) {

}
void CPlant::SetState(int state) {
	switch (state)
	{
	case PLANT_STATE_VISIBLE:
		vy = 0.04f;
		timer = GetTickCount64();
		break;
	case PLANT_STATE_REST:
		vy = -0.04f;
		timer = GetTickCount64();
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}


void CPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom) {

	float yy = (PLANT_BBOX_HEIGHT - 16) / 2;
	left = x - PLANT_BBOX_WIDTH / 2;
	top = y - PLANT_BBOX_HEIGHT / 2 + yy;
	right = left + 16;
	bottom = top + 32;
}
void CPlant::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = x - PLANT_BBOX_WIDTH / 2 + rect.right / 2;
	float yy = (PLANT_BBOX_HEIGHT - PLANT_BBOX_WIDTH) / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy + yy, bbox, nullptr, BBOX_ALPHA, rect.right, rect.bottom);
}