#include "Brick.h"

Brick::Brick(float width, float height) : Block(width, height) {
	blockType = BlockType::BRICK;
	// Fix this to the format
	currentAnimation = animation_set->at(0);
}

void Brick::Render() {
	RenderBoundingBox();
}
