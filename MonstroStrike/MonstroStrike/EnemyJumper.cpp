#include "Enemy.h"
#include "Player.h"
#include "AEEngine.h"
#include "Physics.h"

#include <iostream>


void ENEMY_JUMPER_Update(Enemy& enemy, struct Player& player)
{
	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);

	if (enemy.health <= 0)
	{
		enemy.isAlive = false;
	}
	std::cout << "Enemy On Floor: " << enemy.onFloor << std::endl;
	std::cout << "Slime Enemy vel y: " << enemy.velocity.y << std::endl;
	std::cout << "Slime Enemy g: " << enemy.gravityForce << std::endl;

	switch (enemy.enemyCurrent)
	{
	case ENEMY_IDLE:
		//std::cout << player.obj.pos.y << " " << enemy.obj.pos.y << "\n";
		//std::cout << enemy.obj.pos.x << "\n";

		if (distanceFromPlayer <= enemy.lineOfSight) {
			enemy.enemyNext = ENEMY_TRANSITION;
			enemy.isCollision = false;
			enemy.timePassed = 0.0f;
			enemy.loop_idle = false;
		}
		else {
			enemy.enemyNext = ENEMY_IDLE;
			enemy.timePassed += (f32)AEFrameRateControllerGetFrameTime();
			if (enemy.loop_idle) {


				if (enemy.timePassed >= 2.0f) {
					enemy.timePassed = 0.0f;
					if (enemy.onFloor) {
						Jump(enemy, 300.f);
					}
				}
				if (!enemy.onFloor) {
					MoveTowards(enemy, enemy.waypoint);
				}



				if ((enemy.obj.pos.x >= enemy.waypoint.x - 2.0f) && (enemy.obj.pos.x <= enemy.waypoint.x + 2.0f)) {
					enemy.loop_idle = false;
				}
			}
			if ((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) {
				enemy.loop_idle = true;
			}
			if (!((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) && !(enemy.loop_idle)) {

				if (enemy.timePassed >= 2.0f) {
					enemy.timePassed = 0.0f;
					if (enemy.onFloor) {
						Jump(enemy, 300.f);
					}
				}
				if (!enemy.onFloor) {
					MoveTowards(enemy, enemy.starting_position);
				}




			}
		}
		break;
	case ENEMY_TRANSITION:

		//Lock on
		if (enemy.target_position == ENEMY_DEFAULT) {	//finding which direction to charge towards
			if (enemy.obj.pos.x >= player.obj.pos.x) {
				enemy.target_position = ENEMY_LEFT;
			}
			if (enemy.obj.pos.x <= player.obj.pos.x) {
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
		enemy.timePassed += (f32)AEFrameRateControllerGetFrameTime();
		//Change to jump attack
		//Attack_Charge(enemy, enemy.target_position);	//the charge attack

		//if (enemy.isCollision == true) {

		//	enemy.isCollision = false;
		//	enemy.isShooting = false;	//out of attacking mode
		//	enemy.speed = 80.f;			//return to normal speed after attack
		//	enemy.target_position = ENEMY_DEFAULT;
		//	enemy.enemyNext = ENEMY_IDLE;
		//}


		if (distanceFromPlayer < enemy.lineOfSight) {

			if (enemy.timePassed >= 0.5f) {
				enemy.timePassed = 0.0f;
				if (enemy.onFloor) {
					Jump(enemy, 500.f);
				}
			}
			if (!enemy.onFloor) {
				MoveTowards(enemy, player.obj.pos);
			}
			
		}
		else {
			enemy.isShooting = false;
			enemy.timePassed = 0.f;
			enemy.enemyNext = ENEMY_IDLE;
			//std::cout << "Going back to IDLE!\n";
		}



		break;
	default:
		break;
	}

	//for gravity

	enemy.obj.pos.y += enemy.velocity.y * AEFrameRateControllerGetFrameTime();

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