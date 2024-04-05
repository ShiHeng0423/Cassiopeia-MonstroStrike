#pragma once
/*!************************************************************************
  \file					MapPlatformGenerate.h
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (90%)
  \secondary author		Goh Jun Jie (10%)
  \brief				  This file contains declarations of functions and structures essential for generating and
  managing moving platforms. The header defines structures for representing platform objects and enumerates different
  types of platforms, along with functions for creating, updating, and handling collisions with these platforms.


All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/
#include <AEEngine.h>
#include "CollisionShape.h"
#include <AEMath.h>
#include "Player.h"

enum PlatformTypes
{
	PLATFORM_NONE,
	HORIZONTAL_MOVING_PLATFORM = 1,
	VERTICAL_MOVING_PLATFORM = 2,
	DIAGONAL_PLATFORM = 3
};

//Temporary moving platform object
struct Platforms
{
	AEVec2 velocity; //Added for movement - Johny
	AEVec2 collisionNormal;
	AEVec2 size;
	AEVec2 position;

	AEVec2 endPoint;
	AEVec2 startPoint;

	AABB collisionBox;

	AEMtx33 scale;
	AEMtx33 rotation;
	AEMtx33 translation;
	AEMtx33 transformation;

	f32 speed;
};


void CreatePlatform(f32 xPos, f32 yPos, f32 xSize, f32 ySize, f32 speed, PlatformTypes typeOfPlatform, std::vector<struct Platforms>& platformVector);//Add end point and start point afterwards

void UpdatePlatforms(Player& player, std::vector<Enemy>& vecEnemy, std::vector<struct Platforms>& platformVector);

//For player vs moving object collision
void PlatformCollision(Platforms& movingObject, Player& player);

//Overloading, for enemy moving object collision
void PlatformCollision(Platforms& movingObject, Enemy& enemy);
