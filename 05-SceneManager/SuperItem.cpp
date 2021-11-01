#include "SuperItem.h"

CSuperItem::CSuperItem(float x, float y) : Item(x, y, 0)
{
	itemType = ItemType::SuperItem;
	baseY = y;
	ay = GRAVITY;
	SetState(STATE_MUSHROOM_INVISIBLE);
}

void CSuperItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
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

void CSuperItem::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;
	if (dynamic_cast<CSuperItem*>(e->obj)) return;
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

void CSuperItem::SetState(int state)
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

void CSuperItem::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	if (state == STATE_MUSHROOM_VISIBLE || state == STATE_MUSHROOM_SPAWN) {
		if (type == 1)
			animations->Get(ID_ANI_MUSHROOM_RED)->Render(x, y);
		else if (type == 2)
			animations->Get(ID_ANI_MUSHROOM_GREEN)->Render(x, y);
	}
	RenderBoundingBox();
}

void CSuperItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - ITEM_BBOX_WIDTH / 2;
	t = y - ITEM_BBOX_HEIGHT / 2;
	r = l + ITEM_BBOX_WIDTH;
	b = t + ITEM_BBOX_HEIGHT;
}

void CSuperItem::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CSuperItem::Spawn(int nx)
{
	this->nx = nx;
	SetState(STATE_MUSHROOM_VISIBLE);
}
