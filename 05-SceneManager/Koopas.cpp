#include "Koopas.h"
#include "debug.h"
#include "Mario.h"
#include "Platform.h"
#include "ColorBlock.h"
#include "QuestionBlock.h"


CKoopas::CKoopas(float x, float y, int level)
{
	isKicked = isShell = isShell_2 = isHold = canRevive = false;
	ReviveTime = 0;
	Health = 1;
	SetState(ENEMY_STATE_INIT);
	this->level = level;
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	if (isShell == false && isShell_2 == false)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT;
		// 359.6
	}
	else
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT_SHELL / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT_SHELL;
		if (isHold == true)
		{
			left = left - 1.f;
			right = right - 1.f;
		}
	}
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case ENEMY_STATE_WALKING_RIGHT:
		{
			nx = 1;
			vx = nx * KOOPAS_WALKING_SPEED;
			isHold = isShell = isShell_2 = false;
		}
		break;

		case ENEMY_STATE_WALKING_LEFT:
		{
			nx = -1;
			vx = nx * KOOPAS_WALKING_SPEED;
			isHold = isShell = isShell_2 = false;
		}
		break;

		case KOOPAS_STATE_SHELL:
		{
			isShell = true;
			isShell_2 = false;
			isKicked = false;
			vx = 0;
			fallDetector->SetState(FALL_DETECTOR_STATE_INACTIVE);
		}
		break;

		case KOOPAS_STATE_SHELL_2:
		{
			isShell = false;
			isShell_2 = true;
			isKicked = false;
			vx = 0;
			fallDetector->SetState(FALL_DETECTOR_STATE_INACTIVE);
		}
		break;

		case KOOPAS_STATE_SHELL_WALKING_RIGHT:
		{
			//y = y - 5;
			nx = 1;
			isHold = false;
			isShell = true;
			isShell_2 = false;
			vx = nx * KOOPAS_SHELL_SPEED;
		}break;

		case KOOPAS_STATE_SHELL_WALKING_LEFT:
		{
			//y = y - 5;
			nx = -1;
			isHold = false;
			isShell = true;
			isShell_2 = false;
			vx = nx * KOOPAS_SHELL_SPEED;
		}break;

		case KOOPAS_STATE_SHELL_HOLD:
		{
			isHold = true;
			isShell = true;
			isShell_2 = false;
			fallDetector->SetState(FALL_DETECTOR_STATE_INACTIVE);
		}
		break;

		case KOOPAS_STATE_SHELL_2_HOLD:
		{
			isHold = true;
			isShell = false;
			isShell_2 = true;
			fallDetector->SetState(FALL_DETECTOR_STATE_INACTIVE);
		}
		break;

	}
}

void CKoopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}

	if (dynamic_cast<CGoomba*>(e->obj)) {

		CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
		goomba->SetState(GOOMBA_STATE_DIE);
		DebugOut(L"KOOPAS HIT GOOMBA\n");
	}

	if (state == KOOPAS_STATE_SHELL_WALKING_RIGHT || state == KOOPAS_STATE_SHELL_WALKING_LEFT) {
		if (dynamic_cast<CQuestionBlock*>(e->obj)) {
			CQuestionBlock* block = dynamic_cast<CQuestionBlock*>(e->obj);
			if (block->GetState() != STATE_BRICK_EMPTY) {
				block->SetState(STATE_BRICK_HIT);
				DebugOut(L"HIT QUESTION BLOCK");
			}
		}
	}

	if (GetState() == ENEMY_STATE_INIT)
		SetState(ENEMY_STATE_WALKING_LEFT);
	if (dynamic_cast<CPlatform*>(e->obj) || dynamic_cast<CColorBlock*>(e->obj))
	{
		if (e->ny < 0 && isShell == false && isShell_2==false)
			if(fallDetector->GetState() == FALL_DETECTOR_STATE_INACTIVE)
				fallDetector->SetState(FALL_DETECTOR_STATE_ACTIVE);
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	DebugOut(L"vx: %0.4f, vy: %0.4f\n", vx, vy);


	isOnPlatform = false;
	vy += KOOPAS_GRAVITY * dt;

	if (isHold == true)
	{
		canRevive = true;
		// Mario đang cầm mai rùa mà thả nút A ra thì đá rùa
		if (CMario::GetInstance()->pressA == false)
		{
			this->isHold = false;
			this->isKicked = true;
			CMario::GetInstance()->canKick = true;
			CMario::GetInstance()->AttackTime = GetTickCount64();
			if (CMario::GetInstance()->nx == 1)
			{
				if (isShell == true)
					SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
				else
					SetState(KOOPAS_STATE_SHELL_2_WALKING_RIGHT);
			}
			else
			{
				if (isShell == true)
					SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
				else
					SetState(KOOPAS_STATE_SHELL_2_WALKING_LEFT);
			}
			return;
		}
		// rùa đang bị cầm và nút A đang giữ
		else
		{
			if (CMario::GetInstance()->nx == 1)
			{
				if (isShell == true)
				{
					if (CMario::GetInstance()->level == MARIO_LEVEL_SMALL) // chuẩn
					{
						this->x = float(CMario::GetInstance()->x + MARIO_SMALL_BBOX_WIDTH + 1);
						this->y = CMario::GetInstance()->y - 2;
					}
					else if (CMario::GetInstance()->level == MARIO_LEVEL_TAIL) // chuẩn
					{
						this->x = CMario::GetInstance()->x + MARIO_TAIL_BBOX_WIDTH - 1.0f;
						this->y = CMario::GetInstance()->y + 6;
					}
					else // chuẩn
					{
						this->x = float(CMario::GetInstance()->x + MARIO_BIG_BBOX_WIDTH - 1.0f);
						this->y = CMario::GetInstance()->y + 6;
					}
				}
				else if (isShell_2 == true)
				{
					if (CMario::GetInstance()->level == MARIO_LEVEL_SMALL) // chuẩn
					{
						this->x = float(CMario::GetInstance()->x + MARIO_SMALL_BBOX_WIDTH - 1);
						this->y = float(CMario::GetInstance()->y - 4);
					}
					else if (CMario::GetInstance()->level == MARIO_LEVEL_TAIL) // chuẩn
					{
						this->x = float(CMario::GetInstance()->x + MARIO_TAIL_BBOX_WIDTH - 2);
						this->y = CMario::GetInstance()->y + 6;
					}
					else // chuẩn
					{
						this->x = float(CMario::GetInstance()->x + MARIO_BIG_BBOX_WIDTH - 2);
						this->y = CMario::GetInstance()->y + 6;
					}
				}
			}
			else
			{
				if (isShell == true)
				{
					if (CMario::GetInstance()->level == MARIO_LEVEL_SMALL)
					{
						this->x = float(CMario::GetInstance()->x - KOOPAS_BBOX_WIDTH + 1.0f);
						this->y = CMario::GetInstance()->y - 2;
					}
					else
					{
						this->x = float(CMario::GetInstance()->x - KOOPAS_BBOX_WIDTH + 1.0f);
						this->y = CMario::GetInstance()->y + 5;
					}
				}
				else
				{
					if (CMario::GetInstance()->level == MARIO_LEVEL_SMALL)
					{
						this->x = float(CMario::GetInstance()->x - KOOPAS_BBOX_WIDTH + 3.0f);
						this->y = CMario::GetInstance()->y - 4;
					}
					else
					{
						this->x = float(CMario::GetInstance()->x - KOOPAS_BBOX_WIDTH + 2.0f);
						this->y = CMario::GetInstance()->y + 5;
					}
				}
			}
		}

		if (GetTickCount64() - ReviveTime > KOOPAS_REVIVE_TIME)
		{
			ReviveTime = 0;
			if (canRevive == false)
			{
				CMario::GetInstance()->isHolding = false;
				SetState(KOOPAS_STATE_DIE);
			}
			else
			{
				y = (INT16)(y - (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) - 1);
				if (CMario::GetInstance()->nx == 1)
					SetState(ENEMY_STATE_WALKING_LEFT);
				else
					SetState(ENEMY_STATE_WALKING_RIGHT);
			}
		}
	}
	else
	{
		// When it moving on top of the color block, it will not fall when reaching edge
		if (fallDetector->willFall) {
			if (level == KOOPAS_NORMAL) vx = -vx; // ko canh' thi` thong minh hon
			nx = -nx;
		}

		// This will put the fall detector in front of the Koopas
		if (vx <= 0)
			fallDetector->SetPosition(x - 10, y);
		if (vx > 0)
			fallDetector->SetPosition(x + 10, y);
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);

	if (isOnPlatform && level == KOOPAS_WINGS) {
		vy = KOOPAS_WINGS_VY;
	}
}

void CKoopas::Render()
{
	int aniId = ID_ANI_KOOPAS_WALKING_LEFT;
	if (isShell == true)
	{
		aniId = ID_ANI_KOOPAS_SHELL;
		if (ReviveTime != 0 && GetTickCount64() - ReviveTime > KOOPAS_START_REVIVE_TIME)
			aniId = ID_ANI_KOOPAS_REVIVE;
	}
	else if (isShell_2 == true)
	{
		aniId = ID_ANI_KOOPAS_SHELL2;
		if (ReviveTime != 0 && GetTickCount64() - ReviveTime > KOOPAS_START_REVIVE_TIME)
			aniId = ID_ANI_KOOPAS_REVIVE2;
	}
	if (vx > 0 && isHold == false)
	{
		aniId = ID_ANI_KOOPAS_WALKING_RIGHT;
		if (isShell == true)
		{
			aniId = ID_ANI_KOOPAS_SHELL_MOVING_RIGHT;
		}
	}
	else if (vx < 0 && isHold == false)
	{
		aniId = ID_ANI_KOOPAS_WALKING_LEFT;
		if (isShell == true)
		{
			aniId = ID_ANI_KOOPAS_SHELL_MOVING_LEFT;
		}
	}


	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}