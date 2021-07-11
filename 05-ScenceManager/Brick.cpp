#include "Brick.h"

CBrick::CBrick(float width, float height):Block(width, height)
{
	blockType = BlockType::BRICK;
	// Fix this to the format
	currentAnimation = animation_set->at(0);
}

void CBrick::Render()
{
	// only 1 animation/frame
	RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}