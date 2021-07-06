#pragma once
#include "Block.h"

class ColorBlock : public Block
{
public:
	ColorBlock(float width, float height);
	~ColorBlock();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};