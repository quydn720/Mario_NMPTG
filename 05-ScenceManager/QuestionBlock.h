#pragma once
#include "Block.h"
#include "Item.h"
class QuestionBlock : public Block {
private:
	// 
	bool isEmpty = false;
	// Dont need a whole Item object
	ItemType itemType;
public:
	QuestionBlock(ItemType itemType, float w, float h);
	~QuestionBlock() {}
	virtual void Render() {
		//currentAnimation->Render(x, y);
		//RenderBoundingBox();
	}

	ItemType getItemType();
};