#include "StartScene.h"
#include <dinput.h>
#include "Scene.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"

#define TITLE_SCREEN_TEXTURE 43
#define TITLE_SCREEN_SPRITE_ID -28027

#define MAX_SCENE_LINE 1024
#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_MAP	3

StartScene::StartScene(int id, LPCWSTR filePath) : CScene(id, filePath)
{	
	key_handler = new StartScreenKeyHandler(this);
}

void StartScene::Load()
{

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	auto tex = CTextures::GetInstance()->Get(TITLE_SCREEN_TEXTURE);
	CSprites::GetInstance()->Add(TITLE_SCREEN_SPRITE_ID, 0, 0, 256, 226, tex);
}

void StartScene::Render()
{
	float height = (float)CGame::GetInstance()->GetBackBufferHeight();
	float width = (float)CGame::GetInstance()->GetBackBufferWidth();

	CSprites::GetInstance()->Get(-28039)->Draw(width / 2, height / 2);
	CSprites::GetInstance()->Get(-28036)->Draw(width / 2, 0 + 8);
	CSprites::GetInstance()->Get(-28036)->Draw(width / 2, 16);
	CSprites::GetInstance()->Get(-28036)->Draw(width / 2, 24);
	CSprites::GetInstance()->Get(-28035)->Draw(width / 2, height / 2 - 33);
	CSprites::GetInstance()->Get(-28030)->Draw(width / 2, height / 2);
	CSprites::GetInstance()->Get(-28032)->Draw(width / 2, height / 2 + 40);

	//CSprites::GetInstance()->Get(-28037)->Draw(width / 2, height / 2 + 60);
	//CSprites::GetInstance()->Get(option ? -28037 : -28038)->Draw(width / 2, height / 2 + 60);
	CSprites::GetInstance()->Get(option ? -28037 : -28038)->Draw(56, 20);

	//CAnimations* animations = CAnimations::GetInstance();
	//animations->Get(-28036)->Render(width / 2, height / 2);
}

void StartScene::_ParseSection_ASSETS(string line) {
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
void StartScene::_ParseSection_OBJECTS(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < 1) return;
	wstring path = ToWSTR(tokens[0]);
}

void StartScreenKeyHandler::OnKeyDown(int KeyCode)
{
	StartScene* scene = (StartScene*)scence;
	if (KeyCode == DIK_Q) {
		scene->updateOption();
	}
	else if (KeyCode == DIK_W) {
		CGame::GetInstance()->InitiateSwitchScene(WORLD_MAP_SCENE_ID);
	}
}

StartScreenKeyHandler::StartScreenKeyHandler(LPSCENE s) 
	: CSceneKeyHandler(s)
{

}