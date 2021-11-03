#pragma once
#include "GameObject.h"
#include "FallDetector.h"

#define KOOPAS_GRAVITY 0.002f
#define KOOPAS_WALKING_SPEED 0.07f

#define KOOPAS_STATE_WALKING 220
#define KOOPAS_STATE_DIE 229
#define KOOPAS_STATE_SHELL 221

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT_DIE 8
#define KOOPAS_BBOX_HEIGHT 27

#define ID_ANI_KOOPAS_WALKING_LEFT 226
#define ID_ANI_KOOPAS_WALKING_RIGHT 227
#define ID_ANI_KOOPAS_DIE 228

#define KOOPAS_DIE_TIMEOUT 500

class CKoopas : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;

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
};

