#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "ColorBlock.h"
#include "WarpPipe.h"
#include "BreakableBrick.h"

#include "SampleKeyEventHandler.h"
#include "Hud.h"

using namespace std;
#define _game CGame::GetInstance() 

CPlayScene* CPlayScene::__instance = NULL;

CPlayScene::CPlayScene()
{
	map = NULL;
	player = NULL;
	camera = NULL;
	key_handler = new CSampleKeyHandler(this);
}

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	map = NULL;
	player = NULL;
	camera = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_MAP	3

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (size_t i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_MAP(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < 1) return;
	wstring path = ToWSTR(tokens[0]);

	LoadMap(path.c_str());
}

void CPlayScene::LoadMap(LPCWSTR mapFile) {
	DebugOut(L"[INFO] Start loading map from : %s \n", mapFile);
	map = new CMap(mapFile);
	DebugOut(L"[INFO] Done loading map from %s\n", mapFile);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject* obj = NULL;
	FallDetector* fallDetector = NULL;
	CTail* tail = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO: {
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;
		CMario::SetInstance((CMario*)obj);

		tail = new CTail(x, y);
		objects.push_back(tail);
		CMario* tempMario = dynamic_cast<CMario*>(obj);
		tempMario->tail = tail;
		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	}
	case OBJECT_TYPE_GOOMBA: {
		int level = (int)atoi(tokens[3].c_str());
		obj = new CGoomba(x, y, level);
		break;
	}
	case OBJECT_TYPE_KOOPAS: {
		// We adding a [FallDetector] when we instantiate a Red Koopas - to the objects vector
		obj = new CKoopas(x, y);
		CKoopas* tempKoopas = dynamic_cast<CKoopas*>(obj);

		fallDetector = new FallDetector(x, y);
		fallDetector->height = KOOPAS_BBOX_HEIGHT;
		objects.push_back(fallDetector);
		tempKoopas->fallDetector = fallDetector;
		break;
	}
	case OBJECT_TYPE_PLANT: {
		obj = new CPlant(x, y);
		break;
	}
	case OBJECT_TYPE_BRICK: {
		obj = new CBrick(x, y);
		break;
	}
	case OBJECT_TYPE_PIPE: {
		float height = (float)atoi(tokens[3].c_str());
		int hidden = atoi(tokens[4].c_str());
		int green = atoi(tokens[5].c_str());
		obj = new CWarpPipe(x, y, height, hidden, green);
		break;
	}
	case OBJECT_TYPE_QUESTION_BLOCK: {
		obj = new CQuestionBlock(x, y);
		questionBlocks.push_back(dynamic_cast<CQuestionBlock*>(obj));
		break; 
	}
	case OBJECT_TYPE_ITEM: {
		int type = (int)atoi(tokens[3].c_str());
		switch (type)
		{
		case ItemType::SuperItem:
			obj = new CSuperItem(x, y);
			break;
		case ItemType::Coin: {
			int isInsideBrick = (int)atoi(tokens[4].c_str());
			obj = new CCoin(x, y, isInsideBrick);
			break;
		}
		default:
			break;
		}
		items.push_back(dynamic_cast<Item*>(obj));
		break;
	}
	case OBJECT_TYPE_COLOR_BLOCK: {
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		obj = new CColorBlock(x, y, cell_width * length, cell_height,
			cell_width, cell_height, length);
		break;
	}

	case OBJECT_TYPE_PLATFORM:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);
	}
	break;

	case OBJECT_TYPE_BREAKABLE_BRICK: 
	{
		bool HaveButton = false;
		int Item = atoi(tokens[3].c_str());
		obj = new BreakableBrick(x, y, Item);
		
	}
	break;
	
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;

	
	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section) {
			case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		};
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}

	f.close();

	// Assign item to block, then release the vector objects.
	for (size_t i = 0; i < questionBlocks.size(); i++) {
		questionBlocks[i]->setItem(items[i]);
	}
	questionBlocks = vector<CQuestionBlock*>();
	items = vector<Item*>();
	camera = new CCamera(map->getMapWidth(), map->getMapHeight(), (float) _game->GetBackBufferWidth(), (float)_game->GetBackBufferHeight());

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	camera->SetPosition(cx, cy);
	CGame* game = CGame::GetInstance();
	_game->SetCamPos(cx, cy);
	Hud::GetInstance()->Update(dt);

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	map->Render();
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();

	// Hud::GetInstance()->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}