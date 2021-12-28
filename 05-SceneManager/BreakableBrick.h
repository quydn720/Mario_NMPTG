#pragma once
#include "GameObject.h"
#include "Coin.h"
#include "BreakableBrickPiece.h"
#include "PlayScene.h"
#include "AssetIds.h"

#define BRICK_BBOX_WIDTH	16
#define BRICK_BBOX_HEIGHT	16
#define BREAKABLE_BRICK_STATE_TRANSFORMS_COIN	1
#define BREAKABLE_BRICK_STATE_BREAK_DOWN	2
#define BREAKABLE_BRICK_STATE_CREATE_BUTTON	3
#define COIN_STATE_TRANSFORMS_BRICK	4

#define ID_ANI_BREAKABLE_BRICK 10001
#define ID_ANI_BREAKABLE_BRICK_IS_UP	10002

#define INNIT_VX_BREAKABLE_BRICK_EFFECT	0.05
#define INNIT_VY_BREAKABLE_BRICK_EFFECT	0.1

#define BREAKBLE_BRICK_VY	0.05f
class BreakableBrick :
	public CGameObject
{
public:
	int isBlocking;
	float startY;
	bool InitCoin;
	ULONGLONG ChangeBackToBrickTime;
	bool haveButton;
	bool buttonCreated, isBreakDown;
	BreakableBrickEffect* piece1;
	BreakableBrickEffect* piece2;
	BreakableBrickEffect* piece3;
	BreakableBrickEffect* piece4;
	BreakableBrick(float x, float y, bool HaveButton) : CGameObject(x, y) {
		startY = y;
		haveButton = HaveButton;
		buttonCreated = false;
		vy = 0;
		InitCoin = isBreakDown = false;
		isBlocking = 1;
		}
	void Render();
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return isBlocking; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		if (!isBreakDown)
		{
			l = x - BRICK_BBOX_WIDTH / 2;
			t = y - BRICK_BBOX_HEIGHT / 2;
			r = l + BRICK_BBOX_WIDTH;
			b = t + BRICK_BBOX_HEIGHT;
		}
	};

	void SetState(int state) {
		switch (state) {
		case BREAKABLE_BRICK_STATE_TRANSFORMS_COIN:
			objType = OBJECT_TYPE_COIN;
			ChangeBackToBrickTime = GetTickCount64();
			isBlocking = 0;
			InitCoin = true;
			break;
		case BREAKABLE_BRICK_STATE_BREAK_DOWN:
			isBreakDown = true;
			break;
		case BREAKABLE_BRICK_STATE_CREATE_BUTTON:
			/*buttonCreated = true;
			vy = -BREAKBLE_BRICK_VY;
			ButtonP::GetInstance()->SetPosition(x, y - BRICK_BBOX_HEIGHT);
			ButtonP::GetInstance()->isCreated = true;*/
			break;
		case COIN_STATE_TRANSFORMS_BRICK:
			isBlocking = 1;
			break;
		default:break;
		}
		CGameObject::SetState(state);
	}
};

