#pragma once
#include "GameObject.h"
class CColorBlock : public CGameObject
{
private:
	float width, height;
	float cellWidth;
	float cellHeight;
	int length;
public:
	CColorBlock(float x, float y, float w, float h, float cw, float ch, int l) : CGameObject(x, y) {
		width = w;
		height = h;
		cellWidth = cw;
		cellHeight = ch;
		length = l;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	virtual void DirectBlocking(int& l, int& t, int& r, int& b) { l = 0; t = 1; b = 0; r = 0; }
};

