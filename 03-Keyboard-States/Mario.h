#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_GRAVITY			0.1f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300

#define MARIO_TAIL_STATE_IDLE		350
#define MARIO_STATE_ATTACK			400
#define MARIO_STATE_JUMP			500
#define MARIO_STATE_FALLING			600

#define MARIO_STATE_SIT				700

#define MARIO_TAIL_STATE_IDLE		800

#define MARIO_ANI_IDLE_RIGHT		0
#define MARIO_ANI_IDLE_LEFT			1
#define MARIO_ANI_WALKING_RIGHT		2
#define MARIO_ANI_WALKING_LEFT		3

#define MARIO_ANI_TAIL_ATTACK_1		4
#define MARIO_ANI_TAIL_ATTACK_2		5
#define MARIO_ANI_TAIL_ATTACK_3		6
#define MARIO_ANI_TAIL_ATTACK_4		7

#define MARIO_ANI_JUMPING			8
#define MARIO_ANI_FALLING			9
#define MARIO_ANI_SITTING			10

#define MARIO_TAIL_ANI_IDLE_RIGHT	11

#define MARIO_ATTACK_EACH_ANI_TIME	5

class CMario : public CGameObject
{
	int state;
	bool isAttacking = false;
	int time_attack = 0;

	bool isJumping = false;
	bool isFalling = false;
	bool isSitting = false;
public: 
	void Debug();
	void Update(DWORD dt);
	void Render();
	void SetState(int state);
};

//enum MarioState {
//	MARIO_STATE_IDLE,
//	MARIO_STATE_WALKING_RIGHT,
//	MARIO_STATE_WALKING_LEFT,
//	MARIO_STATE_JUMP,
//
//	MARIO_TAIL_STATE_IDLE,
//	MARIO_STATE_ATTACK,
//	MARIO_STATE_JUMP,
//	MARIO_STATE_FALLING,
//
//	MARIO_STATE_SIT,
//
//	MARIO_TAIL_STATE_IDLE
//};