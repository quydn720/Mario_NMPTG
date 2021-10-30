#include "Mushroom.h"
#include "Goomba.h"
#include "Platform.h"
#include "Item.h"

CMushroom::CMushroom(float x, float y) : CGameObject(x, y)
{
	//this->itemType = ItemType::Mushroom;
	baseY = y;
	ax = 0;
	ay = GRAVITY;
	SetState(STATE_MUSHROOM_INVISIBLE);
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vx += ax * dt;
	if (state == STATE_MUSHROOM_VISIBLE) {
		y -= ITEM_DEFLECT_GRAVITY * dt;
		if (y <= baseY - TILE_SIZE) {
			SetState(STATE_MUSHROOM_SPAWN);
		}
	}
	if (state == STATE_MUSHROOM_SPAWN) {
		vy += ay * dt;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;
	if (dynamic_cast<CMushroom*>(e->obj)) return;
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	if (e->ny != 0) {
		vy = 0;
	}
	else if (e->nx != 0) {
		vx = -vx;
	}
}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_MUSHROOM_INVISIBLE:
		vy = 0;
		break;
	case STATE_MUSHROOM_VISIBLE:
		break;
	case STATE_MUSHROOM_SPAWN:
		vx = MUSHROOM_SPEED_X * nx;
		IsAlive = true;
		break;
	case STATE_MUSHROOM_DIE:
		IsAlive = false;
		break;
	default:
		break;
	}
}

void CMushroom::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	if (state == STATE_MUSHROOM_VISIBLE || state == STATE_MUSHROOM_SPAWN)
		animations->Get(ID_ANI_MUSHROOM_RED)->Render(x, y);
	RenderBoundingBox();
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - ITEM_BBOX_WIDTH / 2;
	t = y - ITEM_BBOX_HEIGHT / 2;
	r = l + ITEM_BBOX_WIDTH;
	b = t + ITEM_BBOX_HEIGHT;
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushroom::Spawn(int nx)
{
	DebugOut(L"item nx: %d\n", nx);
	this->nx = nx;
	SetState(STATE_MUSHROOM_VISIBLE);
}
