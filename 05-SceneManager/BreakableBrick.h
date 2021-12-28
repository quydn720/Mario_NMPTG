#pragma once
#include "GameObject.h"
#include "Coin.h"
#include "BreakableBrickPiece.h"
#include "PlayScene.h"
#include "AssetIds.h"

#define BRICK_BBOX_WIDTH	16
#define BRICK_BBOX_HEIGHT	16
//#define BREAKABLE_BRICK_STATE_BREAK_DOWN	1
#define BRICK_STATE_NORMAL		0
#define BRICK_STATE_COLLISION	1
#define BRICK_STATE_EMPTY		2
#define BRICK_STATE_DIE		3

#define ID_ANI_BREAKABLE_BRICK 10001
#define ID_ANI_BREAKABLE_BRICK_IS_UP	10002

#define INNIT_VX_BREAKABLE_BRICK_EFFECT	0.05
#define INNIT_VY_BREAKABLE_BRICK_EFFECT	0.1

#define BREAKBLE_BRICK_VY	0.08f
#define MAX_HEIGHT	8

#define TYPE_NORMAL	0
#define TYPE_BUTTON	1

class BreakableBrick :
	public CGameObject
{
public:
	bool isCollision;
	bool hasItem;
	int Item;
	int isBlocking;
	float startY;
	ULONGLONG ChangeBackToBrickTime;
	BreakableBrickEffect* piece1;
	BreakableBrickEffect* piece2;
	BreakableBrickEffect* piece3;
	BreakableBrickEffect* piece4;
	BreakableBrick(float x, float y, int item) : CGameObject(x, y) {
		startY = y;
		Item = item;
		vy = 0;
		if (Item != 0)
			hasItem = true;
		else
			hasItem = false;
		isBlocking = 1;
		}
	void Render();
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return isBlocking; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b) 
	{
		{
			l = x - BRICK_BBOX_WIDTH / 2;
			t = y - BRICK_BBOX_HEIGHT / 2;
			r = l + BRICK_BBOX_WIDTH;
			b = t + BRICK_BBOX_HEIGHT;
		}
	};
	void SetState(int state);
	
};

