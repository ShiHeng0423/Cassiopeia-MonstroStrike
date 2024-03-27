#include "Enemy.h"
#include "EnemyUtils.h"
#include "ParticleSystem.h"
#include "MissionList.h"






void ENEMY_FLY_Update(Enemy& enemy, class Player& player, std::vector<EnemyDrops>& vecCollectables)
{
	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);
	static f32 timePassed = 0;	//for up and down cos
	enemy.timePassed += (f32)AEFrameRateControllerGetFrameTime();	//time.time
	//std::cout << enemy.isCollision << "\n";
	if (enemy.isCollision) {
		enemy.isCollision = false;
	}

	if (enemy.health <= 0)
	{
		EnemyLootSpawn(enemy, vecCollectables);
		enemy.isAlive = false;
		ParticleEmit(10, enemy.obj.pos.x, enemy.obj.pos.y, 15 * AERandFloat(), 15 * AERandFloat(), 0, ENEMY_DEATH_EFFECT, nullptr);
		missionSystem.fliesKilled++;
		return;
	}

	switch (enemy.enemyCurrent)
	{
	case ENEMY_IDLE:
		//std::cout << player.obj.pos.y << " " << enemy.obj.pos.y << "\n";
		//std::cout << enemy.obj.pos.x << "\n";

		if (distanceFromPlayer <= enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
			enemy.enemyNext = ENEMY_CHASE;
			enemy.loopIdle = false;
		}
		else {
			enemy.enemyNext = ENEMY_IDLE;
			if (!((enemy.obj.pos.x >= enemy.startingPosition.x - 1.0f) && (enemy.obj.pos.x <= enemy.startingPosition.x + 1.0f)) && !(enemy.loopIdle)) {
				MoveTowardsFLY(enemy, enemy.startingPosition);
				IsStuck(enemy);
			}
		}
		break;
	case ENEMY_TRANSITION:


		enemy.isAttacking = true;

		if (enemy.timePassed >= 0.5f) {
			enemy.timePassed = 0.0f;
			if (distanceFromPlayer > enemy.lineOfSight) {
				//Gives Up
				enemy.isAttacking = false;	//turns red color off
				enemy.enemyNext = ENEMY_IDLE;
			}
			else {
				enemy.enemyNext = ENEMY_CHASE;		
			}

		}

		break;
	case ENEMY_CHASE: 
		enemy.isAttacking = true;
		if (distanceFromPlayer <= enemy.shootingRange) {
			enemy.enemyNext = ENEMY_ATTACK;
		}
		else if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {


			MoveTowardsFLY(enemy, player.obj.pos);

			enemy.wayPoint = player.obj.pos;	//set a wayPoint a player's location
			IsStuck(enemy);

		}
		else {
			//player runs out of line of sight range
			MoveTowardsFLY(enemy, enemy.wayPoint);	//go to the last location player was at
			IsStuck(enemy);
			//when it reaches the way point (manual typed numbers are offsets )
			if ((enemy.obj.pos.x >= enemy.wayPoint.x - 10.f && enemy.obj.pos.x <= enemy.wayPoint.x + 10.f)
				&& (enemy.obj.pos.y >= enemy.wayPoint.y - 60.f && enemy.obj.pos.y <= enemy.wayPoint.y + 60.f)){	//.y +- value depends on offset in MoveTowards fly

				enemy.timePassed = 0.0f;
				enemy.enemyNext = ENEMY_TRANSITION; //go to transition to pause there abit
			
			}

		}
		break;
	case ENEMY_ATTACK:

		enemy.isAttacking = true;
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

	//makes enemy fluctuate up and down
	timePassed += (f32)AEFrameRateControllerGetFrameTime();
	f32 verticalMovement = 0.5f *cos(timePassed * (2 * PI / 0.5f));
	enemy.obj.pos.y += enemy.velocity.y * verticalMovement;

	enemy.enemyCurrent = enemy.enemyNext;
}