#include "Item.h"
#define BB_W_SUPER_ITEM 16
#define BB_H_SUPER_ITEM 16
Item::Item(ItemType type, float width, float height) : CGameObject() {
	setType(ObjectType::ITEM);
	itemType = type;
}

void Item::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (_state == ObjectState::ITEM_VISIBLE) {
		right = left + currentAnimation->getBBWidth();
		bottom = top + currentAnimation->getBBHeight();
	}
	else {
		right = left + widthBoundingBox;
		bottom = top + heightBoundingBox;
	}
}