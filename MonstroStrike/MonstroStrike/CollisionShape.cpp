/*!************************************************************************
  \file					CollisionShape.cpp
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (100%)
  \brief				File containing definitions of collision detection between AABB boxes.

  This file contains the implementation of collision detection functions. The functions handle collisions between
  axis-aligned bounding boxes (AABBs). It also includes resolving of vertical and horizontal collision between grids and
  entities which prevent piercing through two AABB boxes should the entities should not.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/
#include "CollisionShape.h"
#include <AEMath.h>
#include <iostream>
#include <algorithm>

bool AABBvsAABB(AABB firstBox, AABB secondBox)
{
    const f32 epsilon = -0.001f;
    //Return false if there are no intersection found separated along axis
    if (firstBox.maximum.x + epsilon < secondBox.minimum.x || firstBox.minimum.x > secondBox.maximum.x + epsilon)
    {
        return false;
    }
    if (firstBox.maximum.y + epsilon < secondBox.minimum.y || firstBox.minimum.y > secondBox.maximum.y + epsilon)
    {
        return false;
    }

    //Means at least one overlapping axis
    return true;
}

//bool CirclevsCircle(Circle first, Circle second)
//{
//	float totalRadius = first.radius + second.radius;
//
//	totalRadius *= totalRadius;
//	
//	return totalRadius < (pow(first.position.x + second.position.x, 2) + pow(first.position.y + second.position.y, 2));
//}

void ResolveVerticalCollision(AABB& firstBoxHeadFeet, AABB& second, AEVec2* collisionNormal, AEVec2* position, AEVec2* velocity)
{
    f32 penetrationDepth = 0.f;

    if (collisionNormal->y == 1) // Colliding from entity bottom
    {
        penetrationDepth = second.maximum.y - firstBoxHeadFeet.minimum.y;
        if (penetrationDepth > 0) {
            f32 dampingFactor = 0.3f;
            penetrationDepth *= dampingFactor;
            position->y += penetrationDepth;
        }
    }
    else if (collisionNormal->y == -1) // Colliding from entity top
    {
        penetrationDepth = firstBoxHeadFeet.maximum.y - second.minimum.y;

        if (penetrationDepth > 0) {
            f32 dampingFactor = 1.f;
            penetrationDepth *= dampingFactor;
            position->y -= penetrationDepth;
        }
    }

    if (penetrationDepth > 0.f) {
        velocity->y = 0.f;
    }
}

void ResolveHorizontalCollision(AABB& firstArms, AABB& second, AEVec2* collisionNormal, AEVec2* position, AEVec2* velocity)
{
    f32 penetrationDepth = 0.f;

    if (collisionNormal->x == 1) // Colliding from right
    {
        penetrationDepth = second.maximum.x - firstArms.minimum.x;
        if (penetrationDepth > 0) {
            f32 dampingFactor = 0.3f; 
            position->x += penetrationDepth * dampingFactor; 
        }
    }
    else if (collisionNormal->x == -1) // Colliding from left
    {
        penetrationDepth = firstArms.maximum.x - second.minimum.x;
        if (penetrationDepth > 0) {
            f32 dampingFactor = 0.3f;

            position->x -= penetrationDepth * dampingFactor;
        }
    }

    if (penetrationDepth > 0.f) {
        velocity->x = 0.f;
    }

}