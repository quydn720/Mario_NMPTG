#include "Item.h"

class Coin : public Item {
public:
	Coin(ItemType type, float x, float y);
	~Coin() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	ItemType getItemType() { return itemType; }
};