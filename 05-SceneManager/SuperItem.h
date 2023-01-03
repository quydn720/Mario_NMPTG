#pragma once
#include "GameObject.h"
#include "debug.h"
#include "Goomba.h"
#include "Platform.h"
#include "Item.h"

#define ID_ANI_MUSHROOM_RED 178
#define ID_ANI_MUSHROOM_GREEN 179
#define ID_ANI_LEAF 177

#define ITEM_BBOX_WIDTH 16
#define ITEM_BBOX_HEIGHT 16

#define STATE_ITEM_VISIBLE 20
#define STATE_ITEM_INVISIBLE 21
#define STATE_ITEM_SPAWN 22
#define STATE_ITEM_DIE 23

#define GRAVITY 0.002f
#define TILE_SIZE 16
#define ITEM_DEFLECT_GRAVITY 0.04f
#define LEAF_DEFLECT_GRAVITY 0.08f
#define MUSHROOM_SPEED_X 0.05f
#define MUSHROOM_SPEED_Y 0.02f
#define LEAF_SPEED_Y 0.05f

enum SuperItemType {
	RedMushroom, GreenMushroom, Leaf,
};

// SuperItem can be Mushroom or Leaf based on the level of Mario
class CSuperItem : public Item
{
	int isOnPlatform = 0;
	ULONGLONG timer;
public:
	bool IsAlive = false;
	SuperItemType type = SuperItemType::RedMushroom;

	CSuperItem(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual void SetState(int state);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnNoCollision(DWORD dt);

	void Spawn(int nx);	// Wrapper for SetState with parameter
};

