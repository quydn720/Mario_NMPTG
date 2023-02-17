#pragma once
#include <fstream>
#include "WorldMapPlayer.h"
#include "WorldMapKeyHandler.h"

class WorldMapScene : public CScene {
private:
	WorldMapPlayer* player;
	bool isGoingRight, isGoingLeft, isGoingUp, isGoingDown = false;
	int movedUnit = 0;


	void LoadAssets(LPCWSTR assetPath);
	void _ParseSection_SPRITES(std::string line);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

public:
	WorldMapScene();
	
	void GoRight();
	void GoLeft();
	void GoUp();
	void GoDown();
	int getCurrentPosition();

	int currentX = 2;
	int currentY = 1;

	int maps[9][14] = {
		{0, 0, 0, 2, 1, 20, 1, 2, 1, 2, 1, 20, 0, 0},
		{0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0},
		{0, 2, 1, 20, 0, 0, 0, 20, 1, 2, 1, 2, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 20, 1, 2, 1, 2, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 20, 0, 1, 1, 20, 1, 20, 1, 2, 0, 0},
		{0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 2, 1, 20, 1, 2, 0, 0, 0, 0, 0, 0},
	};
};