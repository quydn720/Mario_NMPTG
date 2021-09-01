#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Map.h"
#include "QuestionBlock.h"

#define TILE_SIZE 16

class CPlayScene: public CScene
{
protected: 
	Mario* player;					// A play scene has to have player, right? 
	//Map* map;
	// Object width, height calculated when we parse SPRITES part
	// Áp dụng với những vật thể 1 sprite. 
	// Mario, coin, nấm, v.v
	// TODO: nên stick với một state cụ thể ?
	int objectWidth = 0; 
	int objectHeight = 0;

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	Mario * GetPlayer() { return player; } 

	// Purpose: Assign item to block object; because we initialize item with the map
	vector<QuestionBlock*> questionBlocks;
	vector<Item*> items;

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

