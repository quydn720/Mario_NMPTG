#include "WorldMapKeyHandler.h"
#include <dinput.h>
#include "debug.h"
#include "Game.h"

void WorldMapKeyHandler::KeyState(BYTE* states)
{
}

void WorldMapKeyHandler::OnKeyDown(int KeyCode)
{
	WorldMapScene *scene = (WorldMapScene*)scence;

	if (player->isNotMoving()) {
		if (KeyCode == DIK_S) {
			CGame::GetInstance()->InitiateSwitchScene(1);
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
