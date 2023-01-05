#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S: {
		if (mario->GetLevel() == MARIO_LEVEL_TAIL && mario->isOnPlatform == false) {
			mario->SetState(MARIO_FLY_DOWN);
		}
		else {
			mario->SetState(MARIO_STATE_JUMP);
		}
		break;
	}
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_TAIL);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_9:
		mario->SetPosition(2010, 300);
		break;
	case DIK_A: {
		if (CMario::GetInstance()->GetLevel() == MARIO_LEVEL_TAIL) {
			CMario::GetInstance()->SetState(MARIO_STATE_ATTACK);
		}
		break;
	}
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_A: {
		DebugOut(L"Up A\n");
		CMario::GetInstance()->pressA = false;
		if (CMario::GetInstance()->isHolding == true) // đang giữ rùa, thả nút a thì chuyển sang đá
		{
			CMario::GetInstance()->canKick = true;
			CMario::GetInstance()->isHolding = false;
		}
		break;
	}

	case DIK_S:
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE* states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
		{
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
			mario->pressA = true;
		}
		else {
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
			mario->pressA = true;
		}
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A)) {
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
			mario->pressA = true;

		}
		else {
			mario->SetState(MARIO_STATE_WALKING_LEFT);
			mario->pressA = true;
		}
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}