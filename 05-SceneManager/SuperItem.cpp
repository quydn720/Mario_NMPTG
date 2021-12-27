#include "SuperItem.h"

CSuperItem::CSuperItem(float x, float y) : Item(x, y, 0)
{
	itemType = ItemType::SuperItem;
	baseY = y;
	ay = GRAVITY;
	SetState(STATE_ITEM_INVISIBLE);
}

void CSuperItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (state == STATE_ITEM_VISIBLE) {
		if (type != SuperItemType::Leaf) {
			y -= ITEM_DEFLECT_GRAVITY * dt;
			if (y <= baseY - TILE_SIZE) {
				SetState(STATE_ITEM_SPAWN);
			}
		}
		// if this a leaf
		else {
			y -= LEAF_DEFLECT_GRAVITY * dt;
			if (y <= baseY - TILE_SIZE * 4) {	// How far the leaf will pop up before drop down
				SetState(STATE_ITEM_SPAWN);
			}
		}

	}
	if (state == STATE_ITEM_SPAWN) {
		vy += ay * dt;

		if (type == SuperItemType::Leaf) {
			vy = LEAF_SPEED_Y;
			if (GetTickCount64() - timer >= 500) {
				vx = -vx;
				timer = GetTickCount64();
			}

		}
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CSuperItem::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (type == SuperItemType::Leaf) {
		if (e->obj->IsBlocking()) {
			vy = 0.04f;
		}
	}
	else {
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
}

void CSuperItem::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_ITEM_INVISIBLE:
		vy = 0;
		break;
	case STATE_ITEM_VISIBLE:
		break;
	case STATE_ITEM_SPAWN:
		timer = GetTickCount64();
		vx = MUSHROOM_SPEED_X * nx;
		IsAlive = true;
		break;
	case STATE_ITEM_DIE:
		IsAlive = false;
		break;
	default:
		break;
	}
}

void CSuperItem::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	if (state == STATE_ITEM_VISIBLE || state == STATE_ITEM_SPAWN) {
		if (type == SuperItemType::RedMushroom)
			animations->Get(ID_ANI_MUSHROOM_RED)->Render(x, y);
		else if (type == SuperItemType::GreenMushroom)
			animations->Get(ID_ANI_MUSHROOM_GREEN)->Render(x, y);
		else if (type == SuperItemType::Leaf)
			(vx > 0) ? animations->Get(ID_ANI_LEAF)->Render(x, y) : animations->Get(173)->Render(x, y);;
	}
	//RenderBoundingBox();
}

void CSuperItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - ITEM_BBOX_WIDTH / 2;
	t = y - ITEM_BBOX_HEIGHT / 2;
	r = l + ITEM_BBOX_WIDTH;
	b = t + ITEM_BBOX_HEIGHT - 1;
}

void CSuperItem::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CSuperItem::Spawn(int nx)
{
	this->nx = nx;
	SetState(STATE_ITEM_VISIBLE);
}
