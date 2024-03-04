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

	//case ENEMY_CHASE:

	//	if (distanceFromPlayer <= enemy.shootingRange) {
	//		enemy.enemyNext = ENEMY_ATTACK;
	//	}
	//	else if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
	//		enemy.enemyNext = ENEMY_CHASE;
	//		MoveTowards(enemy, player.obj.pos);



	//	}

	//	break;

	case ENEMY_ATTACK:
		if (enemy.wing1.isAlive && enemy.wing2.isAlive) {

			static int loopCounter = 0;	//each wing to shoot 20 bullets, then swap wing

			if (loopCounter < 20) {
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
			enemy.timePassed += (f32)AEFrameRateControllerGetFrameTime();
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
