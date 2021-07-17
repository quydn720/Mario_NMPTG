#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Mario.h"
#include "Game.h"
#include "Goomba.h"
#include "Portal.h"
#include "Ground.h"
#include "ColorBlock.h"
#include "QuestionBlock.h"
#include "Item.h"
#include "Coin.h"

Mario* Mario::_instance = NULL;
Mario* Mario::GetInstance()
{
	if (_instance == NULL) {
		_instance = new Mario();
	}
	return _instance;
}

Mario::Mario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	setObjectState(ObjectState::MARIO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

	marioState["onGround"] = false;
}

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);


	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	// test - block mario from edge -
	if (x < 1) {
		x = 1;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (_state != ObjectState::MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

			LPCOLLISIONEVENT e = coEventsResult[i];
			//DebugOut(L"\n[ON_GROUND]: %d", marioState["onGround"]);
			//DebugOut(L"\n[OBJ]: %d", dynamic_cast<Block*>(e->obj)->getBlockType());

			if (dynamic_cast<Block*>(e->obj)) {
				switch (dynamic_cast<Block*>(e->obj)->getBlockType()) {
				case BlockType::GROUND:
				case BlockType::BRICK: {
					marioState["onGround"] = true;
					Ground* ground = dynamic_cast<Ground*>(e->obj);
					if (e->ny > 0) {
						vy = 0;
					}
					if (e->nx != 0) {
						vx = 0;
					}
					break;
				}
				// Mario can go through color block horizontally
				case BlockType::COLOR_BLOCK: {
					marioState["onGround"] = true;
					ColorBlock* colorBlock = dynamic_cast<ColorBlock*>(e->obj);
					if (e->nx != 0) {
						x += dx;
					}
					break;
				}
				case BlockType::QUESTION_BLOCK: {
					marioState["onGround"] = true;
					QuestionBlock* questionBlock = dynamic_cast<QuestionBlock*>(e->obj);
					if (e->ny > 0) {
						if (questionBlock->getItemType() == ItemType::COIN && !questionBlock->isEmpty) {
							questionBlock->setObjectState(ObjectState::QUESTION_BLOCK_EMPTY);
							Coin* c = dynamic_cast<Coin*>(questionBlock->getItem());
							c->setObjectState(ObjectState::ITEM_VISIBLE);
						}
						if (questionBlock->getItemType() == ItemType::SUPER_ITEM && !questionBlock->isEmpty) {
							questionBlock->setObjectState(ObjectState::QUESTION_BLOCK_EMPTY);
							SuperItem* s = dynamic_cast<SuperItem*>(questionBlock->getItem());
							s->setObjectState(ObjectState::ITEM_VISIBLE, this->level, this->x);
						}
					}
					break;
				}
				}
			}
			//if (dynamic_cast<Item*>(e->obj)) {
			//	switch (dynamic_cast<Item*>(e->obj)->getItemType()) {
			//		//case ItemType::COIN: {
			//		//	Coin* coin = dynamic_cast<Coin*>(e->obj);
			//		//	coin->y += dy;
			//		//	//testing -> idea: khởi tạo coin nằm yên trong ?brick, khi mario cụng, thì câu lệnh này làm đồng tiền nảy lên, sau đó biến mất
			//		//	DebugOut(L"\n[COIN]: %s", "+1000000000000");
			//		//	break;
			//		//}
			//	}
			if (dynamic_cast<SuperItem*>(e->obj)) {
				e->obj->isAlive = false;
			}
			if (dynamic_cast<CGoomba*>(e->obj)) {// if e->obj is Goomba 
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->getObjectState() != ObjectState::GOOMBA_STATE_DIE)
					{
						goomba->setObjectState(ObjectState::GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->getObjectState() != ObjectState::GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								setObjectState(ObjectState::MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Mario::Render()
{
	int ani = -1;
	if (_state == ObjectState::MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else if (_state == ObjectState::MARIO_TAIL_STATE_IDLE)
		ani = MARIO_TAIL_ANI_IDLE_RIGHT;
	else
		if (level == MARIO_LEVEL_BIG)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_TAIL_ANI_IDLE_RIGHT;
				else ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_BIG_WALKING_RIGHT;
			else ani = MARIO_ANI_BIG_WALKING_LEFT;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				else ani = MARIO_ANI_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			else ani = MARIO_ANI_SMALL_WALKING_LEFT;
		}


	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->Get(ani)->Render(x, y, alpha);

	//currentAnimation->Render(x, y);
	RenderBoundingBox();
}

void Mario::setObjectState(ObjectState state)
{
	CGameObject::setObjectState(state);
	LPANIMATION ani = NULL;
	switch (state)
	{
	case ObjectState::MARIO_STATE_WALKING_RIGHT:
		ani = CAnimationSets::GetInstance()->Get(0)->Get(500);
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case ObjectState::MARIO_STATE_WALKING_LEFT:
		ani = CAnimationSets::GetInstance()->Get(0)->Get(501);
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case ObjectState::MARIO_STATE_JUMP:
		if (marioState["onGround"]) {
			vy = -MARIO_JUMP_SPEED_Y;
			marioState["onGround"] = false;
		}
		break;
	case ObjectState::MARIO_STATE_IDLE:
		ani = CAnimationSets::GetInstance()->Get(0)->Get(1);
		vx = 0;
		break;
	case ObjectState::MARIO_STATE_DIE:
		ani = CAnimationSets::GetInstance()->Get(0)->Get(599);
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
	setAnimation(ani);
}

void Mario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (level == MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_TAIL) {
		right = x + 20;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	//right += currentAnimation->getBBWidth();
	//bottom += currentAnimation->getBBHeight();
}

/*
	Reset Mario status to the beginning state of a scene
*/
void Mario::Reset()
{
	setObjectState(ObjectState::MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_SMALL);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

