#pragma once
#include "Textures.h"
#include "Game.h"
#include "Sprite.h"

#define HUD_SIZE 40
#define ID_HUD_SPRITE_ARROW	1911
#define ID_HUD_SPRITE_P 1912
#define ID_HUD_BG	1910
#define BBOX_BLACK -101
#define ID_ANI_FLASHING_P 1918

class Hud
{
private:
	static Hud* _instance;
	float cam_y;
	int NumSpeed;
public:
	static Hud* GetInstance();
	Hud();
	void Update(DWORD dt);
	void Render();
};

