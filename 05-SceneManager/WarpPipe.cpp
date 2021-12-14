#include "WarpPipe.h"
#include "Textures.h"

void CWarpPipe::Render()
{
	RenderBoundingBox();
	
	if (green < 0) {
		CAnimations::GetInstance()->Get(B_PIPE_ANI_ID_BODY_LEFT)->Render(x, y);
		CAnimations::GetInstance()->Get(B_PIPE_ANI_ID_BODY_RIGHT)->Render(x + 16, y);
		for (int i = 1; i < length; i++) {
			CAnimations::GetInstance()->Get(B_PIPE_ANI_ID_TOP_LEFT)->Render(x, y + 16 * i);
			CAnimations::GetInstance()->Get(B_PIPE_ANI_ID_TOP_RIGHT)->Render(x + 16, y + 16 * i);
		}
	}
	else {
		CAnimations::GetInstance()->Get(PIPE_ANI_ID_TOP_LEFT)->Render(x, y);
		CAnimations::GetInstance()->Get(PIPE_ANI_ID_TOP_RIGHT)->Render(x + 16, y);
		for (int i = 1; i < length; i++) {
			CAnimations::GetInstance()->Get(PIPE_ANI_ID_BODY_LEFT)->Render(x, y + 16 * i);
			CAnimations::GetInstance()->Get(PIPE_ANI_ID_BODY_RIGHT)->Render(x + 16, y + 16 * i);
		}
	}
}

void CWarpPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float yy = (height - 16) / 2;
	l = x - (PIPE_BBOX_WIDTH - TILE_SIZE) / 2;
	t = y - height / 2 + yy;
	r = l + PIPE_BBOX_WIDTH;
	b = t + height;
}
void CWarpPipe::RenderBoundingBox()
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

	float xx = x - ((PIPE_BBOX_WIDTH - TILE_SIZE) / 2) + rect.right / 2;
	float yy = (height - TILE_SIZE) / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy + yy, bbox, nullptr, BBOX_ALPHA, rect.right, rect.bottom);
}