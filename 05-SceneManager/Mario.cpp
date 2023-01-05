#include <algorithm>

#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Coin.h"
#include "Portal.h"

#include "Collision.h"
#include "ColorBlock.h"
#include "QuestionBlock.h"
#include "Plant.h"
#include "Textures.h"
#include "WarpPipe.h"
#include "ButtonP.h"
#include "PlayScene.h"

#define MARIO_FALLING_SPEED_SLOW 0.1f // tốc độ rơi Mario, bị hãm bởi nút S vẫy đuôi
#define MARIO_TAIL_ATTACK_TIME 250 // 250 = voi tong thoi gian animation

CMario* CMario::__instance = NULL;
int _switchSceneId = 0;

CMario* CMario::GetInstance()
{
	return __instance;
}
void CMario::SetInstance(CMario* p)
{
	__instance = p;
}
void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*if (vy <= -0.5f) {
		DebugOut(L"MAX SPEED Y\n");
		ay = 0.002f;
	}*/

	//DebugOut(L"ay: %0.4f	vy: %0.4f\n", ay, vy);

	// Mario reach max speed 

	// vy += ay * dt;
	if (!isFlying) {
		vy += MARIO_GRAVITY * dt;
	}
	isOnPlatform = false;

	if (state != MARIO_STATE_IDLE)
		vx += ax * dt;

	if (abs(vx) > abs(maxVx)) {
		vx = maxVx;
		if (level == 3) {
			DebugOut(L"can fly\n");
		}
		/*DebugOut(L"mario at max speed\n");*/
	}

	if (isSitting) {
		state = MARIO_STATE_SIT;
	}
	else if (canKick == true)
		// reset lại canKick và hiện ani đá khi canKick=true
	{
		if (AttackTime != 0 && GetTickCount64() - AttackTime > 100)
		{
			canKick = false;
			AttackTime = 0;
		}

	}

	if (isPiping) {
		if (GetTickCount64() - timer >= MARIO_PIPING_TIME) {
			isPiping = false;

			CGame::GetInstance()->InitiateSwitchScene(_switchSceneId);
		}
		else {
			// ay = 0.0f;
			isOnPlatform = true;
			y += vy * dt;
		}
	}

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (isFlying)
	{
		if (GetTickCount64() - flyTimer > 400)
		{
			timer = 0;
			isFlying = false;
		}
	}

	HandleRacoonAttack(dt, coObjects);

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CQuestionBlock*>(e->obj))
		OnCollisionWithQuestionBlock(e);
	else if (dynamic_cast<Item*>(e->obj))
		OnCollisionWithItem(e);
	else if (dynamic_cast<CKoopas*>(e->obj))
		OnCollisionWithKoopas(e);
	else if (dynamic_cast<CPlant*>(e->obj))
		OnCollisionWithPlant(e);
	else if (dynamic_cast<CWarpPipe*>(e->obj))
		OnCollisionWithPipe(e);
	else if (dynamic_cast<ButtonP*>(e->obj))
	{
		ButtonP* buttonP = dynamic_cast<ButtonP*>(e->obj);
		buttonP->SetState(BUTTON_P_STATE_PUSHED);
	}
}

void CMario::OnCollisionWithItem(LPCOLLISIONEVENT e) {
	if (dynamic_cast<CCoin*>(e->obj))
	{
		OnCollisionWithCoin(e);
	}
	else
	{
		if (dynamic_cast<CSuperItem*>(e->obj)) {
			CSuperItem* superItem = dynamic_cast<CSuperItem*>(e->obj);
			if (superItem->type == SuperItemType::RedMushroom) {
				OnCollisionWithMushroom(e);
			}
			else if (superItem->type == SuperItemType::Leaf) {
				OnCollisionWithLeaf(e);
			}
		}
	}
}

void CMario::OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e) {
	CQuestionBlock* qb = dynamic_cast<CQuestionBlock*>(e->obj);
	if (e->ny > 0) {
		if (qb->GetState() != STATE_BRICK_EMPTY)
			qb->SetState(STATE_BRICK_HIT);
	}
}
void CMario::OnCollisionWithPipe(LPCOLLISIONEVENT e) {
	CWarpPipe* pipe = dynamic_cast<CWarpPipe*>(e->obj);

	// có sceneId gán vào cái cống
	bool canTeleport = pipe->GetDestinationSceneId() != -1;

	if (canTeleport) {
		float x1, y1;
		pipe->GetPosition(x1, y1);

		// mario nằm trong vùng có thể bấm S để chui cống
		bool inRange = x > x1 && x < x1 + PIPE_BBOX_DOWN_RANGE;
		if (e->ny < 0) {
			if (inRange) {
				if (state == MARIO_STATE_SIT) {
					isPiping = true;
					vy = MARIO_PIPING_VY;
					_switchSceneId = pipe->GetDestinationSceneId();
					timer = GetTickCount64();
				}
			}
		}
		else if (e->ny > 0) {
			if (inRange) {
				isPiping = true;
				vy = -MARIO_PIPING_VY;
				_switchSceneId = pipe->GetDestinationSceneId();
				timer = GetTickCount64();
			}
		}
	}
}

#pragma region Enemies

void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
	if (e->nx != 0) {	// horizontal
		// k ở dạng mai rùa thì giảm cấp
		if (koopas->isShell == false && koopas->isShell_2 == false)
		{
			if (untouchable == 0) {
				if (level > MARIO_LEVEL_SMALL)
				{
					level--;
					StartUntouchable();
				}
				else
					SetState(MARIO_STATE_DIE);
			}
		}
		// ở dạng mai rùa
		else
		{
			if (koopas->vx != 0 && (koopas->isShell == true || koopas->isShell_2 == true))
			{
				if (untouchable == 0) {
					if (level > MARIO_LEVEL_SMALL)
					{
						level--;
						StartUntouchable();
					}
					else
						SetState(MARIO_STATE_DIE);
				}
			}
			else
			{
				if (e->nx == -1)
				{
					if (pressA == true)
					{
						isHolding = true;
						koopas->nx = 1;
						if (koopas->isShell == true)
							koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
						else
							koopas->SetState(KOOPAS_STATE_SHELL_2_HOLD);
					}

				}
				else if (e->nx == 1)
				{
					if (pressA == true)
					{
						isHolding = true;
						koopas->nx = -1;
						if (koopas->isShell == true)
							koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
						else
							koopas->SetState(KOOPAS_STATE_SHELL_2_HOLD);
					}
				}
			}
		}
	}
	else if (e->ny < 0) {// jump on top
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		if (koopas->vx != 0 && koopas->isShell_2 == false) // bao gồm 4 trạng thái : rùa đi trái/phải, mai rùa đi trái/phải
		{
			koopas->y -= 1;
			koopas->SetState(KOOPAS_STATE_SHELL);
			koopas->ReviveTime = GetTickCount64();
		}
		else if (koopas->GetState() == KOOPAS_STATE_SHELL)
		{
			koopas->y -= 1;
			if (this->x <= koopas->x)
			{
				if (koopas->isShell == true)
					koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
			}
			else
			{
				if (koopas->isShell == true)
					koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
			}
			koopas->isKicked = true;
		}
	}
	else if (e->ny > 0)
	{
		if (koopas->isShell == false && koopas->isShell_2 == false)
		{
			if (untouchable == 0) {
				if (level > MARIO_LEVEL_SMALL)
				{
					level--;
					StartUntouchable();
				}
				else
					SetState(MARIO_STATE_DIE);
			}
		}
		else
		{
			if (pressA == true)
			{
				isHolding = true;
				if (koopas->isShell == true)
					koopas->SetState(KOOPAS_STATE_SHELL_HOLD);
				else
					koopas->SetState(KOOPAS_STATE_SHELL_2_HOLD);
			}
			else
			{
				this->canKick = koopas->isKicked = true;
				AttackTime = GetTickCount64();
				if (this->nx == 1)
				{
					if (koopas->isShell == true) koopas->SetState(KOOPAS_STATE_SHELL_WALKING_RIGHT);
				}
				else if (this->nx == -1)
				{
					if (koopas->isShell == true) koopas->SetState(KOOPAS_STATE_SHELL_WALKING_LEFT);
				}
				koopas->vy -= 0.05f;
			}
		}
	}
}
void CMario::OnCollisionWithPlant(LPCOLLISIONEVENT e)
{
	CPlant* plant = dynamic_cast<CPlant*>(e->obj);
	if (plant->GetState() == PLANT_STATE_UP) {
		if (untouchable == 0) {
			if (level > MARIO_LEVEL_SMALL) {
				level = MARIO_LEVEL_SMALL;
				StartUntouchable();
			}
			else {
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
	}
}
void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetLevel() == LEVEL_PARA_GOOMBA) {
		}
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			if (goomba->GetLevel() == LEVEL_GOOMBA)	goomba->SetState(GOOMBA_STATE_DIE);
			else goomba->SetLevel(LEVEL_GOOMBA);

			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

#pragma endregion

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	int state = (dynamic_cast<CCoin*>(e->obj))->GetState();

	bool collision = state == STATE_ITEM_VISIBLE || state == COIN_STATE_WAIT; 
	if (collision)
	{
		e->obj->Delete();
		coin++;
	}
}
void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CSuperItem* superItem = dynamic_cast<CSuperItem*>(e->obj);
	if (superItem->IsAlive) {
		e->obj->SetState(STATE_ITEM_DIE);
		if (level != MARIO_LEVEL_TAIL) {
			SetLevel(MARIO_LEVEL_BIG);
		}
		e->obj->Delete();
	}
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CSuperItem* superItem = dynamic_cast<CSuperItem*>(e->obj);
	if (superItem->IsAlive) {
		e->obj->SetState(STATE_ITEM_DIE);
		if (level != MARIO_LEVEL_TAIL) {
			SetLevel(MARIO_LEVEL_TAIL);
		}
		e->obj->Delete();
	}
}
void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}

//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (isHolding == true)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_HOLDING_JUMPING_RIGHT;
			else
				aniId = ID_ANI_MARIO_HOLDING_JUMPING_LEFT;
		}
		else
		{
			if (abs(ax) == MARIO_ACCEL_RUN_X)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
				else
					aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
			}
			else
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
				else
					aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
			}
		}
	}
	else
	{
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else if (isHolding)
		{
			if (vx == 0)
			{
				if (nx > 0)
					aniId = ID_ANI_MARIO_HOLDING_IDLE_RIGHT;
				else
					aniId = ID_ANI_MARIO_HOLDING_IDLE_LEFT;
			}
			else if (ax > 0)
			{
				aniId = ID_ANI_MARIO_HOLDING_WALKING_RIGHT;
			}
			else if (ax < 0)
			{
				aniId = ID_ANI_MARIO_HOLDING_WALKING_LEFT;
			}
		}
		else if (canKick == true)
		{
			// phải
			if (nx == 1) {
				aniId = ID_ANI_MARIO_KICKING_RIGHT;
			}
			// trái
			else {
				aniId = ID_ANI_MARIO_KICKING_LEFT;
			}
		}
		else
		{
			if (vx == 0)
			{
				if (nx > 0)
					aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else
					aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
				{
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
					if (vx == MARIO_RUNNING_SPEED)
						aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				}
				else //if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{

				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (abs(ax) == MARIO_ACCEL_RUN_X)
				{
					aniId = ID_ANI_MARIO_WALKING_LEFT;
					if (abs(vx) == MARIO_RUNNING_SPEED)
						aniId = ID_ANI_MARIO_RUNNING_LEFT;
				}
				else //if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;

			}
		}

	}


	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	//DebugOut(L"ani = %d",aniId);
	return aniId;
}

int CMario::GetAniIdTail()
{
	int aniId = -1;
	CAnimations* animations = CAnimations::GetInstance();


	// ON AIR
	if (!isOnPlatform) {
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0) // TODO: && vx >= vxMax
				aniId = (vy < 0) ? ID_ANI_MARIO_TAIL_JUMP_RUN_UP_RIGHT : ID_ANI_MARIO_TAIL_JUMP_RUN_DOWN_RIGHT;
			else
				aniId = (vy < 0) ? ID_ANI_MARIO_TAIL_JUMP_RUN_UP_LEFT : ID_ANI_MARIO_TAIL_JUMP_RUN_DOWN_LEFT;
		}
		else if (abs(ax) <= MARIO_ACCEL_WALK_X)
		{
			if (nx >= 0)
				aniId = (vy < 0) ? ID_ANI_MARIO_TAIL_JUMP_UP_RIGHT : ID_ANI_MARIO_TAIL_JUMP_DOWN_RIGHT;
			else
				aniId = (vy < 0) ? ID_ANI_MARIO_TAIL_JUMP_UP_LEFT : ID_ANI_MARIO_TAIL_JUMP_DOWN_LEFT;
		}
		if (isFlying) {
			DebugOut(L"FLY");

			aniId = nx * ID_ANI_FLY_DOWN;

		}
	}

	else {
		// SIT
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_TAIL_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_TAIL_SIT_LEFT;
		}
		else
			// GROUND
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_TAIL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_TAIL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (state == MARIO_STATE_IDLE) {
					aniId = ID_ANI_MARIO_TAIL_WALKING_RIGHT;
				}
				else if (ax < 0)
					aniId = ID_ANI_MARIO_TAIL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_TAIL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_TAIL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (state == MARIO_STATE_IDLE) {
					aniId = ID_ANI_MARIO_TAIL_WALKING_LEFT;
				}
				else if (ax > 0)
					aniId = ID_ANI_MARIO_TAIL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_TAIL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_TAIL_WALKING_LEFT;
			}
	}

	if (isPiping) { // TODO: add another level ani
		aniId = ID_ANI_MARIO_TAIL_PIPING;
	}

	if (IsAttack) {
		if (nx > 0) aniId = ID_ANI_MARIO_TAIL_ATTACK_RIGHT;
		else aniId = ID_ANI_MARIO_TAIL_ATTACK_LEFT;
	}
	if (aniId == -1) aniId = ID_ANI_MARIO_TAIL_IDLE_RIGHT;

	return aniId;
}

void CMario::HandleRacoonAttack(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - AttackTime > MARIO_TAIL_ATTACK_TIME && IsAttack) { 
		CTail* tail = new CTail(x + nx * MARIO_BIG_BBOX_WIDTH, y);
		CPlayScene::GetInstance()->AddNewObject(tail);
		IsAttack = false;
	}
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_TAIL)
		aniId = GetAniIdTail();
	animations->Get(aniId)->Render(x, y);
	RenderBoundingBox();

	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state) {
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state) {
	case MARIO_STATE_RUNNING_RIGHT: {
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	}

	case MARIO_STATE_RUNNING_LEFT: {
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	}
	case MARIO_STATE_WALKING_RIGHT: {

		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	}
	case MARIO_STATE_WALKING_LEFT: {
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	}
	case MARIO_STATE_JUMP: {
		if (isSitting) break;
		if (isOnPlatform) {
			if (abs(this->vx) >= MARIO_RUNNING_SPEED) {
				vy = -MARIO_JUMP_RUN_SPEED_Y;
				// ay += 0.03f;

				//ay = 0.0012f;
			}
			else {
				vy = -MARIO_JUMP_SPEED_Y;
			}
			break;
		}
	}
	case MARIO_FLY_DOWN: {
		if (level == MARIO_LEVEL_TAIL) {
			if (abs(vx) >= abs(MARIO_RUNNING_SPEED)) {
				vy = -4 * MARIO_FALLING_SPEED_SLOW;
			}
			else if (vy > 0) {
				vy = MARIO_FALLING_SPEED_SLOW;
				isFlying = true;
				flyTimer = GetTickCount64();
			}
		}
		break;
	}
	case MARIO_STATE_RELEASE_JUMP: {
		if (vy < 0 && isFlying == false) vy += MARIO_JUMP_SPEED_Y / 2;
		break;
	}
	case MARIO_STATE_SIT: {
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;
	}

	case MARIO_STATE_SIT_RELEASE: {
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;
	}


	case MARIO_STATE_IDLE:
		if (vx != 0)
		{
			// When change from moving state => idle.
			// We +/- the velocity to 0 => make Mario moving a little bit more before stopping.
			// MARIO_ACCEL_SLOWING_DOWN_X_PARAM: increase this for Mario moving shorter.
			if (nx == 1) {
				vx += -ax * MARIO_ACCEL_SLOWING_DOWN_X_PARAM;
				maxVx += -ax * MARIO_ACCEL_SLOWING_DOWN_X_PARAM;

				if (vx < 0 || maxVx < 0) {
					vx = 0;
					maxVx = 0;
				}
			}
			else {
				vx -= ax * MARIO_ACCEL_SLOWING_DOWN_X_PARAM;
				maxVx -= ax * MARIO_ACCEL_SLOWING_DOWN_X_PARAM;

				if (vx > 0 || maxVx > 0) {
					vx = 0;
					maxVx = 0;
				}
			}
		}
		else ax = 0;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;

	case MARIO_STATE_ATTACK: {
		IsAttack = true;
		AttackTime = GetTickCount64();
		break;
	}
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_TAIL) {
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}


void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		x -= nx; // push mario 1 pixel -- maybe if check if mario is collision x with block
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
	if (level == MARIO_LEVEL_TAIL && this->render_tail == false)
	{
		CTail* tail = new CTail(this->x, this->y + 18);
		_PlayScene->AddNewObject(tail);
		render_tail = true;
	}

}
