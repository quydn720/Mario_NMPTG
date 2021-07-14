#pragma once
#include "QuestionBlock.h"
QuestionBlock::QuestionBlock(ItemType itemType, float width, float height) : Block(width, height) {
	blockType = BlockType::QUESTION_BLOCK;
	this->itemType = itemType;
	setState(ObjectState::QUESTION_BLOCK_FILLED);
	// TODO: nên viết các hàm với tham số thể hiện chi tiết mục đích hàm, lấy ani_set của một state
	//animationSets::GetInstance()->Get(ani_set_id)->Get(state)
}

ItemType QuestionBlock::getItemType()
{
	return itemType;
}

Item* QuestionBlock::getItem() {
	return item;
}

void QuestionBlock::setItem(Item* it) {
	item = it;
}

void QuestionBlock::setState(ObjectState s)
{
	switch (s) {
	case QUESTION_BLOCK_FILLED:
		setAnimation(CAnimationSets::GetInstance()->Get(2)->at(0));
		break;
	case ObjectState::QUESTION_BLOCK_EMPTY:
		isEmpty = true;
		setAnimation(CAnimationSets::GetInstance()->Get(2)->at(1));
		break;
	}

}

