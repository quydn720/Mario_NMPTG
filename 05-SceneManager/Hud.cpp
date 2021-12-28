#include "Hud.h"
#include "Mario.h"
#include "Camera.h"

#define MAX_STACK_SPEED 6
#define BASE_ARROW_POSITION_X 80
#define BASE_ARROW_POSITION_Y 178
#define ARROW_SIZE 8
#define ARROW_P_DISTANCE 12.5

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
}

void Hud::Update(DWORD dt)
{
	float vx, vy;
	CMario::GetInstance()->GetSpeed(vx, vy);
	NumSpeed = 0;
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
	//DebugOut(L"%0.2d\n", NumSpeed);
}

void Hud::Render()
{
	CGame* game = CGame::GetInstance();
	RECT* r = NULL;
	CSprites* sprites = CSprites::GetInstance();
	
	float a =(float) game->GetBackBufferWidth() / 2;
	float b =(float) game->GetBackBufferHeight() - 20;
	// black bg
	game->Draw(a, b, CTextures::GetInstance()->Get(BBOX_BLACK), r, 1.0f, game->GetBackBufferWidth(), HUD_SIZE);
	// hud bg
	sprites->Get(ID_HUD_BG)->DrawFixPos(152, 183);
	// arrow / p
	if (NumSpeed > 0)
	{
		for (int i = 1; i <= NumSpeed; i++)
		{
			if (i == MAX_STACK_SPEED)
			{
				sprites->Get(ID_HUD_SPRITE_ARROW)->DrawFixPos(float(BASE_ARROW_POSITION_X + (i * ARROW_SIZE)), BASE_ARROW_POSITION_Y);
				CAnimations::GetInstance()->Get(ID_ANI_FLASHING_P)->RenderFixPos(float(BASE_ARROW_POSITION_X + ((i) * ARROW_SIZE) + ARROW_P_DISTANCE), BASE_ARROW_POSITION_Y);
			}
			else
				sprites->Get(ID_HUD_SPRITE_ARROW)->DrawFixPos(float(BASE_ARROW_POSITION_X + (i * ARROW_SIZE)), BASE_ARROW_POSITION_Y);
		}
	}
}