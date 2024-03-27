#include "Enemy.h"
#include "EnemyUtils.h"
#include "ParticleSystem.h"





void ENEMY_BOSS_Update(Enemy& enemy, class Player& player, std::vector<EnemyDrops>& vecCollectables)
{
	//f32 distanceFromPlayer = AEVec2Distance(&player.GetPlayerCurrentPosition(), &enemy.obj.pos);
	static f32 timePassed = 0;	//for up and down cos
	AEVec2 Spawnloc1 = { enemy.startingPosition.x, enemy.startingPosition.y + 50.f };

//health check
	if (enemy.health <= 0)
	{
		EnemyLootSpawn(enemy, vecCollectables);
		enemy.isAlive = false;
		ParticleEmit(10, enemy.obj.pos.x, enemy.obj.pos.y, 15 * AERandFloat(), 15 * AERandFloat(), 0, ENEMY_DEATH_EFFECT, nullptr);

	}
	if (enemy.wing1.health <= 0)
	{
		if (enemy.wing1.isAlive) {
			ParticleEmit(10, enemy.obj.pos.x, enemy.obj.pos.y, 15 * AERandFloat(), 15 * AERandFloat(), 0, ENEMY_DEATH_EFFECT, nullptr);
		}
		enemy.wing1.isAlive = false;
	}
	if (enemy.wing2.health <= 0)
	{
		if (enemy.wing2.isAlive) {
			ParticleEmit(10, enemy.obj.pos.x, enemy.obj.pos.y, 15 * AERandFloat(), 15 * AERandFloat(), 0, ENEMY_DEATH_EFFECT, nullptr);
		}
		enemy.wing2.isAlive = false;
	}

	if (enemy.wing1.isAlive == false && enemy.wing2.isAlive == false) {
		enemy.isFlying = false;
	}



	enemy.isCollidedWithPlayer = AABBvsAABB(enemy.collisionBox, player.GetPlayerCollisionBox());
	// Handle collision with player
	if (enemy.isCollidedWithPlayer) {
		if (!enemy.hasDealtDmg) {
			enemy.hasDealtDmg = true;
			player.GetCurrentHealth() -= 20.f;
		}
	}
	else {
		enemy.hasDealtDmg = false;
	}

	switch (enemy.enemyCurrent)
	{
	case ENEMY_IDLE:
		if (enemy.health != enemy.maxHealth) {
			enemy.timePassed = 0.f;
			enemy.enemyNext = ENEMY_TRANSITION;

		}
		break;
	case ENEMY_TRANSITION:
		enemy.timePassed += (f32)AEFrameRateControllerGetFrameTime();
		enemy.wing1.isAlive = true;
		enemy.wing2.isAlive = true;
		enemy.isFlying = true;
		if (enemy.obj.pos.y <= Spawnloc1.y) {
			enemy.speed =  120.f;
			MoveTowardsFLY(enemy, Spawnloc1);
			enemy.timePassed = 0.0f;
		}
		else {

			if (enemy.timePassed >= 1.0f) {
				enemy.timePassed = 0.0f;
				enemy.isAttacking = true;
				enemy.speed = 80.f;
				enemy.enemyNext = ENEMY_ATTACK;
			}
		}
		break;

	case ENEMY_ATTACK:
		if (enemy.wing1.isAlive || enemy.wing2.isAlive) {

			static int loopCounter = 0;	//each wing to shoot n bullets, then swap wing

			if (loopCounter < 20) {	//20bullets
				if (CanPartFire(enemy.wing1) && enemy.wing1.isAlive) {
					Spawnloc1.x = enemy.wing1.obj.pos.x;
					Spawnloc1.y = enemy.wing1.obj.pos.y;
					SpawnBullet(Spawnloc1, player.GetPlayerCurrentPosition(), enemy.bullets);
					loopCounter++;
				}
			}
			else {
				if (CanPartFire(enemy.wing2) && enemy.wing2.isAlive) {
					Spawnloc1.x = enemy.wing2.obj.pos.x;
					Spawnloc1.y = enemy.wing2.obj.pos.y;
					SpawnBullet(Spawnloc1, player.GetPlayerCurrentPosition(), enemy.bullets);
					loopCounter++;
				}
			}

			if (loopCounter == 40) {
				loopCounter = 0;
			}
		}

		else {
			enemy.timePassed += (f32)AEFrameRateControllerGetFrameTime();

			//locking on which direction to dash
			if (enemy.targetPosition == ENEMY_DEFAULT) {
				if (enemy.obj.pos.x >= player.GetPlayerCurrentPosition().x) {
					enemy.targetPosition = ENEMY_LEFT;
				}
				else {
					enemy.targetPosition = ENEMY_RIGHT;
				}
			}

			switch (enemy.attackState) {
			case ENEMY_ATTACK_CHOOSING:

				if (enemy.timePassed >= 2.f) {
					enemy.timePassed = 0.0f;
					switch (rand() % 2) {	//randomize is seeded 
					case 0:
						enemy.timePassed = 0.f;
						enemy.attackState = ENEMY_ATTACK_JUMP;
						break;
					case 1:

						//set way point
						enemy.wayPoint = { enemy.obj.pos.x, enemy.obj.pos.y };
						if (enemy.targetPosition == ENEMY_LEFT) {
							enemy.wayPoint.x += 30.f;	//go right
						}
						else if (enemy.targetPosition == ENEMY_RIGHT) {
							enemy.wayPoint.x -= 30.f;	//go left
						}
						enemy.attackState = ENEMY_ATTACK_REVERSE;
						enemy.speed = 120.f;

						break;
					}

				}
				break;
			case ENEMY_ATTACK_CHARGE:

				Attack_Charge(enemy, enemy.targetPosition, 600.f);
				if (enemy.timePassed >= 1.f) {
					enemy.timePassed = 0.0f;
					enemy.speed = 80.f;
					enemy.targetPosition = ENEMY_DEFAULT;
					enemy.attackState = ENEMY_ATTACK_CHOOSING;
				}
				break;
			case ENEMY_ATTACK_JUMP:

				if (enemy.timePassed >= 0.5f) {
					enemy.timePassed = 0.0f;
					if (enemy.onFloor) {
						Jump(enemy, 1200.f);
						enemy.attackState = ENEMY_ATTACK_CHOOSING;
					}
				}
				break;
			case ENEMY_ATTACK_REVERSE:

				MoveTowards(enemy, enemy.wayPoint);
				if (ReachedPos(enemy, enemy.wayPoint) || enemy.timePassed >= 0.2f) {
					enemy.timePassed = 0.0f;
					enemy.attackState = ENEMY_ATTACK_CHARGE;
				}
				break;
			}//switch end



			if (!enemy.onFloor) { //for the jumping
				MoveTowards(enemy, player.GetPlayerCurrentPosition());
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
	else {
		//makes enemy fluctuate up and down
		timePassed += (f32)AEFrameRateControllerGetFrameTime();
		f32 verticalMovement = 0.5f * cos(timePassed * (2 * PI / 0.5f));
		enemy.obj.pos.y += enemy.velocity.y * verticalMovement;
	}

	//wings collision box
	if (enemy.wing1.isAlive) {
		enemy.wing1.obj.pos.x = enemy.obj.pos.x + enemy.wing1.Offset;
		enemy.wing1.obj.pos.y = enemy.obj.pos.y + 10.f;

		enemy.wing1.collisionBox.minimum.x = enemy.wing1.obj.pos.x - enemy.wing1.obj.scale.x * 0.5f;
		enemy.wing1.collisionBox.minimum.y = enemy.wing1.obj.pos.y - enemy.wing1.obj.scale.y * 0.5f;
		enemy.wing1.collisionBox.maximum.x = enemy.wing1.obj.pos.x + enemy.wing1.obj.scale.x * 0.5f;
		enemy.wing1.collisionBox.maximum.y = enemy.wing1.obj.pos.y + enemy.wing1.obj.scale.y * 0.5f;
	}
	if (enemy.wing2.isAlive) {

		enemy.wing2.obj.pos.x = enemy.obj.pos.x + enemy.wing2.Offset;
		enemy.wing2.obj.pos.y = enemy.obj.pos.y + 10.f;

		enemy.wing2.collisionBox.minimum.x = enemy.wing2.obj.pos.x - enemy.wing2.obj.scale.x * 0.5f;
		enemy.wing2.collisionBox.minimum.y = enemy.wing2.obj.pos.y - enemy.wing2.obj.scale.y * 0.5f;
		enemy.wing2.collisionBox.maximum.x = enemy.wing2.obj.pos.x + enemy.wing2.obj.scale.x * 0.5f;
		enemy.wing2.collisionBox.maximum.y = enemy.wing2.obj.pos.y + enemy.wing2.obj.scale.y * 0.5f;
	}
}
