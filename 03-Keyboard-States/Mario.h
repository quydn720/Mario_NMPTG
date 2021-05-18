#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_GRAVITY			0.1f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_ATTACK			400

#define MARIO_ANI_IDLE_RIGHT		0
#define MARIO_ANI_IDLE_LEFT			1
#define MARIO_ANI_WALKING_RIGHT		2
#define MARIO_ANI_WALKING_LEFT		3

#define MARIO_ANI_TAIL_ATTACK_1		4
#define MARIO_ANI_TAIL_ATTACK_2		5
#define MARIO_ANI_TAIL_ATTACK_3		6
#define MARIO_ANI_TAIL_ATTACK_4		7

#define MARIO_ATTACK_EACH_ANI_TIME	50

class CMario : public CGameObject
{
	CAnimation* animation;
	bool isAttack, endAttack;
	int time_attack = 0;
public: 

	void Update(DWORD dt);
	void Render();
	void SetState(int state);
};