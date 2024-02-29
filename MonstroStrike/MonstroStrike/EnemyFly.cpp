#include "Enemy.h"
#include "Player.h"
#include "AEEngine.h"
#include "Physics.h"

#include <iostream>




void ENEMY_FLY_Update(Enemy& enemy, struct Player& player)
{
	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);
	enemy.timePassed += (f32)AEFrameRateControllerGetFrameTime();	//time.time
	//std::cout << enemy.isCollision << "\n";
	if (enemy.isCollision) {
		enemy.isCollision = false;
	}

	if (enemy.health <= 0)
	{
		enemy.isAlive = false;
	}

	switch (enemy.enemyCurrent)
	{
	case ENEMY_IDLE:
		//std::cout << player.obj.pos.y << " " << enemy.obj.pos.y << "\n";
		//std::cout << enemy.obj.pos.x << "\n";

		if (distanceFromPlayer <= enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
			enemy.enemyNext = ENEMY_CHASE;
			enemy.loop_idle = false;
		}
		else {
			enemy.enemyNext = ENEMY_IDLE;
			if (!((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) && !(enemy.loop_idle)) {

				MoveTowardsFLY(enemy, enemy.starting_position);
				isStuck(enemy);
			}
		}
		break;
	case ENEMY_TRANSITION:


		enemy.isShooting = true;

		if (enemy.timePassed >= 0.5f) {
			enemy.timePassed = 0.0f;
			if (distanceFromPlayer > enemy.lineOfSight) {
				//Gives Up
				enemy.isShooting = false;	//turns red color off
				enemy.enemyNext = ENEMY_IDLE;
			}
			else {
				enemy.enemyNext = ENEMY_CHASE;		
			}

		}

		break;
	case ENEMY_CHASE: 
		enemy.isShooting = true;
		if (distanceFromPlayer <= enemy.shootingRange) {
			enemy.enemyNext = ENEMY_ATTACK;
		}
		else if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {


			MoveTowardsFLY(enemy, player.obj.pos);

			enemy.waypoint = player.obj.pos;	//set a waypoint a player's location
			isStuck(enemy);

		}
		else {
			//player runs out of line of sight range
			MoveTowardsFLY(enemy, enemy.waypoint);	//go to the last location player was at
			isStuck(enemy);
			//when it reaches the way point (manual typed numbers are offsets )
			if ((enemy.obj.pos.x >= enemy.waypoint.x - 10.f && enemy.obj.pos.x <= enemy.waypoint.x + 10.f)
				&& (enemy.obj.pos.y >= enemy.waypoint.y - 60.f && enemy.obj.pos.y <= enemy.waypoint.y + 60.f)){	//.y +- value depends on offset in MoveTowards fly

				enemy.timePassed = 0.0f;
				enemy.enemyNext = ENEMY_TRANSITION; //go to transition to pause there abit
			
			}

		}
		break;
	case ENEMY_ATTACK:

		enemy.isShooting = true;
		if (distanceFromPlayer <= enemy.shootingRange) {

			if(CanFire(enemy)) {
				AEVec2Set(&enemy.spawnPoint, enemy.obj.pos.x - 7.f, enemy.obj.pos.y - 15.f);	//customized spawn point for bullet
				SpawnBullet(enemy.spawnPoint, player.obj.pos, enemy.bullets);
			}
		}
		else {
			//player runs out of shooting range

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