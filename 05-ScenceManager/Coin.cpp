#include "Coin.h"

Coin::Coin(ItemType type, float w, float h) : Item(type, w, h) {
	itemType = ItemType::COIN;
	SetState(9998);
	// Fix this hard-code
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	
	if (state == 9999) {
		vy += COIN_GRAVITY * dt;
	}
	if (vy > 0 && y > destroy) {
		isAlive = false;
		this->~Coin();
	}
	y += dy;
}
void Coin::Render() {
	if (state == 9999) {
		currentAnimation->Render(x, y);
	}
}

void Coin::SetState(int s)
{
	CGameObject::SetState(s);
	switch (s) {
	case 9999: {
		destroy = y - GAME_UNIT;
		vy = -0.3f;
		setAnimation(CAnimationSets::GetInstance()->Get(4)->at(0));
		break;
	}
	case 9998: {
		setAnimation(CAnimationSets::GetInstance()->Get(4)->at(0));
		vy = 0;
		break;
	}
	}

}
