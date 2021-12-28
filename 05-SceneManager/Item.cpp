#include "Item.h"

Item::Item() : CGameObject()
{
	TypeItem = -1;
	Start_X = Start_Y = 0;
	Score = 0;
	OnGround = false;
}
