#pragma once
#include "Scene.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"
#include "Animations.h"
#include "debug.h"
#include "Utils.h"

class StartScene : public CScene
{
protected:
	bool option = true;

public:
	StartScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt) {}
	virtual void Render();
	virtual void Unload() {}

	void updateOption() {
		option = !option;
	}
	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);
};


class StartScreenKeyHandler : public CSceneKeyHandler {
public:
	virtual void KeyState(BYTE* state) {}
	virtual void OnKeyUp(int KeyCode) {}

	virtual void OnKeyDown(int KeyCode);
	StartScreenKeyHandler(LPSCENE s);
};