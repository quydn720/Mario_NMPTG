#include "Coin.h"

Coin::Coin(ItemType type, float w, float h) : Item(type, w, h) {
	itemType = ItemType::COIN;
	setObjectState(ObjectState::ITEM_INVISIBLE);
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	
	if (_state == ObjectState::ITEM_VISIBLE) {
		vy += ITEM_GRAVITY * dt;
	}
	if (vy > 0 && y > destroy) {
		isAlive = false;
	}
	y += dy;
}

void Coin::Render() {
	if (_state == ObjectState::ITEM_VISIBLE) {
		currentAnimation->Render(x, y);
	}
}

void Coin::setObjectState(ObjectState s)
{
	CGameObject::setObjectState(s);
	switch (s) {
	case ObjectState::ITEM_VISIBLE: {
		destroy = y - GAME_UNIT;
		vy = -0.3f;
		setAnimation(CAnimationSets::GetInstance()->Get(4)->at(300));
		break;
	}
	case ObjectState::ITEM_INVISIBLE: {
		setAnimation(CAnimationSets::GetInstance()->Get(4)->at(300));
		vy = 0;
		break;
	}
	}

}
