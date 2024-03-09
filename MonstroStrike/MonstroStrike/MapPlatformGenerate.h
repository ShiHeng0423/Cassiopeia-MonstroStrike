#pragma once
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

void PlatformCollision(Platforms& movingObject, Player& player);

void PlatformCollision(Platforms& movingObject, Enemy& enemy);
