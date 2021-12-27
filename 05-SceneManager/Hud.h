#pragma once
#include "Textures.h"
#include "Game.h"
#include "Sprite.h"

#define HUD_SIZE 40
#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define ID_ANI_ARROW	1915
#define ID_HUD_ANI_ARROW	1911
#define ID_HUD_ANI_P 1912
#define ID_HUD_BG	1910
#define BBOX_BLACK -101

class Hud
{
private:
	static Hud* _instance;
	float cam_y;
	int NumSpeed;
	LPTEXTURE bbox;
	LPSPRITE speed;
public:
	static Hud* GetInstance();
	Hud();
	void Update(DWORD dt);
	void Render();
};

