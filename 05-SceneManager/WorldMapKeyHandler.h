#pragma once
#include "Scene.h"

class WorldMapKeyHandler : public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	WorldMapKeyHandler(LPSCENE s) : CSceneKeyHandler(s) {};
};

