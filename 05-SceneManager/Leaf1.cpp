#include "Leaf1.h"
#include "debug.h"

Leaf1::Leaf1(float x, float y) : Item(x, y, 0) {
	itemType = ItemType::Coin;
}

void Leaf1::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	DebugOut(L"Leaf");
}
void Leaf1::Render(){}
void Leaf1::GetBoundingBox(float& l, float& t, float& r, float& b){}
void Leaf1::SetState(int state){}