#pragma once
#include "GameObject.h"
#include "Block.h"

enum class ItemType {
	COIN,
	SUPER_ITEM,
};

class Item : public CGameObject {
protected:
	ItemType itemType;
public: 
	Item(ItemType type, float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	ItemType getItemType() { return itemType; }
};

