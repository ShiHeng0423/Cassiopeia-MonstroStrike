#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include <AEEngine.h>
#include "CollisionShape.h"

AEVec2 AABBNormalize(AABB firstBox, AABB secondBox);

f32 DotProduct(const AEVec2& first, const AEVec2& second);

void ApplyGravity(AEVec2* velocity, f32 mass, bool* onFloor, f32* gForce);
#endif // PHYSICS_H