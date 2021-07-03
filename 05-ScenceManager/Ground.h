#pragma once
#include "GameObject.h"
class Ground : public CGameObject {
public:
	Ground(float width, float height);
	~Ground() {};
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};