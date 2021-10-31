#pragma once
#include "GameObject.h"

#define STATE_ITEM_VISIBLE 20
#define STATE_ITEM_INVISIBLE 21
#define STATE_ITEM_SPAWN 22
#define STATE_ITEM_DIE 23

enum ItemType {
	Undefined = -1,
	SuperItem,
	Coin,
};

class Item : public CGameObject
{
protected:
	ItemType itemType = ItemType::Undefined;
	float ax, ay;
	float baseY;
public:
	Item(float x, float y, int type) : CGameObject(x, y) { baseY = y; ax = ay = 0; itemType = (ItemType) type; }
	~Item() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {}
	virtual void Render() {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	virtual void SetState(int state) {};
	ItemType GetItemType() { return itemType; }
	// Function for changing the state from Invisible to Visible
	virtual void Spawn(int nx) {}
	// Every item are colliable & not blocking
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
};
