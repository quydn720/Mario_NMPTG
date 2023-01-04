#pragma once
#include "GameObject.h"
#include "Tail.h"

#pragma region define

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0003f

#define MARIO_ACCEL_SLOWING_DOWN_X_PARAM 15

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.002f
#define MARIO_PIPING_VY			0.0055f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_GIU_NUT_S 2398

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_FLY 303

#define MARIO_STATE_DOWN_PIPE 305

#define MARIO_STATE_ATTACK	800
#define MARIO_STATE_HOLDING_IDLE	900
#define MARIO_STATE_FLYING_HIGH_RIGHT	1000
#define MARIO_STATE_FLYING_HIGH_LEFT	1100
#define MARIO_STATE_HOLDING_RIGHT		1200
#define MARIO_STATE_HOLDING_LEFT		1300

#define MARIO_FLY_DOWN					1400 // mario dang roi dan`

#pragma region ANIMATION_ID
// BIG
#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1001
#define ID_ANI_MARIO_BRACE_LEFT 1000

#define ID_ANI_MARIO_HOLDING_IDLE_RIGHT 1002
#define ID_ANI_MARIO_HOLDING_IDLE_LEFT 1003
#define ID_ANI_MARIO_HOLDING_WALKING_RIGHT 1004
#define ID_ANI_MARIO_HOLDING_WALKING_LEFT 1005
#define ID_ANI_MARIO_HOLDING_JUMPING_RIGHT 1006
#define ID_ANI_MARIO_HOLDING_JUMPING_LEFT 1007
#define ID_ANI_MARIO_KICKING_RIGHT 1008
#define ID_ANI_MARIO_KICKING_LEFT 1009

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

// TAIL
#define ID_ANI_MARIO_TAIL_IDLE_RIGHT 3100
#define ID_ANI_MARIO_TAIL_IDLE_LEFT 3102

#define ID_ANI_MARIO_TAIL_WALKING_RIGHT 3200
#define ID_ANI_MARIO_TAIL_WALKING_RIGHT_FAST 3202
#define ID_ANI_MARIO_TAIL_WALKING_LEFT 3201
#define ID_ANI_MARIO_TAIL_WALKING_LEFT_FAST 3203

#define ID_ANI_MARIO_TAIL_RUNNING_RIGHT 3300
#define ID_ANI_MARIO_TAIL_RUNNING_LEFT 3301

#define ID_ANI_MARIO_TAIL_BRACE_RIGHT 3400
#define ID_ANI_MARIO_TAIL_BRACE_LEFT 3401

#define ID_ANI_MARIO_TAIL_JUMP_UP_RIGHT 3500
#define ID_ANI_MARIO_TAIL_JUMP_DOWN_RIGHT 3501

#define ID_ANI_MARIO_TAIL_JUMP_UP_LEFT 3502
#define ID_ANI_MARIO_TAIL_JUMP_DOWN_LEFT 3503

#define ID_ANI_FLY_DOWN 2003
#define ID_ANI_MARIO_TAIL_JUMP_RUN_UP_RIGHT 3600
#define ID_ANI_MARIO_TAIL_JUMP_RUN_DOWN_RIGHT 3601

#define ID_ANI_MARIO_TAIL_JUMP_RUN_UP_LEFT 3602
#define ID_ANI_MARIO_TAIL_JUMP_RUN_DOWN_LEFT 3603

#define ID_ANI_MARIO_TAIL_SIT_RIGHT 3900
#define ID_ANI_MARIO_TAIL_SIT_LEFT 3901

#define ID_ANI_MARIO_TAIL_ATTACK_RIGHT	19180
#define ID_ANI_MARIO_TAIL_ATTACK_LEFT	19190

#define ID_ANI_MARIO_TAIL_PIPING -2000
#define ID_ANI_MARIO_TAIL_KICKING_RIGHT -2010
#define ID_ANI_MARIO_TAIL_KICKING_LEFT -2011


#pragma endregion

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_TAIL		3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_TAIL_BBOX_WIDTH  22
#define MARIO_TAIL_BBOX_HEIGHT 28

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12

#define MARIO_PIPING_TIME	2500

#define MARIO_UNTOUCHABLE_TIME 2500
#define RACOON_ATTACK_TIME 250
#pragma endregion

class CMario : public CGameObject
{
	static CMario* __instance;

	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 

	bool isFlying;
	
	int untouchable;
	ULONGLONG untouchable_start;

	void RenderBoundingBox();

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithPlant(LPCOLLISIONEVENT e);

	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e);
	// Item
	void OnCollisionWithItem(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPipe(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdTail();

public:
	int level;
	static CMario* GetInstance();

	bool render_tail; // Đã vẽ đuôi hay chưa để thoát vòng lặp vẽ đuôi bên playscene
	BOOLEAN isOnPlatform;
	CTail* tail;

	bool isHolding = false, pressA = false, canKick = false;
	bool changeDirection = false;
	int coin;
	bool isOnTopWarpPipe;
	bool isPiping;
	bool IsAttack;
	DWORD AttackTime;
	ULONGLONG timer;
	ULONGLONG flyTimer;

	// Use only one in the initial playscene. not a good practice to put as public method.
	static void SetInstance(CMario* p);

	CMario(float x, float y) : CGameObject(x, y)
	{
		IsAttack = isSitting = false;
		ax = 0.0f;

		level = MARIO_LEVEL_BIG;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
		tail = NULL;
		AttackTime = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{
		return (state != MARIO_STATE_DIE);
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable == 0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	int GetLevel() { return level; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void HandleRacoonAttack(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	float getX() { return x; }
	float getY() { return y; }
	int getNx() { return nx; }
};