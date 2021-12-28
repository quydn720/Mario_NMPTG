#include "BreakableBrick.h"
#include "ButtonP.h"

void BreakableBrick::Render()
{
	int aniId = ID_ANI_BREAKABLE_BRICK;
	CAnimations* animations = CAnimations::GetInstance();

	if (hasItem == false)
		if (isCollision == true)
			aniId = ID_ANI_BREAKABLE_BRICK_IS_UP;
	animations->Get(aniId)->Render(x, y);
}

void BreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BRICK_STATE_DIE)
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
	else
	{
		if (isCollision == true && state != BRICK_STATE_EMPTY)
		{

			if (startY - y >= MAX_HEIGHT)
			{
				vy = BREAKBLE_BRICK_VY;
			}
			y += vy * dt;

			if (startY < y)
			{
				y = startY;
				SetState(BRICK_STATE_EMPTY);
			}
		}
	}
}

void BreakableBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BRICK_STATE_NORMAL:
	{
		vy = 0;
		isCollision = false;
	}
	break;
	case BRICK_STATE_COLLISION:
	{
		// viên gạch nảy lên
		vy = -BREAKBLE_BRICK_VY;
		isCollision = true;
	}
	break;
	case BRICK_STATE_EMPTY:
	{
		vy = 0;
		hasItem = false;
	}
	break;

	case BRICK_STATE_DIE:
	{
		
	}
	break;

	}
}

