#include "QuestionBlock.h"
#include "debug.h"

CQuestionBlock::CQuestionBlock(float x, float y) : CGameObject(x, y) {
	state = STATE_BRICK_INIT;
	baseY = y;
}

void CQuestionBlock::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (!isEmpty) animations->Get(ID_ANI_QUESTION_BRICK)->Render(x, y);
	else
		animations->Get(ID_ANI_QUESTION_BRICK_EMPTY)->Render(x, y);
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	if (state == STATE_BRICK_HIT) {
		if (y < baseY - 10) {
			vy = BRICK_OFFSET;
			SetState(STATE_BRICK_EMPTY);
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
		item->SetState(STATE_MUSHROOM_VISIBLE);
		break;
	case STATE_BRICK_HIT:
		vy -= BRICK_OFFSET;
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

void CQuestionBlock::setItem(CItem* i)
{
	item = i;
}
CItem* CQuestionBlock::getItem()
{
	return item;
}