/*!************************************************************************
  \file					MapPlatformGenerate.cpp
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (90%)
  \secondary author		Goh Jun Jie (10%, copy pasted player to moving platform collision for enemy)
  \brief				This file contains defintions of functions for generating and managing moving platforms.
						Includes functions for creating, updating, and handling collisions with these platforms.
						The creation of the initial position of the moving platforms will be based on the grid 
						in the csv file assigned to it.


All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/
#include "MapPlatformGenerate.h"
#include "CollisionShape.h"
#include "Physics.h"
#include <iostream>

void CreatePlatform(f32 xPos, f32 yPos, f32 xSize, f32 ySize, f32 speed, PlatformTypes typeOfPlatform, std::vector<struct Platforms>& platformVector)
{
	struct Platforms thePlatform;
	//Moving platform test, success
	AEVec2Zero(&thePlatform.collisionNormal);

	thePlatform.rotation = { 0 };
	AEMtx33Rot(&thePlatform.rotation, 0.f);

	AEVec2Set(&thePlatform.size, xSize, ySize);
	AEMtx33Scale(&thePlatform.scale, thePlatform.size.x, thePlatform.size.y);
	AEVec2Set(&thePlatform.position, xPos, yPos);
	AEMtx33Trans(&thePlatform.translation, thePlatform.position.x, thePlatform.position.y);

	thePlatform.transformation = { 0 };
	AEMtx33Concat(&thePlatform.transformation, &thePlatform.rotation, &thePlatform.scale);
	AEMtx33Concat(&thePlatform.transformation, &thePlatform.translation, &thePlatform.transformation);

	thePlatform.speed = speed;

	//Different behavior for different platforms
	switch (typeOfPlatform)
	{
	case HORIZONTAL_MOVING_PLATFORM:
		//Assign start point / beginning
		thePlatform.startPoint = thePlatform.position;

		//End Point X Axis
		thePlatform.endPoint.x = thePlatform.startPoint.x + 300.f;
		//End Point Y Axis
		thePlatform.endPoint.y = thePlatform.position.y;

		//Setting Velocity
		AEVec2Set(&thePlatform.velocity, thePlatform.endPoint.x - thePlatform.startPoint.x, thePlatform.endPoint.y - thePlatform.startPoint.y);

		AEVec2Normalize(&thePlatform.velocity, &thePlatform.velocity);
		AEVec2Scale(&thePlatform.velocity, &thePlatform.velocity, thePlatform.speed);
		break;
	case VERTICAL_MOVING_PLATFORM:
		//Assign start point / beginning
		thePlatform.startPoint = thePlatform.position;

		//End Point X Axis
		thePlatform.endPoint.x = thePlatform.startPoint.x;
		//End Point Y Axis
		thePlatform.endPoint.y = thePlatform.position.y - 300.f;

		//Setting Velocity
		AEVec2Set(&thePlatform.velocity, thePlatform.endPoint.x - thePlatform.startPoint.x, thePlatform.endPoint.y - thePlatform.startPoint.y);

		AEVec2Normalize(&thePlatform.velocity, &thePlatform.velocity);
		AEVec2Scale(&thePlatform.velocity, &thePlatform.velocity, thePlatform.speed);
		break;
	case DIAGONAL_PLATFORM:
		//Assign start point / beginning
		thePlatform.startPoint = thePlatform.position;

		//End Point X Axis
		thePlatform.endPoint.x = thePlatform.startPoint.x - 100.f;
		//End Point Y Axis
		thePlatform.endPoint.y = thePlatform.position.y - 400.f;

		//Setting Velocity
		AEVec2Set(&thePlatform.velocity, thePlatform.endPoint.x - thePlatform.startPoint.x, thePlatform.endPoint.y - thePlatform.startPoint.y);

		AEVec2Normalize(&thePlatform.velocity, &thePlatform.velocity);
		AEVec2Scale(&thePlatform.velocity, &thePlatform.velocity, thePlatform.speed);
		break;
	default:
		break;
	}

	platformVector.push_back(thePlatform);
}

void UpdatePlatforms(Player& player, std::vector<Enemy>& vecEnemy, std::vector<struct Platforms>& platformVector)
{
	for (int i = 0; i < platformVector.size(); i++)
	{
		// Update platform position based on velocity
		AEVec2Add(&platformVector[i].position, &platformVector[i].position, &platformVector[i].velocity);


		AEMtx33Trans(&platformVector[i].translation, platformVector[i].position.x, platformVector[i].position.y);
		AEMtx33Concat(&platformVector[i].transformation, &platformVector[i].rotation, &platformVector[i].scale);
		AEMtx33Concat(&platformVector[i].transformation, &platformVector[i].translation, &platformVector[i].transformation);

		//Loop between the start and end point
		if (AEVec2Distance(&platformVector[i].position, &platformVector[i].endPoint) <= 1.0f) {
			// Swap start and end points
			AEVec2 temp = platformVector[i].startPoint;
			platformVector[i].startPoint = platformVector[i].endPoint;
			platformVector[i].endPoint = temp;
			// Recalculate velocity direction
			AEVec2Set(&platformVector[i].velocity, platformVector[i].endPoint.x - platformVector[i].startPoint.x, platformVector[i].endPoint.y - platformVector[i].startPoint.y);
			AEVec2Normalize(&platformVector[i].velocity, &platformVector[i].velocity);
			AEVec2Scale(&platformVector[i].velocity, &platformVector[i].velocity, platformVector[i].speed);
		}

		//Check if player is on the platform
		PlatformCollision(platformVector[i], player);

		//Check if enemy/bullet is colliding platform
		for (Enemy& enemy : vecEnemy) {
			PlatformCollision(platformVector[i], enemy);
		}

		//Update collision box location
		platformVector[i].collisionBox.minimum.x = platformVector[i].position.x - platformVector[i].size.x * 0.5f;
		platformVector[i].collisionBox.minimum.y = platformVector[i].position.y - platformVector[i].size.y * 0.5f;
		platformVector[i].collisionBox.maximum.x = platformVector[i].position.x + platformVector[i].size.x * 0.5f;
		platformVector[i].collisionBox.maximum.y = platformVector[i].position.y + platformVector[i].size.y * 0.5f;
	}
}

void PlatformCollision(Platforms& movingObject, Player& player)
{
	//Vertical
	if (AABBvsAABB(player.GetPlayerBoxHeadFeet(), movingObject.collisionBox)) {
		player.GetPlayerCollisionNormal() = AABBNormalize(player.GetPlayerBoxHeadFeet(), movingObject.collisionBox);
		ResolveVerticalCollision(player.GetPlayerBoxHeadFeet(), movingObject.collisionBox, &player.GetPlayerCollisionNormal(), &player.GetPlayerCurrentPosition(),
			&player.GetPlayerVelocity());

		if (player.GetPlayerCollisionNormal().y == 1)
		{
			player.GetPlayerCurrentPosition().x += movingObject.velocity.x;
			player.GetPlayerCurrentPosition().y += movingObject.velocity.y;
		}
		else if (player.GetPlayerCollisionNormal().y == -1)
		{
			if (movingObject.velocity.y <= 0.f) //Only moving down
			{
				player.GetPlayerCurrentPosition().y += movingObject.velocity.y * 5.f;
			}
		}
	}

	//Check horizontal box (Left arm -> Right arm)
	if (AABBvsAABB(player.GetPlayerBoxArm(), movingObject.collisionBox)) {
		player.GetPlayerCollisionNormal() = AABBNormalize(player.GetPlayerBoxArm(), movingObject.collisionBox);

		ResolveHorizontalCollision(player.GetPlayerBoxArm(), movingObject.collisionBox, &player.GetPlayerCollisionNormal(), &player.GetPlayerCurrentPosition(),
			&player.GetPlayerVelocity());
	}

}

void PlatformCollision(Platforms& movingObject, Enemy& enemy)
{
	//Check vertical box (Head + Feet) 
	if (AABBvsAABB(enemy.boxHeadFeet, movingObject.collisionBox)) {

		enemy.collisionNormal = AABBNormalize(enemy.boxHeadFeet, movingObject.collisionBox);

		ResolveVerticalCollision(enemy.boxHeadFeet, movingObject.collisionBox, &enemy.collisionNormal, &enemy.obj.pos,
			&enemy.velocity);

		if (enemy.collisionNormal.y == 1) {
			enemy.obj.pos.x += movingObject.velocity.x;
			enemy.obj.pos.y += movingObject.velocity.y;
		}
		else if (enemy.collisionNormal.y == -1)
		{
			if (movingObject.velocity.y <= 0.f)
			{
				enemy.obj.pos.y += movingObject.velocity.y * 5.f;
			}
		}
	}
	//Check horizontal box (Left arm -> Right arm)
	if (AABBvsAABB(enemy.boxArms, movingObject.collisionBox)) {
		enemy.isCollision = true;
		enemy.collisionNormal = AABBNormalize(enemy.boxArms, movingObject.collisionBox);

		ResolveHorizontalCollision(enemy.boxArms, movingObject.collisionBox, &enemy.collisionNormal, &enemy.obj.pos,
			&enemy.velocity);
		enemy.loopIdle = false;

	}

	if (enemy.enemyType == ENEMY_FLY || enemy.enemyType == ENEMY_BOSS1) {
		for (Bullet& bullet : enemy.bullets) {
			if (AABBvsAABB(bullet.collisionBox, movingObject.collisionBox)) {
				bullet.lifeTime = 0; //makes bullet erase
			}
		}
	}

}