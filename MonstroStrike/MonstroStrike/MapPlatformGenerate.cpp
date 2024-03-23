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
		thePlatform.endPoint.x = thePlatform.startPoint.x - 200.f;
		//End Point Y Axis
		thePlatform.endPoint.y = thePlatform.position.y - 200.f;

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
	bool collided = false;
	//Vertical
	if (AABBvsAABB(player.boxHeadFeet, movingObject.collisionBox)) {
		player.collisionNormal = AABBNormalize(player.boxHeadFeet, movingObject.collisionBox);

		ResolveVerticalCollision(player.boxHeadFeet, movingObject.collisionBox, &player.collisionNormal, &player.obj.pos,
			&player.velocity, &player.onFloor, &player.gravityForce, &player.isFalling);

		collided = true;

	}

	//Check horizontal box (Left arm -> Right arm)
	if (AABBvsAABB(player.boxArms, movingObject.collisionBox)) {
		player.collisionNormal = AABBNormalize(player.boxArms, movingObject.collisionBox);

		ResolveHorizontalCollision(player.boxArms, movingObject.collisionBox, &player.collisionNormal, &player.obj.pos,
			&player.velocity);
		collided = true;
	}

	if (collided)
	{
		player.obj.pos.x += movingObject.velocity.x;
		player.obj.pos.y += movingObject.velocity.y;
	}
}

void PlatformCollision(Platforms& movingObject, Enemy& enemy)
{
	//Check vertical box (Head + Feet) 
	if (AABBvsAABB(enemy.boxHeadFeet, movingObject.collisionBox)) {

		bool eCollided = false;
		enemy.collisionNormal = AABBNormalize(enemy.boxHeadFeet, movingObject.collisionBox);

		ResolveVerticalCollision(enemy.boxHeadFeet, movingObject.collisionBox, &enemy.collisionNormal, &enemy.obj.pos,
			&enemy.velocity, &enemy.onFloor, &enemy.gravityForce, &enemy.isFalling);

		if (enemy.collisionNormal.y == 1) {
			eCollided = true;
		}
		if (eCollided) {
			enemy.obj.pos.x += movingObject.velocity.x;
			enemy.obj.pos.y += movingObject.velocity.y;
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