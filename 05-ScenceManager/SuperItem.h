#pragma once
#include "Item.h"
class SuperItem : public Item {
public:
	SuperItem(ItemType type, float w, float h);
	~SuperItem() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void setState(ObjectState s, int marioLevel = 0, int mario_nx = 0);
};