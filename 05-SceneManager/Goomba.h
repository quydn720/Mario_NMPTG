#pragma once
#include "GameObject.h"
#include "FallDetector.h"

#define LEVEL_PARA_GOOMBA	1
#define LEVEL_GOOMBA	0

#define GOOMBA_GRAVITY 0.0007f
#define GOOMBA_WALKING_SPEED 0.05f
#define GOOMBA_JUMPING_SPEED 0.1f
#define GOOMBA_FLYING_SPEED 0.1f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define PARA_GOOMBA_BBOX_WIDTH 20
#define PARA_GOOMBA_BBOX_HEIGHT 24
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_JUMPING 101
#define GOOMBA_STATE_FLYING 102

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001
#define ID_ANI_PARA_GOOMBA_WALKING 5002

#define GOOMBA_WALK_DURATION 2500
#define GOOMBA_JUMP_DURATION 2000
#define GOOMBA_FLY_DURATION 1000



class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 
	int level;
	int step;

	ULONGLONG die_start;
	ULONGLONG timeEllapsed;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	int GetLevel() { return level; }
	void SetLevel(int l);
	CGoomba(float x, float y, int level = 0);
	virtual void SetState(int state);
	void CalcGoombaMove();
};