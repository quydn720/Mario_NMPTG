#pragma once
#include <fstream>
#include "WorldMapPlayer.h"
#include "Scene.h"

class WorldMapScene : public CScene {
private:
	WorldMapPlayer* player;
	bool isGoingRight, isGoingLeft, isGoingUp, isGoingDown = false;
	int movedUnit = 0;
	int wmapRow = 0, wmapColumn = 0;
	int curRow = 0;

	void LoadAssets(LPCWSTR assetPath);
	void _ParseSection_SPRITES(std::string line);
	void _ParseSection_ASSETS(std::string line);
	void _ParseSection_ANIMATIONS(std::string line);
	void _ParseSection_MAP(std::string line);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

public:
	WorldMapScene(int id, LPCWSTR filePath);
	
	void GoRight();
	void GoLeft();
	void GoUp();
	void GoDown();
	int getCurrentPosition();

	int currentX = 2;
	int currentY = 1;

	int maps[9][14];
};