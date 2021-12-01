#pragma once
#include "GameObject.h"

#define PIPE_ANI_ID 1011 // TODO: change the ani
#define PIPE_ANI_ID_TOP_RIGHT 1012 // TODO: change the ani
#define PIPE_ANI_ID_BODY_LEFT 1013 // TODO: change the ani
#define PIPE_ANI_ID_BODY_RIGHT 1014 // TODO: change the ani
#define PIPE_BBOX_WIDTH		32

#define TILE_SIZE 16

class CWarpPipe : public CGameObject
{
	float height;
	int length;
public:
	CWarpPipe(float x, float y, float h) : CGameObject(x, y) { height = h; length = (int) height / TILE_SIZE; }
	void Render();
	void Update(DWORD dt) {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void RenderBoundingBox();
};

