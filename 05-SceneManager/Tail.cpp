#include "Tail.h"
#include "Textures.h"
#include "debug.h"
#include "Mario.h"
#include "Goomba.h"
#include "QuestionBlock.h"
#include "Koopas.h"
#include "BreakableBrick.h"
#include "ButtonP.h"
#include "AssetIDs.h"
#include "Koopas.h"

void CTail::Render() {
	RenderBoundingBox();
}

void CTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MARIO_TAIL_WIDTH / 2;
	top = y - MARIO_TAIL_HEIGHT / 2 + 4;
	right = left + MARIO_TAIL_WIDTH;
	bottom = top + MARIO_TAIL_HEIGHT;
}

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (CMario::GetInstance()->level != MARIO_LEVEL_TAIL)
	{
		this->isDeleted = true;
		return;
	}
	else
	{
		nx = CMario::GetInstance()->nx;
		if (CMario::GetInstance()->nx == 1)
		{
			float temp_x = CMario::GetInstance()->x - 7;
			if (CMario::GetInstance()->IsAttack == false)
			{
				this->x = temp_x;
				float temp_y = CMario::GetInstance()->y;
				this->y = temp_y - MARIO_TAIL_HEIGHT + 7;
			}
			else
			{

				this->SetPosition(CMario::GetInstance()->x + (MARIO_BIG_BBOX_WIDTH / 2) + (TAIL_BBOX_WIDTH / 2) + 2, CMario::GetInstance()->y + TAIL_BBOX_HEIGHT);

				if (GetTickCount64() - CMario::GetInstance()->AttackTime >= RACOON_ATTACK_TIME)
				{
					CMario::GetInstance()->IsAttack = false;
					IsActive = false;
				}
			}
		}
		else if (CMario::GetInstance()->nx == -1)
		{
			if (CMario::GetInstance()->IsAttack == false)
			{
				this->x = CMario::GetInstance()->x + MARIO_BIG_BBOX_WIDTH - 5;
				float temp_y = CMario::GetInstance()->y;
				this->y = temp_y - MARIO_TAIL_HEIGHT + 7;
			}
			else
			{
				this->SetPosition((CMario::GetInstance()->x - MARIO_BIG_BBOX_WIDTH / 2 - TAIL_BBOX_WIDTH / 2) - 2, CMario::GetInstance()->y + TAIL_BBOX_HEIGHT);
				if (GetTickCount64() - CMario::GetInstance()->AttackTime >= RACOON_ATTACK_TIME)
				{
					CMario::GetInstance()->IsAttack = false;
					IsActive = false;
				}
			}
		}
	}

	for (int i = 0; i < coObjects->size(); i++) {
		if (CCollision::GetInstance()->CheckAABB(this, coObjects->at(i)) && CMario::GetInstance()->IsAttack == true)
		{
			if (dynamic_cast<CGoomba*>(coObjects->at(i)))
			{
				CGoomba* g = dynamic_cast<CGoomba*>(coObjects->at(i));
				g->SetState(GOOMBA_STATE_DIE);
			}
			else if (dynamic_cast<CQuestionBlock*>(coObjects->at(i))) {
				CQuestionBlock* qb = dynamic_cast<CQuestionBlock*>(coObjects->at(i));
				qb->SpawnItem(CMario::GetInstance()->getNx(), MARIO_LEVEL_TAIL);
			}
			else if (dynamic_cast<CKoopas*>(coObjects->at(i))) {
				CKoopas* koopas = dynamic_cast<CKoopas*>(coObjects->at(i));
				koopas->SetState(KOOPAS_STATE_SHELL_2);
				koopas->vy = -0.4f;
				koopas->ReviveTime = GetTickCount64();
			}
			else if (dynamic_cast<BreakableBrick*>(coObjects->at(i)))
				OnCollisionWithBreakableBrick(coObjects->at(i));
		}
	}
}

void CTail::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (CMario::GetInstance()->IsAttack == true)
	{
		if (dynamic_cast<CGoomba*>(e->obj)) {
			CGoomba* g = dynamic_cast<CGoomba*>(e->obj);
			g->SetState(GOOMBA_STATE_DIE);
		}
		else if (dynamic_cast<CKoopas*>(e->obj)) {
			CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
			koopas->SetState(KOOPAS_STATE_SHELL_2);
			koopas->vy = -0.4f;
			koopas->ReviveTime = GetTickCount64();
			
		}
		else if (dynamic_cast<CQuestionBlock*>(e->obj)) {
			CQuestionBlock* qb = dynamic_cast<CQuestionBlock*>(e->obj);
			qb->SpawnItem(CMario::GetInstance()->getNx(), MARIO_LEVEL_TAIL);
		}
	}

	if (!e->obj->IsBlocking()) return;
}

void CTail::OnNoCollision(DWORD dt)
{
	//DebugOut(L"No collision at all\n");
	x += vx * dt;
	y += vy * dt;
}
void CTail::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	CGame::GetInstance()->Draw(x - cx, y - cy + 4, bbox, &rect, BBOX_ALPHA);
}

void CTail::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TAIL_ATTACK:
		vx = (CMario::GetInstance()->getNx() > 0) ? TAIL_VELOCITY : -TAIL_VELOCITY;
		break;
	case TAIL_ACTIVE:
		vx = 0.0f;
		break;
	default:
		break;
	}
}

void CTail::OnCollisionWithBreakableBrick(LPGAMEOBJECT& obj)
{
	if (IsActive == true)
	{
		BreakableBrick* breakableBrick = dynamic_cast<BreakableBrick*>(obj);
		if (breakableBrick->hasItem == true)
		{
			breakableBrick->SetState(BRICK_STATE_COLLISION);
			ButtonP* brickitem = new ButtonP(breakableBrick->x, breakableBrick->y - 16);
			_PlayScene->objects.push_back(brickitem);
		}
		else
		{
			if (breakableBrick->isCollision == false)
				breakableBrick->SetState(BRICK_STATE_DIE);
		}
		IsActive = false;
	}
}
