#include "Enemy.h"
#include "Player.h"
#include "AEEngine.h"


#include <iostream>



void ENEMY_BOSS_Update(Enemy& enemy, struct Player& player)
{
	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);
	if (enemy.health <= 0)
	{
		enemy.isAlive = false;
	}

	if (enemy.enemyType == ENEMY_BOSS1) {
		switch (enemy.enemyCurrent)
		{
		case ENEMY_IDLE:
			if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
				enemy.enemyNext = ENEMY_CHASE;
				enemy.loop_idle = false;
				//std::cout << "HELLO CHASING\n";
			}
			else {
				enemy.enemyNext = ENEMY_IDLE;
				//long condition to stop jittering
				//loop_idle is a bool that works as a checker to start the "patrolling mode" for enemy, instead of standing still idling
				if (enemy.loop_idle) {
					MoveTowards(enemy.obj.pos, enemy.waypoint, enemy.speed);
					if ((enemy.obj.pos.x >= enemy.waypoint.x - 2.0f) && (enemy.obj.pos.x <= enemy.waypoint.x + 2.0f)) {
						enemy.loop_idle = false;
					}
				}
				if ((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) {
					enemy.loop_idle = true;
				}
				if (!((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) && !(enemy.loop_idle)) {

					MoveTowards(enemy.obj.pos, enemy.starting_position, enemy.speed);
				}
				//std::cout << enemy.obj.pos.x << "HELLO IDLE\n";
			}
			break;

		case ENEMY_CHASE:

			if (distanceFromPlayer <= enemy.shootingRange) {
				enemy.enemyNext = ENEMY_SHOOT;
			}
			else if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
				enemy.enemyNext = ENEMY_CHASE;
				MoveTowards(enemy.obj.pos, player.obj.pos, enemy.speed);
				//std::cout << "HELLO CHASING HERE!\n";

			}
			else {
				enemy.enemyNext = ENEMY_IDLE;
				//std::cout << "HELLO Going back to IDLE!\n";
			}
			break;

		case ENEMY_SHOOT:
			if (distanceFromPlayer <= enemy.shootingRange) {
				enemy.isShooting = true;
				if (CanFire(enemy)) {
					//std::cout << "HELLO Shooting!\n";
				}

			}
			if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
				enemy.isShooting = false;
				enemy.enemyNext = ENEMY_CHASE;
				//std::cout << "Shooting!\n";
			}
			break;
		default:
			break;
		}
		enemy.enemyCurrent = enemy.enemyNext;
		enemy.collisionBox.minimum.x = enemy.obj.pos.x - enemy.obj.img.scale.x * 0.5f;
		enemy.collisionBox.minimum.y = enemy.obj.pos.y - enemy.obj.img.scale.y * 0.5f;
		enemy.collisionBox.maximum.x = enemy.obj.pos.x + enemy.obj.img.scale.x * 0.5f;
		enemy.collisionBox.maximum.y = enemy.obj.pos.y + enemy.obj.img.scale.y * 0.5f;
	}//if boss1 AI
}
//void ENEMY_BOSSWING1_Update(Enemy& enemy, struct Player& player) {
//
//	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);
//	if (enemy.health <= 0)
//	{
//		enemy.isAlive = false;
//	}
//	if (enemy.enemyType == ENEMY_BOSS1_WING1) {
//		if (AEInputCheckTriggered(AEVK_X)) {
//			enemy.isAlive = false;
//			return;
//		}
//		switch (enemy.enemyCurrent)
//		{
//		case ENEMY_IDLE:
//			if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
//				enemy.enemyNext = ENEMY_CHASE;
//				enemy.loop_idle = false;
//				//std::cout << "HELLO CHASING\n";
//			}
//			else {
//				enemy.enemyNext = ENEMY_IDLE;
//				//long condition to stop jittering
//				//loop_idle is a bool that works as a checker to start the "patrolling mode" for enemy, instead of standing still idling
//				if (enemy.loop_idle) {
//					MoveTowards(enemy.obj.pos, enemy.waypoint, enemy.speed);
//					if ((enemy.obj.pos.x >= enemy.waypoint.x - 2.0f) && (enemy.obj.pos.x <= enemy.waypoint.x + 2.0f)) {
//						enemy.loop_idle = false;
//					}
//				}
//				if ((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) {
//					enemy.loop_idle = true;
//				}
//				if (!((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) && !(enemy.loop_idle)) {
//
//					MoveTowards(enemy.obj.pos, enemy.starting_position, enemy.speed);
//				}
//				//std::cout << enemy.obj.pos.x << "HELLO IDLE\n";
//			}
//			break;
//
//		case ENEMY_CHASE:
//
//			if (distanceFromPlayer <= enemy.shootingRange) {
//				enemy.enemyNext = ENEMY_SHOOT;
//			}
//			else if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
//				enemy.enemyNext = ENEMY_CHASE;
//				MoveTowards(enemy.obj.pos, player.obj.pos, enemy.speed);
//				//std::cout << "HELLO CHASING HERE!\n";
//
//			}
//			else {
//				enemy.enemyNext = ENEMY_IDLE;
//				//std::cout << "HELLO Going back to IDLE!\n";
//			}
//			break;
//
//		case ENEMY_SHOOT:
//			if (distanceFromPlayer <= enemy.shootingRange) {
//				if (CanFire(enemy)) {
//
//				}
//				enemy.enemyNext = ENEMY_SHOOT;
//			}
//			if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
//				enemy.enemyNext = ENEMY_CHASE;
//				//std::cout << "Shooting!\n";
//			}
//			break;
//		default:
//			break;
//		}
//		enemy.enemyCurrent = enemy.enemyNext;
//		enemy.collisionBox.minimum.x = enemy.obj.pos.x - enemy.obj.img.scale.x * 0.5f;
//		enemy.collisionBox.minimum.y = enemy.obj.pos.y - enemy.obj.img.scale.y * 0.5f;
//		enemy.collisionBox.maximum.x = enemy.obj.pos.x + enemy.obj.img.scale.x * 0.5f;
//		enemy.collisionBox.maximum.y = enemy.obj.pos.y + enemy.obj.img.scale.y * 0.5f;
//
//	}
//}
//
//
//void ENEMY_BOSSWING2_Update(Enemy& enemy, struct Player& player) {
//	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);
//	if (enemy.health <= 0)
//	{
//		enemy.isAlive = false;
//	}
//	if (enemy.enemyType == ENEMY_BOSS1_WING2) {
//		if (AEInputCheckTriggered(AEVK_Z)) {
//			enemy.isAlive = false;
//			return;
//		}
//		switch (enemy.enemyCurrent)
//		{
//		case ENEMY_IDLE:
//			if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
//				enemy.enemyNext = ENEMY_CHASE;
//				enemy.loop_idle = false;
//				//std::cout << "HELLO CHASING\n";
//			}
//			else {
//				enemy.enemyNext = ENEMY_IDLE;
//				//long condition to stop jittering
//				//loop_idle is a bool that works as a checker to start the "patrolling mode" for enemy, instead of standing still idling
//				if (enemy.loop_idle) {
//					MoveTowards(enemy.obj.pos, enemy.waypoint, enemy.speed);
//					if ((enemy.obj.pos.x >= enemy.waypoint.x - 2.0f) && (enemy.obj.pos.x <= enemy.waypoint.x + 2.0f)) {
//						enemy.loop_idle = false;
//					}
//				}
//				if ((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) {
//					enemy.loop_idle = true;
//				}
//				if (!((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) && !(enemy.loop_idle)) {
//
//					MoveTowards(enemy.obj.pos, enemy.starting_position, enemy.speed);
//				}
//				//std::cout << enemy.obj.pos.x << "HELLO IDLE\n";
//			}
//			break;
//
//		case ENEMY_CHASE:
//
//			if (distanceFromPlayer <= enemy.shootingRange) {
//				enemy.enemyNext = ENEMY_SHOOT;
//			}
//			else if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
//				enemy.enemyNext = ENEMY_CHASE;
//				MoveTowards(enemy.obj.pos, player.obj.pos, enemy.speed);
//				//std::cout << "HELLO CHASING HERE!\n";
//
//			}
//			else {
//				enemy.enemyNext = ENEMY_IDLE;
//				//std::cout << "HELLO Going back to IDLE!\n";
//			}
//			break;
//
//		case ENEMY_SHOOT:
//			if (distanceFromPlayer <= enemy.shootingRange) {
//				enemy.isShooting = true;
//				if (CanFire(enemy)) {
//					//std::cout << "HELLO Shooting!\n";
//				}
//				enemy.enemyNext = ENEMY_SHOOT;
//			}
//			if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
//				enemy.isShooting = false;
//				enemy.enemyNext = ENEMY_CHASE;
//				//std::cout << "Shooting!\n";
//			}
//			break;
//		default:
//			break;
//		}
//		enemy.enemyCurrent = enemy.enemyNext;
//		enemy.collisionBox.minimum.x = enemy.obj.pos.x - enemy.obj.img.scale.x * 0.5f;
//		enemy.collisionBox.minimum.y = enemy.obj.pos.y - enemy.obj.img.scale.y * 0.5f;
//		enemy.collisionBox.maximum.x = enemy.obj.pos.x + enemy.obj.img.scale.x * 0.5f;
//		enemy.collisionBox.maximum.y = enemy.obj.pos.y + enemy.obj.img.scale.y * 0.5f;
//
//	}
//
//
//}
