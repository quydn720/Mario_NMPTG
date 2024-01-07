#include "WorldMapPlayer.h"
#include "Animations.h"
#include "debug.h"
#include "Collision.h"

#define TILE_SIZE 16
#define WORLD_MAP_PLAYER_MOVING_ANI_ID -27927

WorldMapPlayer::WorldMapPlayer()
{
	x = 64; y = 64; startX = 0; startY = 0;
	vx = 0.0f; vy = 0.0f;
	isMoving = false;
}

void WorldMapPlayer::Render()
{
	CAnimations::GetInstance()->Get(WORLD_MAP_PLAYER_MOVING_ANI_ID)->Render(x, y);
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
	//x = round(x); y = round(y);
}


void WorldMapPlayer::Update(DWORD dt, int currentY, int currentX, int& moved)
{
	DebugOut(L"x: %0.2f, y: %0.2f\n", x, y);
	x += vx * dt;
	y += vy * dt;

	float distance = moved * TILE_SIZE;

	if (vx > 0) {
		if (x > (distance + startX)) {
			x = distance + startX + 0.5f;
			Stop();
			moved = 0;
		}
	}
	if (vx < 0) {
		if (x < startX - (distance)) {
			x = startX - distance - 0.5f;
			Stop();
			moved = 0;
		}
	}
	if (vy > 0) {
		if (y > startY + (distance)) {
			y = startY + distance;
			Stop();
			moved = 0;
		}
	}
	if (vy < 0) {
		if (y < startY - distance) {
			y = startY - distance;
			Stop();
			moved = 0;
		}
	}
}
