#pragma once
#include "GameObject.h"

#define PIPE_ANI_ID 10000 // TODO: change the ani
#define PIPE_BBOX_WIDTH		32

class CWarpPipe : public CGameObject
{
	float height;
public:
	CWarpPipe(float x, float y, float h) : CGameObject(x, y) { height = h; }
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
};

