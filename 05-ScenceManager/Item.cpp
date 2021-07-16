#include "Item.h"

Item::Item(ItemType type, float x, float y) : CGameObject() {
	SetBoundingBox(x, y);
	setType(ObjectType::ITEM);
	itemType = type;
	this->x = x;
	this->y = y;
}

void Item::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + widthBoundingBox;
	b = y + heightBoundingBox;
}