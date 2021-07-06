#pragma once
#include "GameObject.h"

enum BlockType {
	UNDEFINED = -1,
	COLOR_BLOCK,
	GROUND,
	BRICK
};

class Block : public CGameObject
{
protected:
	BlockType blockType = UNDEFINED;
public:
	Block(float width, float height);
	Block();
	~Block();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	BlockType getBlockType() { return blockType; }
};