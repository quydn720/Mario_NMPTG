#pragma once
#include "Scene.h"

class StartScene : public CScene
{
public:
	StartScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt){}
	virtual void Render();
	virtual void Unload(){}
};


class StartScreenKeyHandler : public CSceneKeyHandler {
public:
	virtual void KeyState(BYTE* state){}
	virtual void OnKeyUp(int KeyCode){}

	virtual void OnKeyDown(int KeyCode);
	StartScreenKeyHandler(LPSCENE s);
};