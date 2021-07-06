#pragma once
#include "Block.h"
class Ground : public Block {
public:
	Ground(float width, float height);
	~Ground() {};
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};