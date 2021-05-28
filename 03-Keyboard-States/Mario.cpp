#include "Mario.h"
#include "debug.h"

void CMario::Update(DWORD dt)
{
	CGameObject::Update(dt);

	// This example, y == 100 means Mario is on the ground.
	// simple fall down
	vy += 0.005f;

	//DebugOut(L"y: %f\n", y);
	if (y >= 100)
	{
		isOnGround = true;
		isJumping = false;
		if (isSitting) {
			vy = 0;
			y = 110;
		}
		else {
			vy = 0;
			y = 100.0f;
		}
	}
	else {
		isJumping = true;
		isOnGround = false;
		isSitting = false;
	}

	if (isAttacking) {
		if (time_attack >= 4 * MARIO_ATTACK_EACH_ANI_TIME) {
			time_attack = 0;
			isAttacking = false;
		}
		else {
			time_attack++;
		}
	}

	if (this->GetState() == MARIO_STATE_SIT) {

		if (abs(vx) != 0) {
			if (nx == 1)
			{
				vx -= 0.001f;
				if (vx < 0)
					vx = 0;
			}
			else
			{
				vx += 0.001f;
				if (vx > 0)
					vx = 0;
			}

		}
	}

	// simple screen edge collision!!!
	if (vx > 0 && x > 290) x = 290;
	if (vx < 0 && x < 0) x = 0;
	Debug();
}

void CMario::Render()
{
	int ani;

	if (vy > 0) {
		isJumping = true;
		ani = MARIO_ANI_FALLING;
	}
	else if (vy < 0) {
		isJumping = true;
		ani = MARIO_ANI_JUMPING;
	}
	else if (this->GetState() == MARIO_STATE_IDLE) { // Idle
		if (nx > 0) {
			if (isAttacking) {
				if (time_attack <= MARIO_ATTACK_EACH_ANI_TIME)
					ani = MARIO_ANI_TAIL_ATTACK_1;
				else if (time_attack > MARIO_ATTACK_EACH_ANI_TIME && time_attack <= 2 * MARIO_ATTACK_EACH_ANI_TIME)
					ani = MARIO_ANI_TAIL_ATTACK_2;
				else if (time_attack > 2 * MARIO_ATTACK_EACH_ANI_TIME && time_attack <= 3 * MARIO_ATTACK_EACH_ANI_TIME)
					ani = MARIO_ANI_TAIL_ATTACK_3;
				else if (time_attack > 3 * MARIO_ATTACK_EACH_ANI_TIME && time_attack <= 4 * MARIO_ATTACK_EACH_ANI_TIME)
					ani = MARIO_ANI_TAIL_ATTACK_4;
			}
			else ani = MARIO_TAIL_ANI_IDLE_RIGHT;
		}
		else ani = MARIO_ANI_IDLE_LEFT;
	}
	else if (vx > 0)
		ani = MARIO_ANI_WALKING_RIGHT;
	else ani = MARIO_ANI_WALKING_LEFT;
	if (this->GetState() == MARIO_STATE_SIT && this->isSitting) {
		ani = MARIO_ANI_SITTING;
	}

	animations[ani]->Render(x, y);
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	this->state = state;// :))
	switch (state) {
	case MARIO_STATE_JUMP:
		if (isOnGround && !isSitting) {
			isJumping = true;
			vy = -MARIO_JUMP_SPEED_Y;
			vy = -0.2f;
		}
		break; 
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		isSitting = false;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		isSitting = false;
		nx = -1;
		break;
	case MARIO_STATE_ATTACK:
		isAttacking = true;
		break;
	case MARIO_STATE_SIT:
		if (!isJumping) {
			isSitting = true;
		}
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		isSitting = false;
		break;
	}
}

void CMario::Debug()
{
	switch (this->state)
	{
	case MARIO_STATE_IDLE:
		DebugOut(L"State = IDLE\t"); break;
	case MARIO_STATE_WALKING_RIGHT:
		DebugOut(L"State = WALKING_RIGHT\t"); break;
	case MARIO_STATE_WALKING_LEFT:
		DebugOut(L"State = WALKING_LEFT\t"); break;
	case MARIO_STATE_JUMP:
		DebugOut(L"State = JUMP\t"); break;
	case MARIO_STATE_SIT:
		DebugOut(L"State = SIT\t"); break;
	}

	if (isSitting == true)
		DebugOut(L"isSitting == true\t");
	else
		DebugOut(L"isSitting == false\t");
	if (isJumping == true)
		DebugOut(L"isJumping == true\t");
	else
		DebugOut(L"isJumping == false\t");


	DebugOut(L"\nMario y = %f\n",y);
	//DebugOut(L"ani = %i", ani);
	DebugOut(L"\n");

}