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

//Entity box head, second item / grid's collision box, collisionNormal, entity position, entity velocity, bool CanJump from entity
void ResolveVerticalCollision(AABB& firstBoxHeadFeet, AABB& second, AEVec2* collisionNormal, AEVec2* position, AEVec2* velocity, bool* onFloor)
{
    f32 penetrationDepth = 0.f;
    //std::cout << "Resolving vertical collision" << std::endl;
    //std::cout << "Normal: " << collisionNormal->y << std::endl;

    if (collisionNormal->y == 1)
    {
        penetrationDepth = second.maximum.y - firstBoxHeadFeet.minimum.y;
        position->y += penetrationDepth;
        *onFloor = true;
    }
    else if (collisionNormal->y == -1)
    {
        penetrationDepth = second.minimum.y - firstBoxHeadFeet.maximum.y;
        position->y += penetrationDepth;
    }

    if (collisionNormal->y != 0)
    {
        velocity->y = 0.f;
    }

}

//Entity arms, second item / grid's collision box, collisionNormal, entity position, entity velocity, bool CanJump from entity
void ResolveHorizontalCollision(AABB& firstArms, AABB& second, AEVec2* collisionNormal, AEVec2* position, AEVec2* velocity, bool* onFloor)
{
    f32 penetrationDepth = 0.f;
    //std::cout << "Resolving Horizontal " << collisionNormal->x << std::endl;

    if (collisionNormal->x == 1)
    {
        penetrationDepth = second.maximum.x - firstArms.minimum.x;
        position->x += penetrationDepth;
        velocity->x = 0.f;
    }
    else if (collisionNormal->x == -1)
    {
        penetrationDepth = second.minimum.x - firstArms.maximum.x;
        position->x += penetrationDepth;
        velocity->x = 0.f;
    }
    collisionNormal->x = 0;
}