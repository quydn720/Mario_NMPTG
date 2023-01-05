#pragma once
#include "GameObject.h"
#include "FallDetector.h"


#define KOOPAS_WINGS_VY -0.3f
#define KOOPAS_WINGS 1
#define KOOPAS_NORMAL 0


#define KOOPAS_GRAVITY 0.0015f
#define KOOPAS_WALKING_SPEED 0.05f
#define KOOPAS_SHELL_SPEED 0.1f

#define ENEMY_STATE_DIE_IS_JUMPED	0
#define ENEMY_STATE_DIE_IS_ATTACKED 1
#define ENEMY_STATE_INIT			2
#define ENEMY_STATE_WALKING_RIGHT	3
#define ENEMY_STATE_WALKING_LEFT	4
#define KOOPAS_STATE_SHELL					5
#define KOOPAS_STATE_SHELL_2				6
#define KOOPAS_STATE_SHELL_HOLD				7
#define KOOPAS_STATE_SHELL_2_HOLD			8
#define KOOPAS_STATE_SHELL_WALKING_RIGHT	9
#define KOOPAS_STATE_SHELL_WALKING_LEFT		10
#define KOOPAS_STATE_SHELL_2_WALKING_RIGHT	11
#define KOOPAS_STATE_SHELL_2_WALKING_LEFT	12
#define KOOPAS_STATE_DIE					13

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT_SHELL 14
#define KOOPAS_BBOX_HEIGHT 28

#define ID_ANI_KOOPAS_SHELL 225
#define ID_ANI_KOOPAS_WALKING_LEFT 226
#define ID_ANI_KOOPAS_WALKING_RIGHT 227

#define ID_ANI_KOOPAS_SHELL_MOVING_LEFT 228
#define ID_ANI_KOOPAS_SHELL_MOVING_RIGHT 229
#define ID_ANI_KOOPAS_REVIVE 230
#define ID_ANI_KOOPAS_SHELL2	231
#define ID_ANI_KOOPAS_SHELL2_MOVING_LEFT 232
#define ID_ANI_KOOPAS_SHELL2_MOVING_RIGHT 233
#define ID_ANI_KOOPAS_REVIVE2 234

#define KOOPAS_DIE_TIMEOUT 500
#define KOOPAS_SHELL_TIMEOUT 2500
#define KOOPAS_SPLASH_TIMEOUT 500

#define KOOPAS_START_REVIVE_TIME	5000
#define KOOPAS_REVIVE_TIME					8000

class CKoopas : public CGameObject
{
private:
	int level;
public:
	float ax;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	bool canRevive;
	int Health;
	ULONGLONG ReviveTime;
	bool isKicked, isShell, isShell_2, isHold;
	FallDetector* fallDetector;
	CKoopas(float x, float y, int level = KOOPAS_NORMAL);
	virtual void SetState(int state);
	void beingKicked(int n) { nx = -n; x += nx; }
};

