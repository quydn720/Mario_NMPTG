#include "Camera.h"
#define HUD_SIZE 40

CCamera::CCamera() {
	mapW = 0;
	mapH = 0;
	width = 0;
	height = 0;
	left = top = 0; //TODO size = screen size, top left theo mario
}

CCamera::CCamera(int mw, int mh, float w, float h) {
	mapW = mw;
	mapH = mh;
	width = w;
	height = h;
	left = top = 0; //TODO size = screen size, top left theo mario
}
CCamera::~CCamera() {}

void CCamera::SetPosition(float& playerX, float& playerY) {
	left = playerX;
	top = playerY;

	// Khi mario o vung 1: [255 -> 255++], camY khong di chuyen theo playerY,
	// Scene size = width * 201
	// 300 = 255 + 45 -> 45 la [height / 4 ] khoang cach giua Y va canh tren camera, dua mario o vi tri gan giua camera
	// 215 actual screen height ?
	float floatedY = height + HUD_SIZE + height / 4 + 30;
	if (playerY > floatedY) {
		top = playerY;
	}
	else {
		top = playerY - floatedY / 4;
	}
	CGame* game = CGame::GetInstance();
	left = left - game->GetBackBufferWidth() / 2;
	// Bound of the map
	if (left < 0) left = 0;
	if (top < 0) top = 0;
	if (left > mapW - game->GetBackBufferWidth()) left = mapW - game->GetBackBufferWidth() * 1.0f;
	if (top > floatedY) {
		top = mapH - game->GetBackBufferHeight() + HUD_SIZE * 1.0f - 8;
	}

	playerX = left;
	playerY = top;
}

void CCamera::GetPostion(float& l, float& t) {
	l = left;
	t = top;
}


