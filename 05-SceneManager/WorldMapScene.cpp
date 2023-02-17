#include "WorldMapScene.h"
#include "Sprites.h"
#include "Textures.h"
#include "debug.h"
#include "Utils.h"
#include "Animations.h"

using namespace std;

#define ASSETS_SECTION_UNKNOWN -2
#define ASSETS_SECTION_SPRITES 1
#define MAX_SCENE_LINE 1024

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_MAP	3

#define WORLD_MAP_TEXTURE 41
#define WORLD_MAP_MARIO_TEXTURE 42
#define WORLD_MAP_SPRITE_ID -28127

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
		//if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section) {
			case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		}
	}

	f.close();

	//DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

WorldMapScene::WorldMapScene()
{
	player = new WorldMapPlayer();
	key_handler = new WorldMapKeyHandler(this, player);
}

void WorldMapScene::_ParseSection_SPRITES(string line){
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

void WorldMapScene::Load()
{
	LPTEXTURE tex = CTextures::GetInstance()->Get(WORLD_MAP_TEXTURE);

	CSprites::GetInstance()->Add(WORLD_MAP_SPRITE_ID, 0, 0, 234, 162, tex);

	tex = CTextures::GetInstance()->Get(WORLD_MAP_MARIO_TEXTURE);
	// small mario running
	CSprites::GetInstance()->Add(WORLD_MAP_SPRITE_ID + 1, 4, 116, 18, 132, tex);
	CSprites::GetInstance()->Add(WORLD_MAP_SPRITE_ID + 2, 24, 116, 38, 132, tex);
	
	LPANIMATION ani = new CAnimation();
	ani->Add(WORLD_MAP_SPRITE_ID + 1, 100);
	ani->Add(WORLD_MAP_SPRITE_ID + 2, 100);

	
	CAnimations::GetInstance()->Add(WORLD_MAP_SPRITE_ID + 200, ani);

}

void WorldMapScene::Update(DWORD dt)
{
	player->Update(dt, currentY, currentX, movedUnit);
}

void WorldMapScene::GoUp() {
	
	int next = maps[currentX - 1][currentY];
	int moved = 0;
	DebugOut(L"From Position: %d, %d\t", currentX, currentY);

	while (next > 0) {

		if (currentX - 1 < 0) // out of bound
			break;

		currentX -= 1;
		movedUnit += 1;
		moved += 1;


		if (next == 0) break;
		if (next == 2) break;

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

	while (next > 0) {

		if (currentX + 1 > 9) // out of bound
			break;

		currentX += 1;
		movedUnit += 1;
		moved += 1;

		if (next == 0) break;
		if (next == 2) break;

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

	while (next > 0) {

		if (currentY - 1 < 0) // out of bound
			break;

		currentY -= 1;
		movedUnit += 1;
		moved += 1;

		if (next == 0) break;
		if (next == 2) break;

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

	while (next > 0) {

		if (currentY + 1 > 14) // out of bound
			break;

		currentY += 1;
		movedUnit += 1;
		moved += 1;

		if (next == 0) break;
		if (next == 2) break;

		next = maps[currentX][currentY + 1];
	}

	if (moved > 0) {
		player->GoRight();
		player->startX = player->GetX();
		DebugOut(L"[BaseX]: %f,\n", player->startX);
	}
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

