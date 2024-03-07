#include "Enemy.h"
#include "Player.h"
#include "AEEngine.h"


#include <iostream>


void ENEMY_BOSS_Update(Enemy& enemy, struct Player& player)
{
	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);
	AEVec2 Spawnloc;

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

	if (enemy.wing1.isAlive == false && enemy.wing2.isAlive == false) {
		enemy.isFlying = false;
	}

	if (AEInputCheckCurr(AEVK_M)) {
		enemy.health--;
	}

	if (AEInputCheckCurr(AEVK_L)) {
		enemy.wing1.isAlive = false;
		enemy.wing2.isAlive = false;
	}


	switch (enemy.enemyCurrent)
	{
	case ENEMY_IDLE:
		if (enemy.health != 100) {
			enemy.timePassed = 0.f;
			enemy.enemyNext = ENEMY_TRANSITION;

		}
		break;
	case ENEMY_TRANSITION:
		enemy.timePassed += (f32)AEFrameRateControllerGetFrameTime();
		enemy.wing1.isAlive = true;
		enemy.wing2.isAlive = true;
		enemy.isFlying = true;
		Spawnloc = enemy.starting_position;
		if (enemy.obj.pos.y <= Spawnloc.y) {
			enemy.speed =  120.f;
			MoveTowardsFLY(enemy, Spawnloc);
			enemy.timePassed = 0.0f;
		}
		else {

			if (enemy.timePassed >= 1.0f) {
				enemy.timePassed = 0.0f;
				enemy.isShooting = true;
				enemy.speed = 80.f;
				enemy.enemyNext = ENEMY_ATTACK;
			}
		}
		break;

	case ENEMY_ATTACK:
		if (enemy.wing1.isAlive && enemy.wing2.isAlive) {

			static int loopCounter = 0;	//each wing to shoot n bullets, then swap wing

			if (loopCounter < 20) {	//20bullets
				if (CanPartFire(enemy.wing1) && enemy.wing1.isAlive) {
					Spawnloc.x = enemy.wing1.obj.pos.x;
					Spawnloc.y = enemy.wing1.obj.pos.y;
					SpawnBullet(Spawnloc, player.obj.pos, enemy.bullets);
					loopCounter++;
				}
			}
			else {
				if (CanPartFire(enemy.wing2) && enemy.wing2.isAlive) {
					Spawnloc.x = enemy.wing2.obj.pos.x;
					Spawnloc.y = enemy.wing2.obj.pos.y;
					SpawnBullet(Spawnloc, player.obj.pos, enemy.bullets);
					loopCounter++;
				}
			}

			if (loopCounter == 40) {
				loopCounter = 0;
			}
		}

		else {
			static bool isChoosing = true;

			static bool isCharging = false;
			static bool isReversing = false;
			static bool isJumping = false;

			enemy.timePassed += (f32)AEFrameRateControllerGetFrameTime();

			//locking on which direction to dash
			if (enemy.target_position == ENEMY_DEFAULT) {
				if (enemy.obj.pos.x >= player.obj.pos.x) {
					enemy.target_position = ENEMY_LEFT;
				}
				else {
					enemy.target_position = ENEMY_RIGHT;
				}
			}




			// Reversing logic
			if (isReversing) {
				//std::cout << "Reversing\n";
				MoveTowards(enemy, enemy.waypoint);
				if (reachedPos(enemy, enemy.waypoint)) {
					isReversing = false; // Done with reverse
					isCharging = true; // Start charging
					enemy.speed = 200.f;
				}
			}

			// Charging logic
			if (isCharging) {
				//std::cout << "Charging\n";
				Attack_Charge(enemy, enemy.target_position, 400.f);
				if (enemy.timePassed >= 1.f) {
					enemy.timePassed = 0.0f;
					enemy.speed = 80.f;
					enemy.target_position = ENEMY_DEFAULT;
					isCharging = false; // Done with charging
					isChoosing = true; // Set isChoosing to true to prepare for the next action
				}
			}

			// Jumping logic
			if (isJumping) {
				if (enemy.timePassed >= 0.5f) {
					enemy.timePassed = 0.0f;
					if (enemy.onFloor) {
						Jump(enemy, 500.f);
						isJumping = false;
						isChoosing = true;
					}
				}
			}

			if (!enemy.onFloor) {
				MoveTowards(enemy, player.obj.pos);
			}

			// Choosing attack logic
			if (isChoosing) {
				//std::cout << "Choosing next attack\n";

				if (enemy.timePassed >= 2.f) {
					enemy.timePassed = 0.0f;
					switch (rand() % 2) {	//randomize is seeded 
					case 0:
						enemy.timePassed = 0.f;
						isJumping = true;
						break;
					case 1:

						//set way point
						enemy.waypoint = { enemy.obj.pos.x, enemy.obj.pos.y };
						if (enemy.target_position == ENEMY_LEFT) {
							enemy.waypoint.x += 30.f;	//go right
						}
						else if (enemy.target_position == ENEMY_RIGHT) {
							enemy.waypoint.x -= 30.f;	//go left
						}
						isChoosing = false;
						isReversing = true;
						enemy.speed = 120.f;

						break;
					}
					isChoosing = false; // Reset isChoosing after choosing attack

				}
			}















			//CHAT GPT FAILED
			//// Locking on which direction to dash
			//if (enemy.target_position == ENEMY_DEFAULT) {
			//	enemy.target_position = (enemy.obj.pos.x >= player.obj.pos.x) ? ENEMY_LEFT : ENEMY_RIGHT;
			//}

			//// Jumping logic
			//if (!isChoosing && enemy.onFloor && enemy.timePassed >= 2.f) {
			//	switch (rand() % 2) {
			//	case 0:
			//		Jump(enemy, 500.f);
			//		break;
			//	case 1:
			//		enemy.waypoint = enemy.obj.pos;
			//		enemy.waypoint.x += (enemy.target_position == ENEMY_LEFT) ? 30.f : -30.f;
			//		isReversing = true;
			//		enemy.speed = 100.f;
			//		break;
			//	}
			//	isChoosing = true;
			//	enemy.timePassed = 0.f;
			//}

			//// Action logic
			//if (isReversing) {
			//	MoveTowards(enemy, enemy.waypoint);
			//	if (reachedPos(enemy, enemy.waypoint)) {
			//		isReversing = false;
			//		isCharging = true;
			//		enemy.speed = 200.f;
			//	}
			//}
			//else if (isCharging) {
			//	Attack_Charge(enemy, enemy.target_position, 400.f);
			//	if (enemy.timePassed >= 1.f) {
			//		enemy.timePassed = 0.0f;
			//		enemy.speed = 80.f;
			//		enemy.target_position = ENEMY_DEFAULT;
			//		isCharging = false;
			//	}
			//}

			//// Move towards player if not on floor
			//if (!enemy.onFloor) {
			//	MoveTowards(enemy, player.obj.pos);
			//}










		}


		break;

	}




	//set to next state
	enemy.enemyCurrent = enemy.enemyNext;

	//for gravity
	if (!enemy.isFlying) {
		enemy.obj.pos.y += enemy.velocity.y * (f32)AEFrameRateControllerGetFrameTime();
	}

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
