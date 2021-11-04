#include "Goomba.h"
#include "ColorBlock.h"
#include "debug.h"
#include "PlayScene.h"

CGoomba::CGoomba(float x, float y, int l) :CGameObject(x, y)
{
	level = l;
	step = 0;
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	nx = -1;
	SetState(GOOMBA_STATE_WALKING);
}


void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == LEVEL_PARA_GOOMBA) {
		left = x - PARA_GOOMBA_BBOX_WIDTH / 2;
		top = y - PARA_GOOMBA_BBOX_HEIGHT / 2;
		right = left + PARA_GOOMBA_BBOX_WIDTH;
		bottom = top + PARA_GOOMBA_BBOX_HEIGHT;
	}
	else if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;
	if (e->ny != 0)
	{
		vy = 0;
	}
	if (e->nx != 0)
	{
		vx = nx * vx;
		nx = -nx;
	}
	if (state == GOOMBA_STATE_JUMPING && level == LEVEL_PARA_GOOMBA)
	{
		if (e->ny < 0) vy = -GOOMBA_JUMPING_SPEED;
	}
}

void CGoomba::SetLevel(int l)
{
	level = l;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	if (level == LEVEL_PARA_GOOMBA) CalcGoombaMove();

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int aniId = -1;
	if (level == LEVEL_PARA_GOOMBA) {
		aniId = 5002;
	}
	else {
		aniId = ID_ANI_GOOMBA_WALKING;
	}

	if (state == GOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		nx = 0;
		ay = 0;
		break;
	case GOOMBA_STATE_WALKING:
		vx = nx * GOOMBA_WALKING_SPEED;
		break;
	case GOOMBA_STATE_FLYING:
		vy = -GOOMBA_FLYING_SPEED;
		break;
	}
}

void CGoomba::CalcGoombaMove() {
	// 0: walking	1: jumping	2: flying
	//float a, b;
	//dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetPlayer()->GetPosition(a, b);
	////DebugOut(L"mario - goomba = %0.2f\n", a - x);
	//if (a < x) vx = -vx;
	//

	switch (step) {
	case 0:
	{
		if (timeEllapsed == 0) {
			timeEllapsed = GetTickCount64();
		}
		else if (GetTickCount64() - timeEllapsed > GOOMBA_WALK_DURATION) {
			timeEllapsed = 0;
			step = 1;
		}
		break;
	}
	case 1: {
		if (timeEllapsed == 0) {
			timeEllapsed = GetTickCount64();
			SetState(GOOMBA_STATE_JUMPING);
		}
		else if (GetTickCount64() - timeEllapsed > GOOMBA_JUMP_DURATION) {
			timeEllapsed = 0;
			step = 2;
		}
		break;
	}
	case 2: {
		if (timeEllapsed == 0) {
			timeEllapsed = GetTickCount64();
			SetState(GOOMBA_STATE_FLYING);
		}
		else if (GetTickCount64() - timeEllapsed > GOOMBA_FLY_DURATION) {
			timeEllapsed = 0;
			step = 0;
		}
		break;
	}
	}
}