#pragma once
#include "Item.h"

#define SUPER_MUSHROOM_VX		0.1f
#define SUPER_MUSHROOM_INIT_VY	0.2f

class SuperItem : public Item {
public:
	SuperItem(ItemType type, float w, float h);
	~SuperItem() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void setState(ObjectState s, int marioLevel = 0, int mario_nx = 0);
};