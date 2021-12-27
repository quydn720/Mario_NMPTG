#include "Tail.h"
#include "Textures.h"
#include "debug.h"
#include "Mario.h"
#include "Goomba.h"
#include "QuestionBlock.h"
#include "Koopas.h"

void CTail::Render() {
	RenderBoundingBox();
}

void CTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - TAIL_LENGTH / 2;
	top = y - TAIL_HEIGHT / 2 + 4;
	right = left + TAIL_LENGTH;
	bottom = top + TAIL_HEIGHT;
}

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (int i = 0; i < coObjects->size(); i++) {
		if (CCollision::GetInstance()->CheckAABB(this, coObjects->at(i)))
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
				CKoopas* qb = dynamic_cast<CKoopas*>(coObjects->at(i));
				qb->SetState(KOOPAS_STATE_SHELL);
			}
		}
	}
}

void CTail::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (state == TAIL_ATTACK) {
		if (dynamic_cast<CGoomba*>(e->obj)) {
			CGoomba* g = dynamic_cast<CGoomba*>(e->obj);
			g->SetState(GOOMBA_STATE_DIE);
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
