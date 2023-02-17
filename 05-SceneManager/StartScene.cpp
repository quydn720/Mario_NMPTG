#include "StartScene.h"
#include <dinput.h>
#include "Scene.h"
#include "Game.h"
#include "Sprites.h"
#include "Textures.h"

#define TITLE_SCREEN_TEXTURE 43
#define TITLE_SCREEN_SPRITE_ID -28027


StartScene::StartScene(int id, LPCWSTR filePath) 
{	
	key_handler = new StartScreenKeyHandler(this);
}

void StartScene::Load()
{
	auto tex = CTextures::GetInstance()->Get(TITLE_SCREEN_TEXTURE);
	CSprites::GetInstance()->Add(TITLE_SCREEN_SPRITE_ID, 0, 0, 256, 226, tex);
}

void StartScene::Render()
{
	int height = CGame::GetInstance()->GetBackBufferHeight();
	int width = CGame::GetInstance()->GetBackBufferWidth();

	CSprites::GetInstance()->Get(TITLE_SCREEN_SPRITE_ID)
		->Draw(width / 2, height / 2);
}

void StartScreenKeyHandler::OnKeyDown(int KeyCode)
{
	StartScene *scene = (StartScene*)scence;

	if (KeyCode == DIK_S) {
		CGame::GetInstance()->InitiateSwitchScene(WORLD_MAP_SCENE_ID);
	}
}

StartScreenKeyHandler::StartScreenKeyHandler(LPSCENE s) 
	: CSceneKeyHandler(s)
{

}