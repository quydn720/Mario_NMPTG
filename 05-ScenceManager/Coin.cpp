#include "Coin.h"

Coin::Coin(ItemType type, float x, float y) : Item(type, x, y) {
	itemType = ItemType::COIN;
	setAnimation(CAnimationSets::GetInstance()->Get(4)->at(0));
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);

	vy += 0.000003f * dt;
	y += dy;
}
void Coin::Render() {
	RenderBoundingBox();
	currentAnimation->Render(x, y);
}