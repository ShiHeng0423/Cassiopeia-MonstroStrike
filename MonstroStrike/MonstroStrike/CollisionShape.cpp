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
void ResolveVerticalCollision(AABB& firstBoxHeadFeet, AABB& second, AEVec2* collisionNormal, AEVec2* position, AEVec2* velocity, bool* onFloor)
{
    f32 penetrationDepth = 0.f;

    if (collisionNormal->y == 1) // Colliding from bottom
    {
        penetrationDepth = second.maximum.y - firstBoxHeadFeet.minimum.y;
        if (penetrationDepth > 0) {
            position->y += penetrationDepth;
            *onFloor = true;
        }
    }
    else if (collisionNormal->y == -1) // Colliding from top
    {
        penetrationDepth = firstBoxHeadFeet.maximum.y - second.minimum.y;
        if (penetrationDepth > 0) {
            position->y -= penetrationDepth;
        }
    }

    if (penetrationDepth > 0) {
        velocity->y = 0.f;
    }
}

void ResolveHorizontalCollision(AABB& firstArms, AABB& second, AEVec2* collisionNormal, AEVec2* position, AEVec2* velocity, bool* onFloor)
{
    f32 penetrationDepth = 0.f;

    if (collisionNormal->x == 1) // Colliding from right
    {
        penetrationDepth = second.maximum.x - firstArms.minimum.x;
        if (penetrationDepth > 0) {
            position->x += penetrationDepth;
            velocity->x = 0.f;
        }
    }
    else if (collisionNormal->x == -1) // Colliding from left
    {
        penetrationDepth = firstArms.maximum.x - second.minimum.x;
        if (penetrationDepth > 0) {
            position->x -= penetrationDepth;
            velocity->x = 0.f;
        }
    }
    if (penetrationDepth > 0) {
        collisionNormal->x = 0; // Reset collision normal
    }
}
