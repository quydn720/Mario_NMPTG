#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Item.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_DEFLECT_GRAVITY 0.15f
#define COIN_OFFSET 70

// Mario only collision with coin when state == VISIBLE
class CCoin : public Item {
	int insideBrick;	
public:
	CCoin(float x, float y, int insideB) : Item(x, y, 0) { itemType = ItemType::Coin; insideBrick = insideB; }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsColliable() { return 1; }
	void Spawn(int nx);
};