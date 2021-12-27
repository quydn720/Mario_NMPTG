#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

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
	isOnPlatform = false;

	if (state != MARIO_STATE_IDLE)
		vx += ax * dt;

	if (abs(vx) > abs(maxVx)) {
		vx = maxVx;
	}

	if (isAttack)
	{
		if (nx > 0)
			tail->SetPosition(x + MARIO_BIG_BBOX_WIDTH / 2 + 8 / 2, y + 5);
		else
			tail->SetPosition(x - MARIO_BIG_BBOX_WIDTH / 2 - 8 / 2, y + 5);


		tail->Update(dt, coObjects);
		if (GetTickCount64() - timer >= 300)
		{
			isAttack = false;
			timer = 0;
		}
	}

	if (isKicking) {
		if (GetTickCount64() - timer >= 300) {
			isKicking = false;
			timer = 0;
		}
	}
	if (isSitting) {
		state = MARIO_STATE_SIT;
	}
	if (isPiping == false) {
		vy += ay * dt;
	}
	else {
		if (GetTickCount64() - timer >= MARIO_PIPING_TIME) {
			isPiping = false;

			CGame::GetInstance()->InitiateSwitchScene(_switchSceneId);
		}
		else {
			ay = 0.0f;
			isOnPlatform = true;
			y += vy * dt;
		}
	}

	if (level == MARIO_LEVEL_TAIL) {
		if (tail->GetState() == TAIL_ACTIVE)
			tail->SetPosition((nx > 0) ? x - TAIL_LENGTH : x + TAIL_LENGTH, y);
	}

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
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
		//vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
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
}

void CMario::OnCollisionWithItem(LPCOLLISIONEVENT e) {
	if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CSuperItem*>(e->obj)) {
		CSuperItem* superItem = dynamic_cast<CSuperItem*>(e->obj);
		if (superItem->type == SuperItemType::RedMushroom) {
			OnCollisionWithMushroom(e);
		}
		else if (superItem->type == SuperItemType::Leaf) {
			OnCollisionWithLeaf(e);
		}
	}
}

void CMario::OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e) {
	CQuestionBlock* qb = dynamic_cast<CQuestionBlock*>(e->obj);
	if (e->ny > 0) {
		qb->SpawnItem(nx, level);
	}
}
void CMario::OnCollisionWithPipe(LPCOLLISIONEVENT e) {
	CWarpPipe* pipe = dynamic_cast<CWarpPipe*>(e->obj);
	if (pipe->canGoThroughtScene()) {
		float x1, y1;
		pipe->GetPosition(x1, y1);
		if (e->ny < 0) {
			if (x > x1 && x < x1 + PIPE_BBOX_DOWN_RANGE) {
				if (state == MARIO_STATE_SIT) {
					isPiping = true;
					vy = MARIO_PIPING_VY;
					_switchSceneId = pipe->canGoThroughtScene();
					timer = GetTickCount64();
				}
			}
		}
		else if (e->ny > 0) {
			if (x > x1 && x < x1 + PIPE_BBOX_DOWN_RANGE) {
				isPiping = true;
				vy = -MARIO_PIPING_VY;
				_switchSceneId = pipe->canGoThroughtScene();
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
		if (koopas->GetState() == KOOPAS_STATE_SHELL || koopas->GetState() == KOOPAS_STATE_GOING_UP) {
			// shell idle => kick
			timer = GetTickCount64();
			isKicking = true;
			koopas->beingKicked(nx);
			koopas->SetState(KOOPAS_STATE_SHELL_MOVING);
		}
		else {
			// shell moving & walking => die
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
	if (e->ny < 0) {// jump on top
		if (koopas->GetState() == KOOPAS_STATE_SHELL_MOVING) {
			// shell moving => shell idle
			koopas->changeFromSmallToBig();
			koopas->SetState(KOOPAS_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopas->GetState() == KOOPAS_STATE_WALKING) {
			// walking => shell idle
			koopas->SetState(KOOPAS_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopas->GetState() == KOOPAS_STATE_SHELL || koopas->GetState() == KOOPAS_STATE_GOING_UP) {
			// shell idle / going up => kick
			koopas->beingKicked(nx);
			koopas->SetState(KOOPAS_STATE_SHELL_MOVING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
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
	if (e->obj->GetState() == STATE_ITEM_VISIBLE) {
		e->obj->Delete();
		coin++;
	}
}
void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CSuperItem* superItem = dynamic_cast<CSuperItem*>(e->obj);
	if (superItem->IsAlive) {
		e->obj->SetState(STATE_ITEM_DIE);
		SetLevel(MARIO_LEVEL_BIG);
		e->obj->Delete();
	}
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CSuperItem* superItem = dynamic_cast<CSuperItem*>(e->obj);
	if (superItem->IsAlive) {
		e->obj->SetState(STATE_ITEM_DIE);
		SetLevel(MARIO_LEVEL_TAIL);
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
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (state == MARIO_STATE_IDLE) {
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
				}
				else if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (state == MARIO_STATE_IDLE) {
					aniId = ID_ANI_MARIO_WALKING_LEFT;
				}
				else if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdTail()
{
	int aniId = -1;
	// ON AIR
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0) // TODO: && vx >= vxMax
				aniId = (vy < 0) ? ID_ANI_MARIO_TAIL_JUMP_RUN_UP_RIGHT : ID_ANI_MARIO_TAIL_JUMP_RUN_DOWN_RIGHT;
			else
				aniId = (vy < 0) ? ID_ANI_MARIO_TAIL_JUMP_RUN_UP_LEFT : ID_ANI_MARIO_TAIL_JUMP_RUN_DOWN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = (vy < 0) ? ID_ANI_MARIO_TAIL_JUMP_UP_RIGHT : ID_ANI_MARIO_TAIL_JUMP_DOWN_RIGHT;
			else
				aniId = (vy < 0) ? ID_ANI_MARIO_TAIL_JUMP_UP_LEFT : ID_ANI_MARIO_TAIL_JUMP_DOWN_LEFT;
		}
	}
	else
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
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
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
					aniId = ID_ANI_MARIO_WALKING_LEFT;
				}
				else if (ax > 0)
					aniId = ID_ANI_MARIO_TAIL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_TAIL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_TAIL_WALKING_LEFT;
			}

	if (isPiping) { // TODO: add another level ani
		aniId = ID_ANI_MARIO_TAIL_PIPING;
	}
	if (isKicking) { // TODO: add another level ani
		aniId = (nx > 0) ? ID_ANI_MARIO_TAIL_KICKING_RIGHT : ID_ANI_MARIO_TAIL_KICKING_LEFT;
	}

	if (aniId == -1) aniId = ID_ANI_MARIO_TAIL_IDLE_RIGHT;

	return aniId;
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

	if (ax != 0) {
		DebugOut(L"[After] a: %f\tvx: %f\tx: %0.2f\Ani:%d\n", ax, vx, x, aniId);
	}
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case -99:
		isAttack = true;
		timer = GetTickCount64();
		break;
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (level == MARIO_LEVEL_TAIL) {
			if (isSitting) break;
			if (isOnPlatform)
			{
				if (abs(this->vx) == MARIO_RUNNING_SPEED)
				{
					vy = -MARIO_JUMP_RUN_SPEED_Y;
					//ay = 0.0012f;
				}
				else
					vy = -MARIO_JUMP_SPEED_Y;
			}
		}
		else {
			if (isSitting) break;
			if (isOnPlatform)
			{
				if (abs(this->vx) == MARIO_RUNNING_SPEED)
					vy = -MARIO_JUMP_RUN_SPEED_Y;
				else
					vy = -MARIO_JUMP_SPEED_Y;
			}
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (isPiping == false) {
			if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		}
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;
	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

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
			left += (nx < 0) ? -MARIO_TAIL_WIDTH : MARIO_TAIL_WIDTH;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			left += (nx < 0) ? -MARIO_TAIL_WIDTH : MARIO_TAIL_WIDTH;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;

			//left = left + 4;
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
}

void CMario::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float tx;
	tx = (x - cx);
	if (level == MARIO_LEVEL_TAIL)
		tx += (nx < 0) ? -MARIO_TAIL_WIDTH : MARIO_TAIL_WIDTH;
	CGame::GetInstance()->Draw(tx, y - cy, bbox, &rect, BBOX_ALPHA);
}