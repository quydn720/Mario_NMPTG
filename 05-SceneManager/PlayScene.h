#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Map.h"
#include "QuestionBlock.h"
#include "Item.h"
#include "Koopas.h"
#include "Plant.h"
#include "Camera.h"


class CPlayScene: public CScene
{
public: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;		
	CMap* map;
	CCamera* camera;

	vector<CQuestionBlock*> questionBlocks;
	vector<Item*> items;

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);

	void LoadAssets(LPCWSTR assetFile);
	void LoadMap(LPCWSTR assetFile);
	
public: 
	static CPlayScene* __instance;

	static CPlayScene* GetInstance()
	{
		if (__instance == NULL) __instance = new CPlayScene();
		return __instance;
	}

	CPlayScene();

	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);

	void AddNewObject(LPGAMEOBJECT obj);
};

typedef CPlayScene* LPPLAYSCENE;

