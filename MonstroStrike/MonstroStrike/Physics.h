#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include <AEEngine.h>
#include "CollisionShape.h"

AEVec2 AABBNormalize(AABB firstBox, AABB secondBox);

f32 DotProduct(const AEVec2& first, const AEVec2& second);

//Uses template such that any entity could work as parameter
//!!!! But the entity must have the exact data (.mass, .velocity etc)
template <typename T1>
void ApplyGravity(T1& object)
{
	AEVec2 gravityForce;
	AEVec2 gravity = { 0.f, -9.81f }; //The acceleration aka gravity
	AEVec2Scale(&gravityForce, &gravity, object.mass); //Force = Mass x Acceleration
	AEVec2Scale(&object.velocity, &object.velocity, 0.98f); //Dampen, maybe add terminal velocity if needed to
	object.velocity.y += gravityForce.y * AEFrameRateControllerGetFrameTime();
}

//Uses template such that any entity could work as parameter
//!!!! But the entity must have the exact data (.position, .collisionBox etc)
template <typename T1, typename T2>
void ResolveVerticalCollision(T1& first, T2& second, AEVec2* collisionNormal)
{
    f32 penetrationDepth = 0.f;

    if (collisionNormal->y == 1)
    {
        penetrationDepth = second.collisionBox.maximum.y - first.boxHeadFeet.minimum.y;
        first.position.y += penetrationDepth;
        first.canJump = true;
    }
    else if (collisionNormal->y == -1)
    {
        penetrationDepth = second.collisionBox.minimum.y - first.boxHeadFeet.maximum.y;
        first.position.y += penetrationDepth;
    }

    if (collisionNormal->y != 0)
    {
        first.velocity.y = 0.f;
    }
    collisionNormal->y = 0;
}

//Uses template such that any entity could work as parameter
//!!!! But the entity must have the exact data (.position, .collisionBox etc)
template <typename T1, typename T2>
void ResolveHorizontalCollision(T1& first, T2& second, AEVec2* collisionNormal)
{
    float penetrationDepth = 0.f;

    if (collisionNormal->x == 1)
    {
        penetrationDepth = second.collisionBox.maximum.x - first.arms.minimum.x;
        first.position.x += penetrationDepth;
        first.velocity.x = 0.f;
    }
    else if (collisionNormal->x == -1)
    {
        penetrationDepth = first.arms.maximum.x - second.collisionBox.minimum.x;
        first.position.x -= penetrationDepth;
        first.velocity.x = 0.f;
    }
    collisionNormal->x = 0;
}
#endif // PHYSICS_H