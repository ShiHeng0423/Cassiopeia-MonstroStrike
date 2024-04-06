/*!************************************************************************
  \file					CollisionShape.h
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (100%)
  \brief				File containing declarations of collision detection between AABB boxes.

  This file contains the implementation of collision detection functions. The functions handle collisions between 
  axis-aligned bounding boxes (AABBs). It also includes resolving of vertical and horizontal collision between grids and
  entities which prevent piercing through two AABB boxes should the entities should not.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

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
void ResolveVerticalCollision(AABB& firstBoxHeadFeet, AABB& second, AEVec2* collisionNormal, AEVec2* position, AEVec2* velocity);

//Entity box head, second item / grid's collision box, collisionNormal, entity position, entity velocity, bool CanJump from entity
void ResolveHorizontalCollision(AABB& firstArms, AABB& second, AEVec2* collisionNormal, AEVec2* position, AEVec2* velocity);