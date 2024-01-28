#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include <AEEngine.h>
#include "CollisionShape.h"

AEVec2 NormalizeValue(AABB firstBox, AABB secondBox);

f32 DotProduct(const AEVec2& first, const AEVec2& second);
AEVec2 CalculatePenetrationDepth(const AABB& first, const AABB& second, AEVec2& collisionNormal);

template <typename T1>
void ApplyGravity(T1& object)
{
	std::cout << "GRAVITY" << std::endl;
	AEVec2 gravityForce;
	AEVec2 gravity = { 0.f, -9.81f };
	AEVec2Scale(&gravityForce, &gravity, object.mass);
	AEVec2Scale(&object.velocity, &object.velocity, 0.98f);
	object.velocity.y += gravityForce.y * AEFrameRateControllerGetFrameTime();
}

template <typename T1, typename T2>
void PositionalCorrection(T1& first, T2& second, AEVec2& pD, AEVec2& collisionNormal)
{
	f32 percent = 0.2f;

	AEVec2 correction;
	AEVec2Scale(&correction, &pD, (percent / (first.inverseMass + second.inverseMass)));

	AEVec2 firstCorrection;
	AEVec2Scale(&firstCorrection, &correction, first.inverseMass);
	AEVec2Add(&first.position, &first.position, &firstCorrection);

	AEVec2 secondCorrection;
	AEVec2Scale(&secondCorrection, &correction, -second.inverseMass);
	AEVec2Add(&second.position, &second.position, &secondCorrection);
}

#endif // PHYSICS_H6