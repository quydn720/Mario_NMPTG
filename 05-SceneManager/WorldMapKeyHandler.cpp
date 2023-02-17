#include "WorldMapKeyHandler.h"
#include <dinput.h>
#include "debug.h"
#include "Game.h"

#define EMPTY_NODE_ID 20
#define SCENE_1 1

void WorldMapKeyHandler::KeyState(BYTE* states)
{
}

void WorldMapKeyHandler::OnKeyDown(int KeyCode)
{
	WorldMapScene *scene = (WorldMapScene*)scence;

	if (player->isNotMoving()) {
		if (KeyCode == DIK_S) {
			int nextSceneId = scene->getCurrentPosition();
			if (nextSceneId != EMPTY_NODE_ID){
				CGame::GetInstance()->InitiateSwitchScene(SCENE_1); // every node go to 1-1
				//CGame::GetInstance()->InitiateSwitchScene(nextSceneId);
			}
		}

		switch (KeyCode) {
			case DIK_DOWN:
				scene->GoDown();
				break;
			case DIK_UP:
				scene->GoUp();
				break;
			case DIK_LEFT: {
				scene->GoLeft();
				break;
			}
			case DIK_RIGHT: {
				scene->GoRight();
				break;
			}
			default:
				break;
			}
	}
}

void WorldMapKeyHandler::OnKeyUp(int KeyCode)
{
}
