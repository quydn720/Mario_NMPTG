#pragma once
#include "GameObject.h"

#define PIPE_ANI_ID_TOP_LEFT 1011
#define PIPE_ANI_ID_TOP_RIGHT 1012 
#define PIPE_ANI_ID_BODY_LEFT 1013 
#define PIPE_ANI_ID_BODY_RIGHT 1014
#define PIPE_BBOX_WIDTH		32
#define PIPE_BBOX_DOWN_RANGE		16

#define B_PIPE_ANI_ID_BODY_LEFT 1021
#define B_PIPE_ANI_ID_BODY_RIGHT 1022
#define B_PIPE_ANI_ID_TOP_LEFT 1023
#define B_PIPE_ANI_ID_TOP_RIGHT 1024

#define TILE_SIZE 16

class CWarpPipe : public CGameObject
{
	float height;
	int length;
	int sceneId; // -1: nothing, sceneId
	int green;
public:
	CWarpPipe(float x, float y, float h, int sId, int green) : CGameObject(x, y) {
		this->green = green;
		sceneId = sId;
		height = h;
		length = (int)height / TILE_SIZE;
	}
	int canGoThroughtScene() { return sceneId; }
	void Render();
	void Update(DWORD dt) {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void RenderBoundingBox();
};

