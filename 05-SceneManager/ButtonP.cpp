#include "ButtonP.h"
#include "AssetIDs.h"
#include "debug.h"
#include "BreakableBrick.h"
#include "Coin.h"

ButtonP* ButtonP::__instance = NULL;
ButtonP::ButtonP()
{
	ChangeToCoin = isPressed = isInit = false;
	objType = OBJTYPE_BUTTON_P;
	SetState(BUTTON_P_STATE_INIT);
}

ButtonP::ButtonP(float x, float y)
{
	ChangeToCoin = isPressed = isInit = false;
	objType = OBJTYPE_BUTTON_P;
	this->x = x; this->y = y;
	SetState(BUTTON_P_STATE_INIT);
}

void ButtonP::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	if (isPressed == false)
	{
		bottom = y + 16;
	}
	else
	{
		bottom = y + 6;
	}
}

void ButtonP::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isPressed == true && ChangeToCoin == false)
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (dynamic_cast<BreakableBrick*>(coObjects->at(i)))
			{
				BreakableBrick* brick = (BreakableBrick*)coObjects->at(i);
				if(brick->Item == TYPE_NORMAL)
				{
					brick->isDeleted = true;
					CCoin* coin = new CCoin(brick->x, brick->y);
					coin->SetState(COIN_STATE_WAIT);
					coin->isBrickToCoin = true;
					coin->AppearTime = GetTickCount64();
					_PlayScene->AddNewObject(coin);
				}
			}
		}
		ChangeToCoin = true;
	}
}

void ButtonP::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = ID_ANI_BUTTON_P;
	if (isPressed == false)
		aniId = ID_ANI_BUTTON_P;
	else
		aniId = ID_ANI_BUTTON_P_PUSHED;
	animations->Get(aniId)->Render(x, y);
}

void ButtonP::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BUTTON_P_STATE_INIT:
	{
		isPressed = false;
	}
	break;

	case BUTTON_P_STATE_PUSHED:
	{
		if (isPressed == false)
		{
			isPressed = true;
			y = y + (16 - 6);
		}
	}
	break;
	}
}
