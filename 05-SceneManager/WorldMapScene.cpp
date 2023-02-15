#include "WorldMapScene.h"
#include "Sprites.h"

using namespace std;

#define ASSETS_SECTION_UNKNOWN -2
#define ASSETS_SECTION_SPRITES 1
#define MAX_SCENE_LINE 1024

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_MAP	3

#define WORLD_MAP_TEXTURE 41
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
	key_handler = new WorldMapKeyHandler(this);
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
	DebugOut(L"[ERROR] Texture ID %d not found!\n", 1);

}

void WorldMapScene::Update(DWORD dt)
{
}

void WorldMapScene::Render()
{
	LPTEXTURE tex = CTextures::GetInstance()->Get(WORLD_MAP_TEXTURE);

	CSprites::GetInstance()->Add(WORLD_MAP_SPRITE_ID, 0, 0, 234, 162, tex);

	int height = CGame::GetInstance()->GetBackBufferHeight();
	int width = CGame::GetInstance()->GetBackBufferWidth();
	//DebugOut(L"Width: %d Height: %d\n", width, height);

	CSprites::GetInstance()->Get(WORLD_MAP_SPRITE_ID)->Draw(150, 100);
}

void WorldMapScene::Unload()
{
}

