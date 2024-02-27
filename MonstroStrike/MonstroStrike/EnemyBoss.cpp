#include "Enemy.h"
#include "Player.h"
#include "AEEngine.h"


#include <iostream>



void ENEMY_BOSS_Update(Enemy& enemy, struct Player& player)
{
	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);

//health check
	if (enemy.health <= 0)
	{
		enemy.isAlive = false;
	}
	if (enemy.wing1.health <= 0)
	{
		enemy.wing1.isAlive = false;
	}
	if (enemy.wing2.health <= 0)
	{
		enemy.wing2.isAlive = false;
	}



















	switch (enemy.enemyCurrent)
	{
	case ENEMY_IDLE:
		if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
			enemy.enemyNext = ENEMY_CHASE;
			enemy.loop_idle = false;
		}
		else {
			enemy.enemyNext = ENEMY_IDLE;
			//long condition to stop jittering
			//loop_idle is a bool that works as a checker to start the "patrolling mode" for enemy, instead of standing still idling
			if (enemy.loop_idle) {
				MoveTowards(enemy, enemy.waypoint);

				if ((enemy.obj.pos.x >= enemy.waypoint.x - 2.0f) && (enemy.obj.pos.x <= enemy.waypoint.x + 2.0f)) {
					enemy.loop_idle = false;
				}
			}
			if ((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) {
				enemy.loop_idle = true;
			}
			if (!((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) && !(enemy.loop_idle)) {
				MoveTowards(enemy, enemy.starting_position);
			}
		}
		break;

	case ENEMY_CHASE:

		if (distanceFromPlayer <= enemy.shootingRange) {
			enemy.enemyNext = ENEMY_ATTACK;
		}
		else if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
			enemy.enemyNext = ENEMY_CHASE;
			MoveTowards(enemy, player.obj.pos);



		}
		else {
			enemy.enemyNext = ENEMY_IDLE;
		}
		break;

	case ENEMY_ATTACK:
		if (distanceFromPlayer <= enemy.shootingRange) {
			enemy.isShooting = true;
			//if(CanFire(enemy)) {
			//	SpawnBullet(enemy.obj.pos, player.obj.pos, enemy.bullets);
			//}
			if(CanPartFire(enemy.wing1) && enemy.wing1.isAlive) {;
				SpawnBullet(enemy.wing1.obj.pos, player.obj.pos, enemy.bullets);

			}
			if (CanPartFire(enemy.wing2) && enemy.wing2.isAlive) {
				SpawnBullet(enemy.wing2.obj.pos, player.obj.pos, enemy.bullets);
			}
		}

		if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
			enemy.isShooting = false;
			enemy.enemyNext = ENEMY_CHASE;
		}
		break;
	default:
		break;
	}




	//set to next state
	enemy.enemyCurrent = enemy.enemyNext;

	//for gravity
	enemy.obj.pos.y += enemy.velocity.y * AEFrameRateControllerGetFrameTime();


	//wings collision box
	if (enemy.wing1.isAlive) {
		enemy.wing1.obj.pos.x = enemy.obj.pos.x + enemy.wing1.Offset;
		enemy.wing1.obj.pos.y = enemy.obj.pos.y + 10.f;

		enemy.wing1.collisionBox.minimum.x = enemy.wing1.obj.pos.x - enemy.wing1.obj.img.scale.x * 0.5f;
		enemy.wing1.collisionBox.minimum.y = enemy.wing1.obj.pos.y - enemy.wing1.obj.img.scale.y * 0.5f;
		enemy.wing1.collisionBox.maximum.x = enemy.wing1.obj.pos.x + enemy.wing1.obj.img.scale.x * 0.5f;
		enemy.wing1.collisionBox.maximum.y = enemy.wing1.obj.pos.y + enemy.wing1.obj.img.scale.y * 0.5f;
	}
	if (enemy.wing2.isAlive) {

		enemy.wing2.obj.pos.x = enemy.obj.pos.x + enemy.wing2.Offset;
		enemy.wing2.obj.pos.y = enemy.obj.pos.y + 10.f;

		enemy.wing2.collisionBox.minimum.x = enemy.wing2.obj.pos.x - enemy.wing2.obj.img.scale.x * 0.5f;
		enemy.wing2.collisionBox.minimum.y = enemy.wing2.obj.pos.y - enemy.wing2.obj.img.scale.y * 0.5f;
		enemy.wing2.collisionBox.maximum.x = enemy.wing2.obj.pos.x + enemy.wing2.obj.img.scale.x * 0.5f;
		enemy.wing2.collisionBox.maximum.y = enemy.wing2.obj.pos.y + enemy.wing2.obj.img.scale.y * 0.5f;
	}
	
	//main body collision box
	enemy.collisionBox.minimum.x = enemy.obj.pos.x - enemy.obj.img.scale.x * 0.5f;	//changing this to 0.25 will make the bullet glitch
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
