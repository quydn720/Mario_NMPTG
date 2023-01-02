#include "Coin.h"
#include "debug.h"
#include "BreakableBrick.h"

CCoin::CCoin(float x, float y) : Item(x, y, 0)
{
	itemType = ItemType::Coin;
	vx = vy = 0.0f;
	isBrickToCoin = false;
	AppearTime = 0;
	insideBrick = -1;
}

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y);
}

// Make the coin bouncing before die
void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	y += vy * dt;

	switch (state) {
	case COIN_STATE_FLY: {
		if (y <= baseY - COIN_OFFSET_UP) {
			SetState(COIN_STATE_FALL);
		}
		break;
	}
	case COIN_STATE_FALL: {
		if (y >= baseY - COIN_OFFSET_DOWN) {
			SetState(STATE_ITEM_DIE);
		}
		break;
	}
	case COIN_STATE_NORMAL: {
		break;
	}
	}

	if (state==COIN_STATE_WAIT && GetTickCount64() - AppearTime >= 3000) {
		this->Delete();
		CQuestionBlock* item = new CQuestionBlock(x, y, 0);
		CPlayScene::GetInstance()->AddNewObject(item);

		DebugOut(L"COIN_DELETED_TO_BRICK\n");
	}

	/*else {
		SetState(STATE_ITEM_VISIBLE);
		if (isBrickToCoin == true && GetTickCount64() - AppearTime >= COIN_APPEAR_TIME)
		{
			BreakableBrick* itembrick = new BreakableBrick(this->x, this->y, false);

			_PlayScene->objects.push_back(itembrick);
			this->isDeleted = true;
			isBrickToCoin = false;
			AppearTime = 0;
		}
	}*/
	CGameObject::Update(dt, coObjects);
}

void CCoin::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_ITEM_INVISIBLE:
		break;
	case COIN_STATE_FLY:
		vy = -COIN_VY;
		break;
	case COIN_STATE_FALL:
		vy = COIN_VY;
		break;
	case COIN_STATE_WAIT:
		AppearTime = GetTickCount64();
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