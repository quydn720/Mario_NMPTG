#pragma once
#include "ColorBlock.h"
ColorBlock::ColorBlock(float width, float height) {
	blockType = COLOR_BLOCK;
	SetBoundingBox(width, height);
}

void ColorBlock::Render()
{
	RenderBoundingBox();
}

void ColorBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + widthBoundingBox;
	b = y + heightBoundingBox;
}
