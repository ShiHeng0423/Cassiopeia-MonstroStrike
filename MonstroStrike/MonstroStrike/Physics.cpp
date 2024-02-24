#include "Physics.h"

AEVec2 AABBNormalize(AABB firstBox, AABB secondBox)
{
    AEVec2 result = { 0.f, 0.f };

    // Distance between 2 AABBs
    AEVec2 centerDist;
    centerDist.x = (firstBox.minimum.x + firstBox.maximum.x) * 0.5f - (secondBox.minimum.x + secondBox.maximum.x) * 0.5f;
    centerDist.y = (firstBox.minimum.y + firstBox.maximum.y) * 0.5f - (secondBox.minimum.y + secondBox.maximum.y) * 0.5f;

    AEVec2 halfExtents;
    halfExtents.x = (firstBox.maximum.x - firstBox.minimum.x) * 0.5f;
    halfExtents.y = (firstBox.maximum.y - firstBox.minimum.y) * 0.5f;

    //Overlap check
    AEVec2 overlap;
    //Use fabsf to get absolute value for float
    overlap.x = fabsf(centerDist.x) - (halfExtents.x + (secondBox.maximum.x - secondBox.minimum.x) * 0.5f);
    overlap.y = fabsf(centerDist.y) - (halfExtents.y + (secondBox.maximum.y - secondBox.minimum.y) * 0.5f);

    // Determine the normal direction based on the overlap
    if (overlap.x > 0 && overlap.y > 0) {
        //Corner collision detected, check which overlap is more than the other
        if (overlap.x > overlap.y) //More overlap at X
        {
            if (centerDist.x < 0) {
                result.x = -1.0f;
            }
            else {
                result.x = 1.0f;
            }
        }
        else //More overlap at Y 
        {
            if (centerDist.y < 0) {
                result.y = -1.0f;
            }
            else {
                result.y = 1.0f;
            }
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

void ApplyGravity(AEVec2 *velocity, f32 mass)
{
	AEVec2 gravityForce;
	AEVec2 gravity = { 0.f, -9.81f }; //The acceleration aka gravity
	AEVec2Scale(&gravityForce, &gravity, mass); //Force = Mass x Acceleration
	AEVec2Scale(velocity, velocity, 0.98f); //Dampen, maybe add terminal velocity if needed to
	velocity->y += gravityForce.y * AEFrameRateControllerGetFrameTime();
}