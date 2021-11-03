#include "ColorBlock.h"
#include "Textures.h"

void CColorBlock::Render()
{
	//RenderBoundingBox();
}

void CColorBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	// have not get it ... but this variable will put the block with height > 16 at the right place,
	// also with the render bounding box above
	float cellHeight_div_2 = this->cellHeight / 2;
	float yy = (cellHeight - 16) / 2;

	l = x - cellWidth_div_2;
	t = y - cellHeight_div_2 + yy;
	r = l + this->cellWidth * this->length;
	b = t + this->cellHeight;
}

void CColorBlock::RenderBoundingBox() {
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

	float xx = x - this->cellWidth / 2 + rect.right / 2;
	float yy = (this->cellHeight - 16) / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy + yy, bbox, nullptr, BBOX_ALPHA, rect.right, rect.bottom);
}