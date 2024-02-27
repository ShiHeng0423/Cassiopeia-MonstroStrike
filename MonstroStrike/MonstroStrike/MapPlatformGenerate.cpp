#include "MapPlatformGenerate.h"
#include "CollisionShape.h"
#include "Physics.h"
#include <iostream>

void CreatePlatform(f32 xPos, f32 yPos, f32 xSize, f32 ySize, f32 speed, PlatformTypes typeOfPlatform, Platforms& thePlatform)
{
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
		thePlatform.endPoint.y = thePlatform.position.y - 100.f;

		//Setting Velocity
		AEVec2Set(&thePlatform.velocity, thePlatform.endPoint.x - thePlatform.startPoint.x, thePlatform.endPoint.y - thePlatform.startPoint.y);

		AEVec2Normalize(&thePlatform.velocity, &thePlatform.velocity);
		AEVec2Scale(&thePlatform.velocity, &thePlatform.velocity, thePlatform.speed);
		break;
	case DIAGONAL_PLATFORM:
		//Assign start point / beginning
		thePlatform.startPoint = thePlatform.position;

		//End Point X Axis
		thePlatform.endPoint.x = thePlatform.startPoint.x + 200.f;
		//End Point Y Axis
		thePlatform.endPoint.y = thePlatform.position.y - 100.f;

		//Setting Velocity
		AEVec2Set(&thePlatform.velocity, thePlatform.endPoint.x - thePlatform.startPoint.x, thePlatform.endPoint.y - thePlatform.startPoint.y);

		AEVec2Normalize(&thePlatform.velocity, &thePlatform.velocity);
		AEVec2Scale(&thePlatform.velocity, &thePlatform.velocity, thePlatform.speed);
		break;
	default:
		break;
	}
}

void UpdatePlatforms(Platforms* movingObject, int numberOfPlatforms, Player& player, std::vector<Enemy>& vecEnemy)
{
	for (int i = 0; i < numberOfPlatforms; i++)
	{
		// Update platform position based on velocity
		AEVec2Add(&movingObject[i].position, &movingObject[i].position, &movingObject[i].velocity);


		AEMtx33Trans(&movingObject[i].translation, movingObject[i].position.x, movingObject[i].position.y);
		AEMtx33Concat(&movingObject[i].transformation, &movingObject[i].rotation, &movingObject[i].scale);
		AEMtx33Concat(&movingObject[i].transformation, &movingObject[i].translation, &movingObject[i].transformation);

		//Loop between the start and end point
		if (AEVec2Distance(&movingObject[i].position, &movingObject[i].endPoint) <= 1.0f) {
			// Swap start and end points
			AEVec2 temp = movingObject[i].startPoint;
			movingObject[i].startPoint = movingObject[i].endPoint;
			movingObject[i].endPoint = temp;
			// Recalculate velocity direction
			AEVec2Set(&movingObject[i].velocity, movingObject[i].endPoint.x - movingObject[i].startPoint.x, movingObject[i].endPoint.y - movingObject[i].startPoint.y);
			AEVec2Normalize(&movingObject[i].velocity, &movingObject[i].velocity);
			AEVec2Scale(&movingObject[i].velocity, &movingObject[i].velocity, movingObject[i].speed);
		}

		//Check if player is on the platform
		PlayerOnPlatform(movingObject[i], player);

		//Check if enemy/bullet is colliding platform
		for (Enemy& enemy : vecEnemy) {
			PlatformCollision(movingObject[i], enemy);
		}

		//Update collision box location
		movingObject[i].collisionBox.minimum.x = movingObject[i].position.x - movingObject[i].size.x * 0.5f;
		movingObject[i].collisionBox.minimum.y = movingObject[i].position.y - movingObject[i].size.y * 0.5f;
		movingObject[i].collisionBox.maximum.x = movingObject[i].position.x + movingObject[i].size.x * 0.5f;
		movingObject[i].collisionBox.maximum.y = movingObject[i].position.y + movingObject[i].size.y * 0.5f;
	}
}

void PlayerOnPlatform(Platforms& movingObject, Player& player)
{
	//Check horizontal box (Left arm -> Right arm)
	if (AABBvsAABB(player.boxArms, movingObject.collisionBox)) {
		player.collisionNormal = AABBNormalize(player.boxArms, movingObject.collisionBox);

		ResolveHorizontalCollision(player.boxArms, movingObject.collisionBox, &player.collisionNormal, &player.obj.pos,
			&player.velocity, &player.onFloor);
	}

	//Vertical
	if (AABBvsAABB(player.boxHeadFeet, movingObject.collisionBox)) {
		player.collisionNormal = AABBNormalize(player.boxHeadFeet, movingObject.collisionBox);

		ResolveVerticalCollision(player.boxHeadFeet, movingObject.collisionBox, &player.collisionNormal, &player.obj.pos,
			&player.velocity, &player.onFloor);

		if (player.collisionNormal.y == 1) //Player on top
		{
			player.obj.pos.x += movingObject.velocity.x;
			player.obj.pos.y += movingObject.velocity.y;
		}
	}
}

void PlatformCollision(Platforms& movingObject, Enemy& enemy)
{
	//Check vertical box (Head + Feet) 
	if (AABBvsAABB(enemy.boxHeadFeet, movingObject.collisionBox)) {

		enemy.collisionNormal = AABBNormalize(enemy.boxHeadFeet, movingObject.collisionBox);

		ResolveVerticalCollision(enemy.boxHeadFeet, movingObject.collisionBox, &enemy.collisionNormal, &enemy.obj.pos,
			&enemy.velocity, &enemy.onFloor);

		if (enemy.collisionNormal.y == 1) //Enemy on top
		{
			enemy.obj.pos.x += movingObject.velocity.x;
			enemy.obj.pos.y += movingObject.velocity.y;
		}

	}
	//Check horizontal box (Left arm -> Right arm)
	if (AABBvsAABB(enemy.boxArms, movingObject.collisionBox)) {
		enemy.isCollision = true;
		enemy.collisionNormal = AABBNormalize(enemy.boxArms, movingObject.collisionBox);

		ResolveHorizontalCollision(enemy.boxArms, movingObject.collisionBox, &enemy.collisionNormal, &enemy.obj.pos,
			&enemy.velocity, &enemy.onFloor);
		enemy.loop_idle = false;
	}

	for (Bullet& bullet : enemy.bullets) {
		if (AABBvsAABB(bullet.collisionBox, movingObject.collisionBox)) {
			bullet.lifetime = 0.f; //makes bullet erase

		}
	}

}