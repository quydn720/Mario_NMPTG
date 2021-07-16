#include "SuperItem.h"

SuperItem::SuperItem(ItemType type, float w, float h) : Item(type, w, h) {
	itemType = ItemType::SUPER_ITEM;
	setState(ObjectState::SUPER_ITEM_INVISIBLE);
}

void SuperItem::Render() {
	RenderBoundingBox();
	if (state == ObjectState::SUPER_ITEM_VISIBLE) {
		currentAnimation->Render(x, y);
	}
}

void SuperItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);

	if (state == ObjectState::SUPER_ITEM_VISIBLE) {
		vy += ITEM_GRAVITY * dt;

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() == 0) {
			y += dy;
		}
		else {
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
			for (UINT i = 0; i < coEventsResult.size(); i++) {
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<Block*>(e->obj)) {
					if (e->ny != 0) {
						vy = 0;
						x += min_tx * dx + nx * 0.4f;
					}
				}
			}
		}
	}
}

// should have argument: int level to figure what animation
void SuperItem::setState(ObjectState s, int marioLevel, int mario_x)
{
	state = s;
	switch (s) {
	case ObjectState::SUPER_ITEM_INVISIBLE: {
		setAnimation(CAnimationSets::GetInstance()->Get(5)->at(0));
		vy = 0;
		break;
	}
	case ObjectState::SUPER_ITEM_VISIBLE: {
		LPANIMATION ani = (marioLevel == 2) ?
			CAnimationSets::GetInstance()->Get(5)->at(0) : // Super Mushroom
			CAnimationSets::GetInstance()->Get(5)->at(1);  // Super Leaf
		setAnimation(ani);
		if (x <= mario_x) {
			nx = -1;
			vx = -SUPER_MUSHROOM_VX;
		}
		else {
			nx = 1;
			vx = SUPER_MUSHROOM_VX;
		}
		vy -= SUPER_MUSHROOM_INIT_VY;
		break;
	}
	}
}