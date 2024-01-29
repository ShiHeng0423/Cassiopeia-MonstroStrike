#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include <AEEngine.h>
#include "CollisionShape.h"

AEVec2 NormalizeValue(AABB firstBox, AABB secondBox);

f32 DotProduct(const AEVec2& first, const AEVec2& second);

template <typename T1>
void ApplyGravity(T1& object)
{
	AEVec2 gravityForce;
	AEVec2 gravity = { 0.f, -9.81f };
	AEVec2Scale(&gravityForce, &gravity, object.mass);
	AEVec2Scale(&object.velocity, &object.velocity, 0.98f); //Dampen
	object.velocity.y += gravityForce.y * AEFrameRateControllerGetFrameTime();
}

template <typename T1, typename T2>
void ResolveVerticalCollision(T1& first, T2& second, AEVec2* collisionNormal)
{
    float penetrationDepth = (collisionNormal->y == 1) ? second.collisionBox.maximum.y - first.collisionBox.minimum.y
        : second.collisionBox.minimum.y - first.collisionBox.maximum.y;

    if (collisionNormal->y == 1)
    {
        float penetrationDepth = second.collisionBox.maximum.y - first.collisionBox.minimum.y;
        first.position.y += penetrationDepth;
    }
    else if (collisionNormal->y == -1)
    {
        float penetrationDepth = second.collisionBox.minimum.y - first.collisionBox.maximum.y;
        first.position.y += penetrationDepth;
    }

    collisionNormal->y = 0;
}

template <typename T1, typename T2>
void ResolveHorizontalCollision(T1& first, T2& second, AEVec2* collisionNormal)
{
    float penetrationDepth = 0.0f;

    if (collisionNormal->x == 1)
    {
        penetrationDepth = second.collisionBox.maximum.x - first.collisionBox.minimum.x;
        first.position.x += penetrationDepth;
    }
    else if (collisionNormal->x == -1)
    {
        penetrationDepth = first.collisionBox.maximum.x - second.collisionBox.minimum.x;
        first.position.x -= penetrationDepth;
    }

    if (collisionNormal->x != 0)
    {
        first.velocity.x = 0.f;
    }

    collisionNormal->x = 0;
}
#endif // PHYSICS_H6