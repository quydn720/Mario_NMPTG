#include "StartScene.h"

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

	//auto tex = CTextures::GetInstance()->Get(TITLE_SCREEN_TEXTURE);
	//CSprites::GetInstance()->Add(TITLE_SCREEN_SPRITE_ID, 0, 0, 256, 226, tex);

	//auto tex2 = CTextures::GetInstance()->Get(44);
	// Background
	//CSprites::GetInstance()->Add(-28039, 0, 0, 256, 224, tex2);
	//// Mario Bros 3
	//CSprites::GetInstance()->Add(-28030, 295, 31, 477, 71, tex2);
	//// Super
	//CSprites::GetInstance()->Add(-28035, 343, 0, 433, 34, tex2);
	//// 1 dòng của cái màn
	//CSprites::GetInstance()->Add(-28036, 257, 110, 513, 124, tex2);

	//// Chon Players
	//CSprites::GetInstance()->Add(-28037, 257, 265, 369, 305, tex2);
	//CSprites::GetInstance()->Add(-28038, 382, 265, 494, 305, tex2);

	//// Số 3 sprites
	//CSprites::GetInstance()->Add(-28031, 257, 125, 299, 167, tex2);
	//CSprites::GetInstance()->Add(-28032, 303, 125, 345, 167, tex2);
	//CSprites::GetInstance()->Add(-28033, 350, 125, 392, 167, tex2);
	//CSprites::GetInstance()->Add(-28034, 396, 125, 438, 167, tex2);

	//LPANIMATION ani = new CAnimation();

	//ani->Add(-28031, 100);
	//ani->Add(-28032, 100);
	//ani->Add(-28033, 100);
	//ani->Add(-28034, 100);

	//// Số 3 Animations
	//CAnimations::GetInstance()->Add(-28036, ani);


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
