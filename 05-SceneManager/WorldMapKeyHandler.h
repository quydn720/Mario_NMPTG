#pragma once
#include "Scene.h"
#include "WorldMapPlayer.h"

class WorldMapKeyHandler : public CSceneKeyHandler
{

private: 
	WorldMapPlayer* player;
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	WorldMapKeyHandler(LPSCENE s, WorldMapPlayer* p) : CSceneKeyHandler(s) {
		player = p;
	};
};

