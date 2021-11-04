#include "FallDetector.h"

void FallDetector::Render() {
	if (state != FALL_DETECTOR_STATE_INACTIVE) RenderBoundingBox();
}

void FallDetector::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BBOX_WIDTH / 2;
	top = y - height / 2;
	right = left + BBOX_WIDTH;
	bottom = top + height;
}

void FallDetector::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state != FALL_DETECTOR_STATE_INACTIVE) {
		vy += ay * dt;
		vx += ax * dt;
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
}
void FallDetector::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (!e->obj->IsBlocking()) return;
	if (e->ny != 0)
	{
		vy = 0;
		willFall = false;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void FallDetector::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	willFall = true;
}
