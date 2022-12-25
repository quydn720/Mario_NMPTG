#include "QuestionBlock.h"
#include "debug.h"
#include "PlayScene.h"

CQuestionBlock::CQuestionBlock(float x, float y, int itemType) : CGameObject(x, y) {
	state = STATE_BRICK_INIT;
	this->itemType = itemType;
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

	RenderBoundingBox();
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	y += vy * dt;

	if (state == STATE_BRICK_HIT) {
		// gạch nhảy lên khi bị mario đụng

		if (y < baseY - QBRICK_BOUND_OFFSET) {
			vy = BRICK_MOVING_SPEED;
			SetState(STATE_BRICK_EMPTY);
		}
	}
	else if (state == STATE_BRICK_EMPTY) {
		if (y > baseY) {
			vy = 0;
			y = baseY;
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
		{
			vy = BRICK_MOVING_SPEED;
			isEmpty = true;
			break;
		}
		case STATE_BRICK_HIT:
		{
			SpawnItem();
			vy = -BRICK_MOVING_SPEED;
			break;
		}
		default:
		{
			isEmpty = false;
			break;
		}
	}
}
void CQuestionBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QBRICK_BBOX_WIDTH / 2;
	t = y - QBRICK_BBOX_HEIGHT / 2;
	r = l + QBRICK_BBOX_WIDTH;
	b = t + QBRICK_BBOX_HEIGHT;
}

CCoin* CQuestionBlock::SpawnCoin()
{
	CCoin* coin = new CCoin(x, y);
	coin->SetState(COIN_STATE_FLY);

	return coin;
}

CSuperItem* CQuestionBlock::SpawnSuperItem() {
	return new CSuperItem(x, y);
}


void CQuestionBlock::SpawnItem()
{
	// TODO: bug:
	// - thứ tự render, render gạch trước, nấm sau để có cảm giác nấm chui dưới gạch lên
	// - nấm di chuyến sau khi gạch đã về chỗ cũ, khác với coin
	Item* item = NULL;
	switch (itemType)
	{
		case ItemType::Coin: {
			item = SpawnCoin();
			break;
		}
		case ItemType::SuperItem: {
			item = SpawnSuperItem();
			break;
		}
		default:
		{
			break;
		}
	}
	CPlayScene::GetInstance()->AddNewObject(item);
}
