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
	vx = 0; vy = 0; isMoving = false; mx = x; my = y;
	x = round(x); y = round(y);
}


void WorldMapPlayer::Update(DWORD dt, int currentY, int currentX, int& moved)
{
	//DebugOut(L"my: %f, \n", my);

	x += vx * dt;
	y += vy * dt;

	//DebugOut(L"%f\t\n", y);// 64 - (currentX * 16));
	
	if (vx > 0) {
		if (x > ((currentY - 1) * 16 + 64)) {
			Stop();
			moved = 0;
		}
	}
	if (vx < 0) {
		if (x < ((currentY - 1) * 16 + 64)) {
			Stop();
			moved = 0;
		}
	}
	if (vy > 0) {
		//if (y > 64 - currentX * 15) {

		/*if (y > my - (moved * 15)) {
			Stop();
			moved = 0;
		}*/
	}
	if (vy < 0) {
		if (y < abs(my - (moved) * 16)) {
			Stop();
			moved = 0;
			my = abs((moved) * 15);
		}
	}
	

	/*CCollision* collision = new CCollision();
	collision->CheckAABB()*/
	/*float Left, Top, Right, Bottom, left, top, right, bottom;
	Left = x;
	Top = y;
	Right = Left + 14;
	Bottom = Top + 16;
	
	
	left = 112;
	top = 64;
	right = left + 16;
	bottom = top + 16;

	bool colide = (!(Left > right || Top > bottom || Right < left || Bottom < top));*/

	/*if (x < 62) {
		x = 62; 
		isMoving = false; vx = 0; vy = 0;
	}

	if (x > 94) {
		x = 94;
		isMoving = false; vx = 0; vy = 0; 
	}*/

	//DebugOut(L"%0.2f\t%0.2f\n", vx, vy);
}
