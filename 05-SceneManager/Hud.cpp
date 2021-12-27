#include "Hud.h"
#include "Mario.h"
#include "Camera.h"

#define MAX_STACK_SPEED 6
#define BASE_ARROW_POSITION_X 80
#define BASE_ARROW_POSITION_Y 178

Hud* Hud::_instance = nullptr;

Hud* Hud::GetInstance()
{
	if (_instance == nullptr)
		_instance = new Hud();
	return _instance;
}

Hud::Hud()
{
	NumSpeed = 0;
	bbox = CTextures::GetInstance()->Get(BBOX_BLACK);
	auto sprites = CSprites::GetInstance();
	speed = sprites->Get(ID_ANI_ARROW);
}

void Hud::Update(DWORD dt)
{
	float vx, vy;
	CMario::GetInstance()->GetSpeed(vx, vy);

	if (0.1f < abs(vx) && abs(vx) <= 0.115f)
	{
		NumSpeed = 1;
	}
	else
	{
		if (0.115f < abs(vx) && abs(vx) <= 0.13f)
		{
			NumSpeed = 2;
		}
		else
		{
			if (0.13f < abs(vx) && abs(vx) <= 0.15f)
			{
				NumSpeed = 3;
			}
			else
			{
				if (0.15f < abs(vx) && abs(vx) <= 0.17f)
				{
					NumSpeed = 4;
				}
				else
				{
					if (0.17f < abs(vx) && abs(vx) <= 0.19f)
					{
						NumSpeed = 5;
					}
					else
					{
						if (abs(vx) >= MARIO_RUNNING_SPEED)
						{
							NumSpeed = 6;
						}
					}
				}
			}
		}
	}
	DebugOut(L"%0.2d\n", NumSpeed);
}

void Hud::Render()
{
	CGame* game = CGame::GetInstance();
	RECT* r = NULL;
	CSprites* sprites = CSprites::GetInstance();
	
	float a = game->GetBackBufferWidth() / 2;
	float b = game->GetBackBufferHeight() - 20;

	game->Draw(a, b, bbox, r, 1.0f, game->GetBackBufferWidth(), 40);
	
	sprites->Get(ID_HUD_BG)->DrawFixPos(152, 183);
	
	if (NumSpeed > 0)
	{
		for (int i = 1; i <= NumSpeed; i++)
		{
			if (i == MAX_STACK_SPEED)
			{
				sprites->Get(ID_HUD_ANI_P)->DrawFixPos(float(BASE_ARROW_POSITION_X + ((6) * 8) + 11), BASE_ARROW_POSITION_Y); // fix pos
				sprites->Get(ID_HUD_ANI_ARROW)->DrawFixPos(float(BASE_ARROW_POSITION_X + (i * 8)), BASE_ARROW_POSITION_Y);
			}
			else
				sprites->Get(ID_HUD_ANI_ARROW)->DrawFixPos(float(BASE_ARROW_POSITION_X + (i * 8)), BASE_ARROW_POSITION_Y);
		}
	}
}