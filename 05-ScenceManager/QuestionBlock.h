#pragma once
#include "Block.h"
#include "Item.h"
class QuestionBlock : public Block {
private:
	ItemType itemType;
	Item* item;
public:
	bool isEmpty = false;

	QuestionBlock(ItemType itemType, float w, float h);
	ItemType getItemType();
	Item* getItem();
	void setItem(Item* it);
	virtual void Render() {
		currentAnimation->Render(x, y);
	};
	void setObjectState(ObjectState s);
};