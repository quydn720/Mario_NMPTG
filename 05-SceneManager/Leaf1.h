#pragma once
#include "Item.h"
class Leaf1 : public Item
{
	Leaf1(float x, float y);
	~Leaf1() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	ItemType GetItemType() { return itemType; }
};

