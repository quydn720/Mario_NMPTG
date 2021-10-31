#include "Coin.h"
#include "debug.h"

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y);

	//RenderBoundingBox();
}

// Make the coin bouncing before die
void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (insideBrick) {
		if (state == STATE_ITEM_VISIBLE) {
			y -= COIN_DEFLECT_GRAVITY * dt;
			if (y <= baseY - COIN_OFFSET) {
				SetState(STATE_ITEM_SPAWN);
			}
		}
		else if (state == STATE_ITEM_SPAWN) {
			y += COIN_DEFLECT_GRAVITY * dt;
			if (y >= baseY - 20) {
				SetState(STATE_ITEM_DIE);
			}
		}
	}
	else {
		SetState(STATE_ITEM_VISIBLE);
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CCoin::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_ITEM_INVISIBLE:
		vy = 0;
		break;
	case STATE_ITEM_VISIBLE:
		break;
	case STATE_ITEM_SPAWN:
		break;
	case STATE_ITEM_DIE:
		this->Delete();
		break;
	default:
		break;
	}
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void CCoin::Spawn(int nx)
{
	SetState(STATE_ITEM_VISIBLE);
}
