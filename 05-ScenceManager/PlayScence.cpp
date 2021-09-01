#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Game.h"

// Block - to 1 file
#include "Ground.h"
#include "ColorBlock.h"
#include "Block.h"
#include "QuestionBlock.h"
#include "Coin.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) : CScene(id, filePath) {
	player = NULL;
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP	7

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_GOOMBA	5
#define OBJECT_TYPE_KOOPAS	3

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_MAP(string line) {
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());
	wstring map_path = ToWSTR(tokens[5]);

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
	Map::GetInstance()->Load(map_path);
}

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

	objectWidth = abs(r - l);
	objectHeight = abs(t - b);

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new Animation();

	int ani_id = atoi(tokens[0].c_str());
	for (size_t i = 1; i < tokens.size(); i += 2)
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (size_t i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->Add(ani_id, ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	ObjectType object_type = (ObjectType)atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type) {
	case ObjectType::PLAYER: {
		if (player != NULL) {
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new Mario(x, y);
		player = (Mario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	}
	case ObjectType::BLOCK: {
		BlockType blockType = BlockType(atoi(tokens[6].c_str()));

		float width = (float)atof(tokens[4].c_str()) * TILE_SIZE;
		float height = (float)atof(tokens[5].c_str()) * TILE_SIZE;
		switch (blockType) {
		case BlockType::GROUND: {
			obj = new Ground(width, height);
			break;
		}
		case BlockType::COLOR_BLOCK: {
			obj = new ColorBlock(width, height);
			break;
		}
		case BlockType::BRICK: {
			obj = new Brick(width, height);
			break;
		}
		case BlockType::QUESTION_BLOCK: {
			ItemType type = ItemType(atoi(tokens[7].c_str()));
			obj = new QuestionBlock(type, width, height);
			questionBlocks.push_back(dynamic_cast<QuestionBlock*>(obj));
			break;
		}
		}
		break;
	}

	case ObjectType::ITEM: {
		ItemType itemType = ItemType(atoi(tokens[6].c_str()));
		switch (itemType) {
		case ItemType::COIN: {
			// The coins is in the middle of the bricks
			x += 3.5;
			obj = new Coin(itemType, objectWidth, objectHeight);
			items.push_back(dynamic_cast<Item*>(obj));
			break;
		}
		case ItemType::SUPER_ITEM: {
			obj = new SuperItem(itemType, objectWidth, objectHeight);
			items.push_back(dynamic_cast<SuperItem*>(obj));
			break;
		}
		}
		break;
	}
						 //case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
						 //case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
						 //case OBJECT_TYPE_PORTAL:
						 //{
						 //	float r = atof(tokens[4].c_str());
						 //	float b = atof(tokens[5].c_str());
						 //	int scene_id = atoi(tokens[6].c_str());
						 //	obj = new CPortal(x, y, r, b, scene_id);
						 //}
						 //break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

/// <summary>
/// Load all the resource from scene file
/// </summary>
void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[MAPS]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox_red.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	// Assign item to block, then release the vector objects.
	for (size_t i = 0; i < items.size(); i++) {
		questionBlocks[i]->setItem(items[i]);
	}
	questionBlocks = vector<QuestionBlock*>();
	items = vector<Item*>();
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		if (objects[i]->isAlive) {
			coObjects.push_back(objects[i]);
		}
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isAlive) {
			objects[i]->Update(dt, &coObjects);
		}
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	int mapWidth = Map::GetInstance()->GetWidth();
	if (player->y >= 240)
		cy = 240;

	CGame::GetInstance()->SetCamPos(cx, cy);
}

void CPlayScene::Render()
{
	// Map ve 1 lan thoi
	Map::GetInstance()->Render();

	for (size_t i = 0; i < objects.size(); i++) {
		if (objects[i]->isAlive) {
			objects[i]->Render();
		}
	}
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (size_t i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] Keydown: %d\n", KeyCode);

	Mario* mario = ((CPlayScene*)scence)->GetPlayer();
	CPlayScene* s = ((CPlayScene*)scence);
	switch (KeyCode)
	{
	case DIK_SPACE:
		mario->setObjectState(ObjectState::MARIO_STATE_JUMP);
		break;
	case DIK_A:
		s->Unload();
		s->Load();
		break;
	case DIK_I:
		mario->setObjectState(ObjectState::MARIO_TAIL_STATE_IDLE);
		break;
	}

}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	Mario* mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->getObjectState() == ObjectState::MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		mario->setObjectState(ObjectState::MARIO_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->setObjectState(ObjectState::MARIO_STATE_WALKING_LEFT);
	else
		mario->setObjectState(ObjectState::MARIO_STATE_IDLE);
}