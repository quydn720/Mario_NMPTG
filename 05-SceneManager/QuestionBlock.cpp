#include "QuestionBlock.h"
#include "debug.h"

CQuestionBlock::CQuestionBlock(float x, float y) : CGameObject(x, y) {
	state = STATE_BRICK_INIT;
	baseY = y;
}

void CQuestionBlock::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == STATE_BRICK_EMPTY || state == STATE_BRICK_HIT) {
		animations->Get(ID_ANI_QUESTION_BRICK_EMPTY)->Render(x, y);
	}
	else {
		animations->Get(ID_ANI_QUESTION_BRICK)->Render(x, y);
	}
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	if (state == STATE_BRICK_HIT) {
		switch (item->GetItemType()) {
		case ItemType::SuperItem: {
			if (y < baseY - QBRICK_BOUND_OFFSET) {
				vy = BRICK_MOVING_SPEED;
				SetState(STATE_BRICK_EMPTY);
			}
			break;
		}
		case ItemType::Coin: {
			// The coin doesn't need to wait the brick to finish moving
			item->Spawn(0);
			if (y < baseY - QBRICK_BOUND_OFFSET) {
				vy = BRICK_MOVING_SPEED;
				SetState(STATE_BRICK_EMPTY);
			}
			break;
		}
		}

	}
	CGameObject::Update(dt, coObjects);
}
void CQuestionBlock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_BRICK_EMPTY:
		vy = 0;
		y = baseY;
		isEmpty = true;
		if (item->GetItemType() != ItemType::Coin) {
			item->Spawn(nx);
		}
		break;
	case STATE_BRICK_HIT:
		vy -= BRICK_MOVING_SPEED;
		break;
	default:
		isEmpty = false;
		break;
	}
}
void CQuestionBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QBRICK_BBOX_WIDTH / 2;
	t = y - QBRICK_BBOX_HEIGHT / 2;
	r = l + QBRICK_BBOX_WIDTH;
	b = t + QBRICK_BBOX_HEIGHT;
}

void CQuestionBlock::SpawnItem(int nx, int l)
{
	SetState(STATE_BRICK_HIT);
	this->nx = -nx;
	if (item->GetItemType() == ItemType::SuperItem)
		dynamic_cast<CSuperItem*>(item)->type = l == MARIO_LEVEL_SMALL ? 2 : 1;
}

void CQuestionBlock::setItem(Item* i)
{
	item = i;
}
Item* CQuestionBlock::getItem()
{
	return item;
}