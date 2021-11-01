#pragma once
#include "GameObject.h"

#define STATE_ITEM_VISIBLE 20
#define STATE_ITEM_INVISIBLE 21
#define STATE_ITEM_SPAWN 22
#define STATE_ITEM_DIE 23

enum ItemType {
	Undefined = -1,
	SuperItem,	// Red Mushroom, Green Mushroom, Leaf
	Coin,		
};

// Interface for items
class Item : public CGameObject
{
protected:
	ItemType itemType = ItemType::Undefined;
	float ay;		// Gravity
	float baseY;	// For offset calculating when init item
public:
	Item(float x, float y, int type) : CGameObject(x, y) { baseY = y;  ay = 0; itemType = (ItemType)type; }
	~Item() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {}
	virtual void Render() {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	virtual void SetState(int state) {};
	ItemType GetItemType() { return itemType; }
	// Function for changing the state from Invisible to Visible, for now, mario only collide with item when it fully init
	virtual void Spawn(int nx) {}
	// Every item are colliable & not blocking
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
};
