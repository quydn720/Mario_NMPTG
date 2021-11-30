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
	if (state == KOOPAS_STATE_WALKING)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT;
	}
	else 
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT_SHELL / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT_SHELL;
	}
}


void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_SHELL_MOVING:
		vx = nx * 0.1f;
		nx = -nx;
		fallDetector->SetState(FALL_DETECTOR_STATE_INACTIVE);
		break;
	case KOOPAS_STATE_SHELL:
		y += (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2;
		vx = 0;
		fallDetector->SetState(FALL_DETECTOR_STATE_INACTIVE);
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
	if (fallDetector->willFall) {
		vx = -vx;
		nx = -nx;
	}

	vy += ay * dt;
	vx += ax * dt;

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
	case KOOPAS_STATE_SHELL:
		aniId = ID_ANI_KOOPAS_SHELL;
		break;
	case KOOPAS_STATE_SHELL_MOVING:
		aniId = ID_ANI_KOOPAS_SHELL_MOVING;
		break;
	case KOOPAS_STATE_WALKING:
		if (vx < 0) aniId = ID_ANI_KOOPAS_WALKING_LEFT;
		else aniId = ID_ANI_KOOPAS_WALKING_RIGHT;
		break;
	default: {
		break;	
	}
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}