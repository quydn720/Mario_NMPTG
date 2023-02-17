#pragma once
#include "KeyEventHandler.h"

class WorldMapPlayer : public CKeyEventHandler
{
private: 
	float x, y;
	float vx, vy;
	bool isMoving;

public:
	float startY, startX;

	WorldMapPlayer();
	WorldMapPlayer(float x, float y) {
		this->x = x; this->y = y;  this->startY = y;
		vx = 0.0f; vy = 0.0f;
		isMoving = false;
	}

	float GetX() { return x; }
	float GetY() { return y; }

	bool isNotMoving() { return isMoving == false; }

	void GoUp() { vy = -0.1f; vx = 0; isMoving = true; }
	void GoDown() { vy = 0.1f; vx = 0; isMoving = true; }
	void GoLeft() {
		vx = -0.1f; vy = 0; isMoving = true; 
	}
	void GoRight() {
		vx = 0.1f; vy = 0; isMoving = true; 
	}
	void Stop();

	void Render();
	void Update(DWORD dt, int currentY, int currentX, int& moved);

	void OnKeyDown(int KeyCode);
	void KeyState(BYTE* state);
	void OnKeyUp(int KeyCode);
};

