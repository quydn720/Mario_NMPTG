#pragma once
#include <fstream>

#include "Scene.h"
#include "debug.h"
#include "Utils.h"
#include "WorldMapKeyHandler.h"
#include "Textures.h"

class WorldMapScene : public CScene {
	void LoadAssets(LPCWSTR assetPath);
	void _ParseSection_SPRITES(string line);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
public:
	WorldMapScene();
};