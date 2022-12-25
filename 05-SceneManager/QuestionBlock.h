#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "SuperItem.h"
#include "Coin.h"

#define ID_ANI_QUESTION_BRICK 727
#define ID_ANI_QUESTION_BRICK_EMPTY 728
#define QBRICK_BBOX_WIDTH 16
#define QBRICK_BBOX_HEIGHT 16

#define QBRICK_BOUND_OFFSET 10

#define STATE_BRICK_HIT	310
#define STATE_BRICK_EMPTY 311
#define STATE_BRICK_INIT 312

#define BRICK_MOVING_SPEED 0.07f

class CQuestionBlock : public CGameObject
{
	bool isEmpty = false;
	float baseY;
	int itemType = 0;
	
	CSuperItem* SpawnSuperItem();
	CCoin* SpawnCoin();

public:
	CQuestionBlock(float x, float y, int itemType);
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SpawnItem();

};

