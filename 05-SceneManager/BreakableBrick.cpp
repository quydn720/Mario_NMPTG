#include "BreakableBrick.h"
#include "ButtonP.h"

void BreakableBrick::Render()
{
	int aniId = -1;
	CAnimations* animations = CAnimations::GetInstance();
	
	if (objType == OBJECT_TYPE_COIN)
	{
		aniId = ID_ANI_COIN;
		animations->Get(aniId)->Render(x, y);
	}
	else {
		aniId = ID_ANI_BREAKABLE_BRICK;
		if (buttonCreated)
			aniId = ID_ANI_BREAKABLE_BRICK_IS_UP;
		animations->Get(aniId)->Render(x, y);
	}

}

void BreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects ) 
{
	y += vy * dt;
	if (startY - y >= 5 && vy < 0)
	{
		vy = -vy;
	}
	if (vy > 0 && y >= startY)
	{
		y = startY;
		vy = 0;
	}
	
	if (state == BREAKABLE_BRICK_STATE_BREAK_DOWN)
	{
		BreakableBrickEffect* piece1 = new BreakableBrickEffect(x, y, -0.08f, -0.25f);
		_PlayScene->objects.push_back(piece1);
		BreakableBrickEffect* piece2 = new BreakableBrickEffect(x, y, 0.08f, -0.25f);
		_PlayScene->objects.push_back(piece2);
		BreakableBrickEffect* piece3 = new BreakableBrickEffect(x, y, -0.08f, -INNIT_VY_BREAKABLE_BRICK_EFFECT);
		_PlayScene->objects.push_back(piece3);
		BreakableBrickEffect* piece4 = new BreakableBrickEffect(x, y, 0.08f, -INNIT_VY_BREAKABLE_BRICK_EFFECT);
		_PlayScene->objects.push_back(piece4);

		isDeleted = true;
	}
}

