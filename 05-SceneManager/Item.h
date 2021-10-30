#pragma once
#include "GameObject.h"
#include "debug.h"

#define ID_ANI_MUSHROOM_RED 178
#define ITEM_BBOX_WIDTH 16
#define ITEM_BBOX_HEIGHT 16

#define STATE_MUSHROOM_VISIBLE 20
#define STATE_MUSHROOM_INVISIBLE 21
#define STATE_MUSHROOM_SPAWN 22
#define STATE_MUSHROOM_DIE 23

#define GRAVITY 0.002f
#define TILE_SIZE 16
#define ITEM_DEFLECT_GRAVITY 0.007f
#define MUSHROOM_SPEED_X 0.05f

class CItem : public CGameObject
{
	float ax, ay;
	float baseY;
	int isOnPlatform = 0;
	bool alive = false;
public:
	bool IsVisible = false;
	bool IsAlive = false;

	CItem(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void SetState(int state);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnNoCollision(DWORD dt);
};

