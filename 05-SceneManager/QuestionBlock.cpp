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
	else animations->Get(ID_ANI_QUESTION_BRICK_EMPTY)->Render(x, y);
}

void CQuestionBlock::spawnItem()
{
	SetState(STATE_BRICK_HIT);
	if (isEmpty == false) {
		isEmpty = true;
		getItem()->SetState(STATE_MUSHROOM_VISIBLE);
	}
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	DebugOut(L"y: %0.1f\n", y);
	if (state == STATE_BRICK_HIT) {
		vy -= 0.03f;
		if (y < baseY - 10) {
			vy = 0.03f;
			SetState(STATE_BRICK_EMPTY);
		}
	}
	else if (state == STATE_BRICK_EMPTY) {
		if (y >= baseY) {
			vy = 0;
			y = baseY;
		}
	}
	CGameObject::Update(dt, coObjects);
}
//void CQuestionBlock::SetState(int state)
//{
//}
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