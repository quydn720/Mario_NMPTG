#include "Hud.h"

Hud* Hud::_instance = nullptr;

Hud* Hud::GetInstance()
{
	if (_instance == nullptr)
		_instance = new Hud();
	return _instance;
}

void Hud::Render()
{
	LPTEXTURE bbox = CTextures::GetInstance()->Get(35);
	CGame* game = CGame::GetInstance();
	RECT* r = NULL;
	game->Draw(float(game->GetBackBufferWidth()) / 2, game->GetBackBufferHeight() - 20, bbox, r, 1.0f, game->GetBackBufferWidth(), 40);
}