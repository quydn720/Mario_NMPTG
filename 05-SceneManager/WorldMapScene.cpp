#include "WorldMapScene.h"
#include "Sprites.h"
#include "Textures.h"
#include "debug.h"
#include "Utils.h"
#include "Animations.h"
#include "WorldMapKeyHandler.h"

using namespace std;

#define ASSETS_SECTION_UNKNOWN -2
#define ASSETS_SECTION_SPRITES 10
#define ASSETS_SECTION_ANIMATIONS 20
#define MAX_SCENE_LINE 1024

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_MAP	3


#define WORLD_MAP_TEXTURE 41
#define WORLD_MAP_MARIO_TEXTURE 42

#define WORLD_MAP_SPRITE_ID -28127


#define WMAP_INFO_SECTION 1
#define MAP_TILE_SECTION 2
#define MAP_UNKNOWN_SECTION -1

void WorldMapScene::LoadAssets(LPCWSTR assetPath)
{
	//DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetPath);

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

	//DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

WorldMapScene::WorldMapScene(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	player = new WorldMapPlayer();
	key_handler = new WorldMapKeyHandler(this, player);
}

void WorldMapScene::_ParseSection_SPRITES(string line) {
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


void WorldMapScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

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

void WorldMapScene::Load()
{
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
			/*case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;*/
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}
	// enter the bg color in the file
	CGame::GetInstance()->setBackgroundColor(D3DXCOLOR(0, 0, 0, 100));

	f.close();
}


void WorldMapScene::_ParseSection_MAP(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < 1) return;
	wstring path = ToWSTR(tokens[0]);

	ifstream f;
	f.open(path);

	// current resource section flag
	int section = MAP_UNKNOWN_SECTION;
	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE)) {
		string line(str);
		if (line[0] == '#') continue;
		if (line == "[Info]") {
			section = WMAP_INFO_SECTION;
			continue;
		}
		if (line == "[Map]") {
			section = MAP_TILE_SECTION;
			continue;
		}
		if (line[0] == '[') {
			section = MAP_UNKNOWN_SECTION;
			continue;
		}

		switch (section) {
		case WMAP_INFO_SECTION: {
			vector<string> tokens = split(line);
			size_t size = tokens.size();

			if (size < 2) return;
			wmapRow = atoi(tokens[0].c_str());
			wmapColumn = atoi(tokens[1].c_str());
			break;
		}
		case MAP_TILE_SECTION: {
			vector<string> tokens = split(line);

			size_t size = tokens.size();
			if (size < wmapColumn) return;
			for (int i = 0; i < wmapColumn; i++) {
				int info = stoi(tokens[i]);
				maps[curRow][i] = info;
			}
			curRow++;
			break;
		}
		}
	}
	f.close();
	camera = new CCamera(162, 144, 304, 200);
}

void WorldMapScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 1) return;
	wstring path = ToWSTR(tokens[0]);
	LoadAssets(path.c_str());
}

void WorldMapScene::Update(DWORD dt)
{
	float a, b = 0;
	camera->SetPosition(a, b);
	player->Update(dt, currentY, currentX, movedUnit);
}

void WorldMapScene::GoUp() {

	int next = maps[currentX - 1][currentY];
	int moved = 0;
	DebugOut(L"From Position: %d, %d\t", currentX, currentY);

	while (next != 0) {

		if (currentX - 1 < 0) // out of bound
			break;

		currentX -= 1;
		movedUnit += 1;
		moved += 1;


		if (next == 0) break;
		if (next != 1) break;

		next = maps[currentX - 1][currentY];
	}

	DebugOut(L"To Position: %d, %d\t", currentX, currentY);
	if (moved > 0)
	{
		player->startY = player->GetY();
		player->GoUp();
		DebugOut(L"[BaseY]: %f,\n", player->startY);
	}
}
void WorldMapScene::GoDown() {

	int next = maps[currentX + 1][currentY];
	int moved = 0;
	DebugOut(L"[DOWN] From Position: %d, %d\t", currentX, currentY);

	while (next != 0) {

		if (currentX + 1 > wmapRow) // out of bound
			break;

		currentX += 1;
		movedUnit += 1;
		moved += 1;

		if (next == 0) break;
		if (next != 1) break;

		next = maps[currentX + 1][currentY];
	}
	DebugOut(L"[DOWN] To Position: %d, %d\t", currentX, currentY);

	if (moved > 0)
	{
		player->startY = player->GetY();
		player->GoDown();
		DebugOut(L"[BaseY]: %f,\n", player->startY);
	}
}
void WorldMapScene::GoLeft() {

	int next = maps[currentX][currentY - 1];
	int moved = 0;
	DebugOut(L"[DOWN] From Position: %d, %d\t", currentX, currentY);

	while (next != 0) {

		if (currentY - 1 < 0) // out of bound
			break;

		currentY -= 1;
		movedUnit += 1;
		moved += 1;

		if (next == 0) break;
		if (next != 1) break;

		next = maps[currentX][currentY - 1];
	}
	DebugOut(L"[DOWN] To Position: %d, %d\t", currentX, currentY);

	if (moved > 0)
	{
		player->GoLeft();
		player->startX = player->GetX();
		DebugOut(L"[BaseX]: %f,\n", player->startX);
	}
}
void WorldMapScene::GoRight() {

	int next = maps[currentX][currentY + 1];
	int moved = 0;

	while (next != 0) {

		if (currentY + 1 > wmapColumn) // out of bound
			break;

		currentY += 1;
		movedUnit += 1;
		moved += 1;

		if (next == 0) break;
		if (next != 1) break;

		next = maps[currentX][currentY + 1];
	}

	if (moved > 0) {
		player->GoRight();
		player->startX = player->GetX();
		DebugOut(L"[BaseX]: %f,\n", player->startX);
	}
}

int WorldMapScene::getCurrentPosition() {
	return maps[currentX][currentY];
}

void WorldMapScene::Render()
{
	int height = CGame::GetInstance()->GetBackBufferHeight();
	int width = CGame::GetInstance()->GetBackBufferWidth();

	CSprites::GetInstance()->Get(WORLD_MAP_SPRITE_ID)->Draw(width / 2, height / 2);
	player->Render();
}

void WorldMapScene::Unload()
{
}

