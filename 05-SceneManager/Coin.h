#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Item.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_VY 0.10f
#define COIN_OFFSET_UP 50
#define COIN_OFFSET_DOWN 20 

#define COIN_APPEAR_TIME 3000

#define COIN_STATE_FLY 453
#define COIN_STATE_FALL 454
#define COIN_STATE_NORMAL 455
// state waiting to change back to brick
#define COIN_STATE_WAIT 456

class CCoin : public Item {
	int insideBrick;	
public:
	// các biến hỗ trợ việc tiền biến thành gạch lại như cũ
	bool isBrickToCoin;
	ULONGLONG AppearTime;

	CCoin(float x, float y);
	CCoin(float x, float y, int insideB) : Item(x, y, -1) 
	{
		itemType = ItemType::Coin; insideBrick = -1; SetState(STATE_ITEM_VISIBLE);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};