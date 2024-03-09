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
void ResolveVerticalCollision(AABB& firstBoxHeadFeet, AABB& second, AEVec2* collisionNormal, AEVec2* position, AEVec2* velocity, bool* onFloor, f32* gravityForce, bool* falling)
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
    else if (collisionNormal->y == -1) // Colliding from top
    {
        penetrationDepth = firstBoxHeadFeet.maximum.y - second.minimum.y;
        if (penetrationDepth > 0) {
            f32 dampingFactor = 1.f;
            penetrationDepth *= dampingFactor;
            position->y -= penetrationDepth;
            velocity->y = 0.f;
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


bool DynamicCollisionX(AABB grid, AEVec2 vel1, AABB others, AEVec2 vel2, AEVec2& collisionNormal, f32 collisionTime)
{
	// assign tFirst to 0 and tLast to the frame time
	float tFirst = 0, tLast = (float)(AEFrameRateControllerGetFrameTime());

	//calculate the relative velocity based on the two game object velocity
	AEVec2 relVelocity;
	AEVec2Set(&relVelocity, vel2.x - vel1.x, vel2.y - vel1.y);


	//checking on one dimension(x - axis).

	//others move leftwards while grid remains static
	if (relVelocity.x < 0)
	{
		//case 1 (others on left grid on right - no collision)
		if (grid.minimum.x > others.maximum.x)
		{
			return 0;
		}
		//case 4 (others on right grid on left - collision)
		if (grid.maximum.x < others.minimum.x)
		{
			float tFirstX = (grid.maximum.x - others.minimum.x) / relVelocity.x;
			collisionNormal.y = 0;
			//get the maximumimum tFirst value for first time of collision
			if (tFirst < tFirstX)
			{
				tFirst = tFirstX;
				collisionNormal.x = -1;
			}
		}
		if (grid.minimum.x < others.maximum.x)
		{
			float tLastX = (grid.minimum.x - others.maximum.x) / relVelocity.x;
			//get the minimumimum tLast value for the time of exiting collision
			tLast = min(tLast, tLastX);
		}
	}
	//others move rightwards while grid remains static
	else if (relVelocity.x > 0)
	{
		//case2 (others on left grid on right - collision)
		if (grid.minimum.x > others.maximum.x)
		{
			float tFirstX = (grid.minimum.x - others.maximum.x) / relVelocity.x;
			collisionNormal.y = 0;
			//get the maximumimum tFirst value for first time of collision
			if (tFirst < tFirstX)
			{
				tFirst = tFirstX;
				collisionNormal.x = 1;
			}
		}
		if (grid.maximum.x > others.minimum.x)
		{
			float tLastX = (grid.maximum.x - others.minimum.x) / relVelocity.x;
			//get the minimumimum tLast value for the time of exiting collision
			tLast = min(tLast, tLastX);
		}
		//case 3 (others on right grid on left - no collision)
		if (grid.maximum.x < others.minimum.x)
		{
			return 0;
		}
	}
	else
	{
		//case 5 (others is moving parallel to the y-axis)
		if (grid.maximum.x < others.minimum.x)
		{
			return 0;
		}
		else if (grid.minimum.x > others.maximum.x)
		{
			return 0;
		}
	}
	//case 6 (no intersection when tFirst is more than tLast)
	if (tFirst > tLast)
	{
		//no collision
		return 0;
	}

	//assign the calculated first time of collision to firstTimeOfCollision to return the value
	collisionTime = tFirst;
	return 1;
}

bool DynamicCollisionY(AABB grid, AEVec2 vel1, AABB others, AEVec2 vel2, AEVec2& collisionNormal, f32 collisionTime)
{
	// assign tFirst to 0 and tLast to the frame time
	float tFirst = 0, tLast = (float)(AEFrameRateControllerGetFrameTime());

	//calculate the relative velocity based on the two game object velocity
	AEVec2 relVelocity;
	AEVec2Set(&relVelocity, vel2.x - vel1.x, vel2.y - vel1.y);

	//others move downwards while grid remains static
	if (relVelocity.y < 0)
	{
		//case 1 (others on below grid on above - no collision)
		if (grid.minimum.y > others.maximum.y)
		{
			return 0;
		}
		//case 4 (others on above grid on below - collision)
		if (grid.maximum.y < others.minimum.y)
		{
			float tFirstY = (grid.maximum.y - others.minimum.y) / relVelocity.y;
			collisionNormal.x = 0;
			//get the maximumimum tFirst value for first time of collision
			if (tFirst < tFirstY)
			{
				tFirst = tFirstY;
				collisionNormal.y = -1;
			}
		}
		if (grid.minimum.y < others.maximum.y)
		{
			float tLastY = (grid.minimum.y - others.maximum.y) / relVelocity.y;
			//get the minimumimum tLast value for the time of exiting collision
			tLast = min(tLast, tLastY);
		}
	}

	//others move upwards while grid remains static
	else if (relVelocity.y > 0)
	{
		//case2 (others on below grid on above - collision)
		if (grid.minimum.y > others.maximum.y)
		{
			float tFirstY = (grid.minimum.y - others.maximum.y) / relVelocity.y;
			collisionNormal.x = 0;
			//get the maximumimum tFirst value for first time of collision
			if (tFirst < tFirstY)
			{
				tFirst = tFirstY;
				collisionNormal.y = 1;
			}
		}
		if (grid.maximum.y > others.minimum.y)
		{
			float tLastY = (grid.maximum.y - others.minimum.y) / relVelocity.y;
			//get the minimumimum tLast value for the time of exiting collision
			tLast = min(tLast, tLastY);
		}
		//case 3 (others on above grid on below - no collision)
		if (grid.maximum.y < others.minimum.y)
		{
			return 0;
		}
	}
	else
	{
		//case 5 (others is moving parallel to the x-axis)
		if (grid.maximum.y < others.minimum.y)
		{
			return 0;
		}
		else if (grid.minimum.y > others.maximum.y)
		{
			return 0;
		}
	}

	//case 6 (no intersection when tFirst is more than tLast)
	if (tFirst > tLast)
	{
		return 0;
	}

	//assign the calculated first time of collision to firstTimeOfCollision to return the value
	collisionTime = tFirst;
	return 1;
}
