#pragma once
class CCamera
{
	float width, height;
	float left, top;

public:
	CCamera(float width,float height);
	~CCamera();
	void SetPosition(float x, float y);
	float GetWidth();
	float GetHeight();
	void GetPostion(float& l, float& t);
};

