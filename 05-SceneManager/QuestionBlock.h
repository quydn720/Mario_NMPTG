#pragma once
#include "GameObject.h"
#include "Item.h"

#define ID_ANI_QUESTION_BRICK 727
#define ID_ANI_QUESTION_BRICK_EMPTY 728
#define QBRICK_BBOX_WIDTH 16
#define QBRICK_BBOX_HEIGHT 16

#define QBRICK_BOUND_OFFSET 10

#define STATE_BRICK_HIT	310
#define STATE_BRICK_EMPTY 311
#define STATE_BRICK_INIT 312


class CQuestionBlock : public CGameObject
{
	bool isEmpty = false;
	CItem* item = NULL;
	float baseY;
public:
	CQuestionBlock(float x, float y);
	void Render();
	void spawnItem();
	void setItem(CItem* i);
	CItem* getItem();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	//virtual void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

