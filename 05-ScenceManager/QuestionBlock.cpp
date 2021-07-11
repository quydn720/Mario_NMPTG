#pragma once
#include "QuestionBlock.h"
QuestionBlock::QuestionBlock(ItemType itemType, float width, float height) : Block(width, height) {
	blockType = BlockType::QUESTION_BLOCK;
	this->itemType = itemType;
	setAnimation(CAnimationSets::GetInstance()->Get(2)->at(0));
	// TODO: nên viết các hàm với tham số thể hiện chi tiết mục đích hàm, lấy ani_set của một state
	//animationSets::GetInstance()->Get(ani_set_id)->Get(state)
}

ItemType QuestionBlock::getItemType()
{
	return itemType;
}

