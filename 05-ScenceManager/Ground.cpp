#include "Ground.h"
Ground::Ground(float width, float height){
	blockType = GROUND;
	SetBoundingBox(width, height);
}

void Ground::Render(){
	RenderBoundingBox();
}
void Ground::GetBoundingBox(float& l, float& t, float& r, float& b){
	l = x;
	t = y;
	r = x + widthBoundingBox;
	b = y + heightBoundingBox;
}