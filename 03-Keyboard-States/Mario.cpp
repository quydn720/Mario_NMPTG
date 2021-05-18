#include "Mario.h"
#include "debug.h"

void CMario::Update(DWORD dt)
{
	CGameObject::Update(dt);

	// This example, y == 100 means Mario is on the ground.
	// simple fall down
	vy += MARIO_GRAVITY;

	//DebugOut(L"vy: %f\n", vy);
	if (y >= 100)
	{
		if (isSitting) {
			vy = 0;
		}
		vy = 0; y = 100.0f;
	}
	else if (isAttacking) {
		if (time_attack >= 4 * MARIO_ATTACK_EACH_ANI_TIME) {
			time_attack = 0;
			isAttacking = false;
		}
		else {
			time_attack++;
		}
	}

	// simple screen edge collision!!!
	if (vx > 0 && x > 290) x = 290;
	if (vx < 0 && x < 0) x = 0;
}

void CMario::Render()
{
	int ani;

	if (vy > 0) {
		isJumping = true;
		ani = MARIO_ANI_JUMPING;
	}
	else if (vy < 0) {
		isFalling = true;
		ani = MARIO_ANI_FALLING;
	}
	else if (vx == 0) { // Idle
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
			else ani = MARIO_ANI_IDLE_RIGHT;
		}
		else ani = MARIO_ANI_IDLE_LEFT;
	}
	else if (vx > 0)
		ani = MARIO_ANI_WALKING_RIGHT;
	else ani = MARIO_ANI_WALKING_LEFT;

	animations[ani]->Render(x, y);
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_ATTACK:
		isAttacking = true;
		break;
	case MARIO_STATE_JUMP:
		isJumping = true;
		vy = -MARIO_JUMP_SPEED_Y;
		vy = -0.8f;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	}
}

