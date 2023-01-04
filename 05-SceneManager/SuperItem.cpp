#include "SuperItem.h"

CSuperItem::CSuperItem(float x, float y) : Item(x, y, 0)
{
	itemType = ItemType::SuperItem;
	baseY = y;
	baseX = x;
	SetState(STATE_ITEM_INVISIBLE);
}

void CSuperItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	//if (x < 0 || x > Get) this->Delete();

	if (type == SuperItemType::RedMushroom || type == SuperItemType::GreenMushroom) {
		DebugOut(L"item spawn vx: %0.4f x: %0.4f\n", vx, x);
		if (state == STATE_ITEM_SPAWN) {
			vy += GRAVITY * dt;
			CCollision::GetInstance()->Process(this, dt, coObjects);

		}
		else if (state == STATE_ITEM_VISIBLE) {
			y += -ITEM_SPEED_Y * dt;
			if (baseY - y > TILE_SIZE) {
				SetState(STATE_ITEM_SPAWN);
			}
		}
	}
	else if (type == SuperItemType::Leaf) {
		DebugOut(L"item spawn vx: %0.4f x: %0.4f\n", vx, x);

		if (state == STATE_ITEM_SPAWN) {
			if (abs(x - baseX) >= 1.5 * TILE_SIZE) {
				vx = -vx;
			}
			x += vx * dt;
			y += ITEM_SPEED_Y * dt;
		}
		else if (state == STATE_ITEM_VISIBLE) {
			y += 1.5 * -ITEM_SPEED_Y  * dt;
			if (baseY - y > TILE_SIZE * 3) {
				SetState(STATE_ITEM_SPAWN); // la' roi
			}
		}
	}
}

void CSuperItem::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}

	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
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
		vx = ITEM_SPEED_X * nx;
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
