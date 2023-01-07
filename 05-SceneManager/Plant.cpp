#include "Plant.h"
#include "Textures.h"
#include "Mario.h"
#include "debug.h"
#include "PlayScene.h"
#include "FireBullet.h"

#define PLANT_SHOOTING_RANGE 160 // half of the screen
#define BULLET_SPEED 0.04f

CPlant::CPlant(float x, float y) : CGameObject(x, y)
{
	SetState(PLANT_STATE_UP);
	baseY = y;
}

void CPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	y += vy * dt;

	float mx = CMario::GetInstance()->getX();
	bool marioInRange = abs(x - mx) <= PLANT_SHOOTING_RANGE;

	if (state == PLANT_STATE_DOWN && marioInRange) {
		nx = (int)(x - mx); // Plant will fixed direction when below the pipe
		if (y >= baseY) {
			y = baseY;
			vy = 0;
			if (GetTickCount64() - timer >= 2000) {
				SetState(PLANT_STATE_UP);
			}
		}
	}
	//TODO: Get position of mario
	//TODO: if (state == rest && marioX in range [150-150] change to state visible
	//TODO: after a period of time, shoot, then go rest state.
	else if (state == PLANT_STATE_UP) {
		if (y <= baseY - PLANT_BBOX_HEIGHT) {
			y = baseY - PLANT_BBOX_HEIGHT;
			vy = 0;
			if (GetTickCount64() - timer >= 2000) {
				SetState(PLANT_STATE_DOWN); // shoot
			}
		}
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CPlant::Render() {
	int aniId = -1;
	float my = CMario::GetInstance()->getY();

	// TODO: Can reduce the animation, when fully behind the [pipe]
	// (at the waiting time)
	if (nx > 0) {
		if (y <= my)
			aniId = PLANT_ANI_BOTTOM_LEFT;
		else
			aniId = PLANT_ANI_UP_LEFT;
	}
	else {
		if (y <= my)
			aniId = PLANT_ANI_BOTTOM_RIGHT;
		else
			aniId = PLANT_ANI_UP_RIGHT;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y + PLANT_BBOX_HEIGHT_OFFSET);

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
	case PLANT_STATE_DOWN:
	{
		float mx, my;
		CMario::GetInstance()->GetPosition(mx, my);

		vy = PLANT_VY;
		timer = GetTickCount64();
		FireBullet* bullet = new FireBullet(x, y, (x - mx > 0 ? -1 : 1) * BULLET_SPEED, (y - my > 0 ? -1 : 1) * BULLET_SPEED);

		CPlayScene::GetInstance()->AddNewObject(bullet);
		break;
	}
	case PLANT_STATE_UP:
		vy = -PLANT_VY;
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
	right = left + PLANT_BBOX_WIDTH;
	bottom = top + PLANT_BBOX_HEIGHT;
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