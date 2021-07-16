#include "Item.h"

Item::Item(ItemType type, float width, float height) : CGameObject() {
	SetBoundingBox(width, height);
	setType(ObjectType::ITEM);
	itemType = type;
	this->x = width;
	this->y = height;
}

void Item::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + widthBoundingBox;
	b = y + heightBoundingBox;
}