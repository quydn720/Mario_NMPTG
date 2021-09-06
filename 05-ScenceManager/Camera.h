#pragma once
#include "Game.h"

class CCamera
{
	float width, height;
	float left, top;
	int mapW, mapH;

public:
	CCamera(int mapW, int mapH, float w, float h);
	~CCamera();
	// Set and return the position of the camera
	void SetPosition(float& playerX, float& playerY);
	float GetWidth();
	float GetHeight();
	void GetPostion(float& l, float& t);

	void addx(int x) { left += x; }
	void addy(int y) { top += y; }
};

