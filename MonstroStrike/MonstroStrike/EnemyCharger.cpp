#include "Enemy.h"
#include "EnemyUtils.h"
#include "Player.h"
#include "AEEngine.h"
#include "Physics.h"

#include <iostream>


void ENEMY_CHARGER_Update(Enemy& enemy, struct Player& player)
{
	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);
	
	if (enemy.health <= 0)
	{
		enemy.isAlive = false;
	}
	//std::cout << "Enemy On Floor: " << enemy.onFloor << std::endl;
	//std::cout << "Charger Enemy vel y: " << enemy.velocity.y << std::endl;

	switch (enemy.enemyCurrent)
	{
	case ENEMY_IDLE:
		//std::cout << player.obj.pos.y << " " << enemy.obj.pos.y << "\n";
		//std::cout << areAligned(player.obj.pos, enemy.obj.pos) << "\n";
		//std::cout << enemy.onFloor << "\n";

		if (distanceFromPlayer <= enemy.lineOfSight && areAligned(player.obj.pos, enemy.obj.pos)) {
			enemy.enemyNext = ENEMY_TRANSITION;
			enemy.isCollision = false;
			enemy.timePassed = 0.0f;
			enemy.loop_idle = false;
		}
		else {
			enemy.enemyNext = ENEMY_IDLE;

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
	case ENEMY_TRANSITION:

		// Lock on
		if (enemy.target_position == ENEMY_DEFAULT) {
			if (enemy.obj.pos.x >= player.obj.pos.x) {
				enemy.target_position = ENEMY_LEFT;
			}
			else {
				enemy.target_position = ENEMY_RIGHT;
			}
		}

		enemy.timePassed += (f32)AEFrameRateControllerGetFrameTime();
		enemy.isShooting = true;

		if (enemy.timePassed >= 1.0f) {
			enemy.timePassed = 0.0f;
			enemy.enemyNext = ENEMY_ATTACK;
		}

		break;
	case ENEMY_ATTACK: 
		Attack_Charge(enemy, enemy.target_position, 200.f);	//the charge attack

		if (enemy.isCollision == true) {

			enemy.isCollision = false;
			enemy.isShooting = false;	//out of attacking mode
			enemy.speed = 80.f;			//return to normal speed after attack
			enemy.target_position = ENEMY_DEFAULT;
			enemy.enemyNext = ENEMY_IDLE;
		}
		break;
	default:
		break;
	}

	//for gravity
	enemy.obj.pos.y += enemy.velocity.y * (f32)AEFrameRateControllerGetFrameTime();

	enemy.enemyCurrent = enemy.enemyNext;
}