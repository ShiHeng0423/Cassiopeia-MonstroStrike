/*!************************************************************************
  \file					Physics.cpp
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (100%)
  \brief				Implementation of physics system-related functions
                        and utilities. Such as calculating the normal vector of collision between two axis-aligned bounding 
                        boxes (AABBs). Computing the dot product between two vectors. As well as
                        application of gravity to a given velocity vector, simulating downward acceleration due to gravity.

All content � 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/
#include "Physics.h"

AEVec2 AABBNormalize(AABB firstBox, AABB secondBox)
{
    AEVec2 result = { 0.f, 0.f };

    // Distance between 2 AABBs
    AEVec2 centerDist = {};
    centerDist.x = (firstBox.minimum.x + firstBox.maximum.x) * 0.5f - (secondBox.minimum.x + secondBox.maximum.x) * 0.5f;
    centerDist.y = (firstBox.minimum.y + firstBox.maximum.y) * 0.5f - (secondBox.minimum.y + secondBox.maximum.y) * 0.5f;

    AEVec2 halfExtents = {};
    halfExtents.x = (firstBox.maximum.x - firstBox.minimum.x) * 0.5f;
    halfExtents.y = (firstBox.maximum.y - firstBox.minimum.y) * 0.5f;

    //Overlap check
    AEVec2 overlap = {};
    //Use fabsf to get absolute value for float
    overlap.x = fabsf(centerDist.x) - (halfExtents.x + (secondBox.maximum.x - secondBox.minimum.x) * 0.5f);
    overlap.y = fabsf(centerDist.y) - (halfExtents.y + (secondBox.maximum.y - secondBox.minimum.y) * 0.5f);

    // Determine the normal direction based on the overlap
    if (overlap.x > 0 || overlap.y > 0) {
        //Corner collision detected, check which overlap is more than the other
        if (overlap.x > 0) {
            // Collision on the left side
            result.x = -1.0f;
        }
        else if (overlap.x < 0) {
            // Collision on the right side
            result.x = 1.0f;
        }

        if (overlap.y > 0) {
            // Collision on the bottom
            result.y = 1.0f;
        }
        else if (overlap.y < 0) {
            // Collision on the top
            result.y = -1.0f;
        }

    }
    else //No corner collision
    {
        if (overlap.x > overlap.y) //X overlap is higher than y 
        {
            if (centerDist.x < 0) 
            {
                result.x = -1.0f;
            }
            else 
            {
                result.x = 1.0f;
            }
        }
        else //Y overlap is higher than x
        {
            if (centerDist.y < 0) 
            {
                result.y = -1.0f;
            }
            else 
            {
                result.y = 1.0f;
            }
        }
    }

    return result; //Return normal value
}


f32 DotProduct(const AEVec2& first, const AEVec2& second)
{
	return first.x * second.x + first.y * second.y;
}

void ApplyGravity(AEVec2 *velocity, f32 mass, bool* onFloor, f32* gForce)
{
	AEVec2 gravityForce;
	AEVec2 gravity = { 0.f, -20.81f }; //The acceleration aka gravity
	AEVec2Scale(&gravityForce, &gravity, mass); //Force = Mass x Acceleration
	AEVec2Scale(velocity, velocity, 0.98f); //Dampen, maybe add terminal velocity if needed to
    *gForce = velocity->y;
    if (*gForce == 0.f) {
        *onFloor = true;
    }
    else
    {
        *onFloor = false;
    }
	velocity->y += gravityForce.y * (f32)AEFrameRateControllerGetFrameTime();
}