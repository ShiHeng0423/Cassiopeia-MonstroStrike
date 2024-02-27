#include "Enemy.h"
#include "Player.h"
#include "AEEngine.h"
#include "Physics.h"

#include <iostream>




void ENEMY_FLY_Update(Enemy& enemy, struct Player& player)
{
	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);

	if (enemy.health <= 0)
	{
		enemy.isAlive = false;
	}

	switch (enemy.enemyCurrent)
	{
	case ENEMY_IDLE:
		//std::cout << player.obj.pos.y << " " << enemy.obj.pos.y << "\n";
		//std::cout << enemy.obj.pos.x << "\n";

		if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
			enemy.enemyNext = ENEMY_CHASE;
			enemy.loop_idle = false;
		}
		else {
			enemy.enemyNext = ENEMY_IDLE;
			
			//if (enemy.loop_idle) {
			//	MoveTowards(enemy, enemy.waypoint);
			//	MoveTowardsY(enemy, enemy.waypoint);

			//	if ((enemy.obj.pos.x >= enemy.waypoint.x - 2.0f) && (enemy.obj.pos.x <= enemy.waypoint.x + 2.0f)) {
			//		enemy.loop_idle = false;
			//	}
			//}
			//if ((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) {
			//	enemy.loop_idle = true;
			//}
			if (!((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) && !(enemy.loop_idle)) {
				MoveTowards(enemy, enemy.starting_position);
				MoveTowardsY(enemy, enemy.starting_position);
			}
		}
		break;
	case ENEMY_TRANSITION:




		enemy.timePassed += (f32)AEFrameRateControllerGetFrameTime();
		enemy.isShooting = true;

		if (enemy.timePassed >= 0.5f) {
			enemy.timePassed = 0.0f;
			enemy.enemyNext = ENEMY_CHASE;
		}

		break;
	case ENEMY_CHASE: 

		if (distanceFromPlayer <= enemy.shootingRange) {
			enemy.enemyNext = ENEMY_ATTACK;
		}
		else if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {

			MoveTowards(enemy, player.obj.pos);
			MoveTowardsY(enemy, player.obj.pos);



		}
		else {
			enemy.enemyNext = ENEMY_IDLE;
		}
		break;

		break;
	case ENEMY_ATTACK:
		//enemy.timePassed += (f32)AEFrameRateControllerGetFrameTime();

		if (distanceFromPlayer <= enemy.shootingRange) {
			enemy.isShooting = true;
			if(CanFire(enemy)) {
				AEVec2Set(&enemy.spawnPoint, enemy.obj.pos.x - 7.f, enemy.obj.pos.y - 15.f);	//customized spawn point for bullet
				SpawnBullet(enemy.spawnPoint, player.obj.pos, enemy.bullets);
			}
		}
		else {
			enemy.isShooting = false;
			enemy.enemyNext = ENEMY_CHASE;

		}



		break;
	default:
		break;
	}

	//for gravity
	//enemy.obj.pos.y += enemy.velocity.y * AEFrameRateControllerGetFrameTime();

	enemy.enemyCurrent = enemy.enemyNext;
	//main body collision box
	enemy.collisionBox.minimum.x = enemy.obj.pos.x - enemy.obj.img.scale.x * 0.5f;
	enemy.collisionBox.minimum.y = enemy.obj.pos.y - enemy.obj.img.scale.y * 0.5f;
	enemy.collisionBox.maximum.x = enemy.obj.pos.x + enemy.obj.img.scale.x * 0.5f;
	enemy.collisionBox.maximum.y = enemy.obj.pos.y + enemy.obj.img.scale.y * 0.5f;

	f32 verticalOffset = enemy.obj.img.scale.y * 0.01f;
	//Vertical
	enemy.boxHeadFeet = enemy.collisionBox; // Get original collision box size
	enemy.boxHeadFeet.minimum.y -= verticalOffset;
	enemy.boxHeadFeet.maximum.y += verticalOffset;

	f32 horizontalOffset = enemy.obj.img.scale.x * 0.01f;
	//Horizontal
	enemy.boxArms = enemy.collisionBox;
	enemy.boxArms.minimum.x -= horizontalOffset;
	enemy.boxArms.maximum.x += horizontalOffset;

}