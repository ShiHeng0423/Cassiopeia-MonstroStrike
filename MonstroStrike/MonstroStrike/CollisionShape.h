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
//bool CirclevsCircle(Circle first, Circle second);

//Entity box head, second item / grid's collision box, collisionNormal, entity position, entity velocity, bool CanJump from entity
void ResolveVerticalCollision(AABB& firstBoxHeadFeet, AABB& second, AEVec2* collisionNormal, AEVec2* position, AEVec2* velocity, bool* onFloor, f32* gravityForce, bool* falling);

//Entity box head, second item / grid's collision box, collisionNormal, entity position, entity velocity, bool CanJump from entity
void ResolveHorizontalCollision(AABB& firstArms, AABB& second, AEVec2* collisionNormal, AEVec2* position, AEVec2* velocity);


bool DynamicCollisionX(AABB grid, AEVec2 vel1, AABB others, AEVec2 vel2, AEVec2& collisionNormal, f32 collisionTime);
bool DynamicCollisionY(AABB grid, AEVec2 vel1, AABB others, AEVec2 vel2, AEVec2& collisionNormal, f32 collisionTime);