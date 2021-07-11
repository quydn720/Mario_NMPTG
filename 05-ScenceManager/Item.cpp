#include "Item.h"

Item::Item() : CGameObject()
{
	itemType = ItemType::COIN;
}

Item::Item(ItemType type, float x, float y) {
	SetBoundingBox(x, y);
	setType(ObjectType::ITEM);
	itemType = type;
	this->x = x;
	this->y = y;
}

Item::~Item()
{
}

//void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
//{
//	CGameObject::Update(dt);
//}

void Item::Render()
{
	//
	RenderBoundingBox();
}

void Item::SetState(int state)
{
	CGameObject::SetState(state);
}

void Item::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + widthBoundingBox;
	b = y + heightBoundingBox;
}