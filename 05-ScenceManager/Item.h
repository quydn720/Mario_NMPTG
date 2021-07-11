#pragma once
#include "GameObject.h"

enum class ItemType {
	COIN,
	SUPER_MUSHROOM,
	SUPER_LEAF
};

class Item : public CGameObject {
protected:
	ItemType itemType;
public: 
	Item();
	Item(ItemType type, float x, float y);
	~Item();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {}
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	ItemType getItemType() { return itemType; }
};

