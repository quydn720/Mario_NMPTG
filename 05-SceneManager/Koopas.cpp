#include "Koopas.h"
#include "debug.h"

CKoopas::CKoopas(float x, float y)
{
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	die_start = -1;
	nx = -1;
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPAS_STATE_DIE)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT_DIE / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT;
	}
}


void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		die_start = GetTickCount64();
		y += (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		nx = 0;
		ay = 0;
		break;
	case KOOPAS_STATE_WALKING:
		vx = nx * KOOPAS_WALKING_SPEED;
		break;
	}
}
void CKoopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopas*>(e->obj)) return;
	if (e->ny != 0)
	{
		vy = 0;
	}
	if (e->nx != 0)
	{
		vx = nx * vx;
		nx = -nx;
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// When it moving on top of the color block, it will not fall when reaching edge
	/*if (fallDetector->willFall) {
		vx = -vx;
		nx = -nx;
	}*/
	vy += ay * dt;
	vx += ax * dt;

	if ((state == KOOPAS_STATE_DIE) && (GetTickCount64() - die_start > KOOPAS_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	// This will put the fall detector in front of the Koopas
	if (vx <= 0)
		fallDetector->SetPosition(x - 10, y);
	if (vx > 0)
		fallDetector->SetPosition(x + 10, y);

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopas::Render()
{
	int aniId = ID_ANI_KOOPAS_WALKING_LEFT;
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		aniId = ID_ANI_KOOPAS_DIE;
		break;
	case KOOPAS_STATE_WALKING:
		if (vx < 0) aniId = ID_ANI_KOOPAS_WALKING_LEFT;
		else aniId = ID_ANI_KOOPAS_WALKING_RIGHT;
		break;
	default: {
		int x = 0;
		break;
	}
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}