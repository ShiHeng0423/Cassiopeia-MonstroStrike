#pragma once
#include <AEEngine.h>
#include <iostream>

struct AABB
{
	AEVec2 minimum;
	AEVec2 maximum;
};

struct Circle
{
	f32 radius;
	AEVec2 position;
};

//AABB vs AABB
bool AABBvsAABB(AABB firstBox, AABB secondBox);

//Circle vs Circle Collision
bool CirclevsCircle(Circle first, Circle second);