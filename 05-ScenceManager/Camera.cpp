#include "Camera.h"

CCamera::CCamera(float w, float h) : width(w), height(h), left(0), top(0) {}
CCamera::~CCamera() {}
void CCamera::SetPosition(float x, float y) {
	left = x;
	top = y;
}

void CCamera::GetPostion(float& l, float& t) {
	l = left; 
	t = top;
}

float CCamera::GetHeight() { return height; }
float CCamera::GetWidth() { return width; }



