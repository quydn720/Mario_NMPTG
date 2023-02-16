#include "WorldMapPlayer.h"
#include "Animations.h"
#include "debug.h"
#include "Collision.h"

void WorldMapPlayer::Render()
{
	CAnimations::GetInstance()->Get(-28127 + 200)->Render(x, y);
}

void WorldMapPlayer::OnKeyDown(int KeyCode)
{
	
}

void WorldMapPlayer::KeyState(BYTE* state)
{
}

void WorldMapPlayer::OnKeyUp(int KeyCode)
{
}
void WorldMapPlayer::Stop()
{
	vx = 0; vy = 0; isMoving = false; startX = x; startY = y;
	x = round(x); y = round(y);
}


void WorldMapPlayer::Update(DWORD dt, int currentY, int currentX, int& moved)
{

	x += vx * dt;
	y += vy * dt;

	if (vx > 0) {
		if (x > ((moved) * 16 + startX)) {
			Stop();
			moved = 0;
		}
	}
	if (vx < 0) {
		if (x < startX - ((moved) * 16)) {
			Stop();
			moved = 0;
		}
	}
	if (vy > 0) {
		if (y > startY + ((moved) * 16)) {
			Stop();
			moved = 0;
	}
	}
	if (vy < 0) {
		if (y < startY - ((moved) * 16)) {
			Stop();
			moved = 0;
		}
	}
}
