#pragma once
#include "GameObject.h"
#include "debug.h"
#include "Goomba.h"
#include "Platform.h"
#include "Item.h"

#define ID_ANI_MUSHROOM_RED 178
#define ID_ANI_MUSHROOM_GREEN 179

#define ITEM_BBOX_WIDTH 16
#define ITEM_BBOX_HEIGHT 16

#define STATE_MUSHROOM_VISIBLE 20
#define STATE_MUSHROOM_INVISIBLE 21
#define STATE_MUSHROOM_SPAWN 22
#define STATE_MUSHROOM_DIE 23

#define GRAVITY 0.002f
#define TILE_SIZE 16
#define ITEM_DEFLECT_GRAVITY 0.04f
#define MUSHROOM_SPEED_X 0.05f

// type 0: mushroom, 1: leaf
class CSuperItem : public Item
{
	int isOnPlatform = 0;
public:
	bool IsAlive = false;
	int type = 0;

	CSuperItem(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual void SetState(int state);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnNoCollision(DWORD dt);

	void Spawn(int nx);	// Wrapper for SetState with parameter
};

