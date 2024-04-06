/*!************************************************************************
  \file					Physics.h
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (100%)
  \brief				The file contains declaration of physics-related functions and utilities
                        including AABB normalization, dot product calculation,
                        and gravity application.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include <AEEngine.h>
#include "CollisionShape.h"

AEVec2 AABBNormalize(AABB firstBox, AABB secondBox);

f32 DotProduct(const AEVec2& first, const AEVec2& second);

void ApplyGravity(AEVec2* velocity, f32 mass, bool* onFloor, f32* gForce);
#endif // PHYSICS_H