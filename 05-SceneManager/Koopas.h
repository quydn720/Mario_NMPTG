#pragma once
#include "GameObject.h"
#include "FallDetector.h"

#define KOOPAS_GRAVITY 0.002f
#define KOOPAS_WALKING_SPEED 0.05f
#define KOOPAS_SHELL_SPEED 0.1f

#define KOOPAS_STATE_WALKING 220
#define KOOPAS_STATE_SHELL 221
#define KOOPAS_STATE_SHELL_MOVING 222
#define KOOPAS_STATE_GOING_UP 223

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT_SHELL 16
#define KOOPAS_BBOX_HEIGHT 27

#define ID_ANI_KOOPAS_WALKING_LEFT 226
#define ID_ANI_KOOPAS_WALKING_RIGHT 227
#define ID_ANI_KOOPAS_SHELL 225
#define ID_ANI_KOOPAS_SHELL_MOVING 228
#define ID_ANI_KOOPAS_GOING_UP 229

#define KOOPAS_DIE_TIMEOUT 500
#define KOOPAS_SHELL_TIMEOUT 2500
#define KOOPAS_SPLASH_TIMEOUT 500

class CKoopas : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	ULONGLONG timer;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	FallDetector* fallDetector;
	CKoopas(float x, float y);
	virtual void SetState(int state);
	void beingKicked(int n) { nx = -n; x += nx; }
	void changeFromSmallToBig();
};

