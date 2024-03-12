#include "Enemy.h"
#include "EnemyUtils.h"



void ENEMY_CHARGER_Update(Enemy& enemy, struct Player& player)
{
	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);
	enemy.timePassed += (f32)AEFrameRateControllerGetFrameTime();

	// Check collision with player
	enemy.isCollidedWithPlayer = AABBvsAABB(enemy.collisionBox, player.collisionBox);
	
	if (enemy.health <= 0)
	{
		enemy.isAlive = false;
	}

	// Handle collision with player
	if (enemy.isCollidedWithPlayer) {
		if (enemy.targetPosition == ENEMY_LEFT) {
			enemy.wayPoint.x = enemy.obj.pos.x + 100.f; // Move right
		}
		else if (enemy.targetPosition == ENEMY_RIGHT) {
			enemy.wayPoint.x = enemy.obj.pos.x - 100.f; // Move left
		}
		enemy.speed = 120.f;
		enemy.isRecoil = true;
		if (!enemy.hasDealtDmg) {
			enemy.hasDealtDmg = true;
			std::cout << "Hit!\n";
		}
	}
	else {
		enemy.hasDealtDmg = false;
	}

	// If the enemy is in recoil state, move towards the wayPoint until reached or collision occurs
	if (enemy.isRecoil) {
		MoveTowards(enemy, enemy.wayPoint);
		if (ReachedPos(enemy, enemy.wayPoint) || enemy.isCollision) {
			enemy.enemyCurrent = ENEMY_IDLE;
			enemy.speed = 80.f;
			enemy.isRecoil = false;
		}
	}
	else {
		// Based on enemy's current state, perform appropriate actions
		switch (enemy.enemyCurrent) {
		case ENEMY_IDLE:
			// Check if player is within line of sight and aligned with enemy
			if (distanceFromPlayer <= enemy.lineOfSight && AreAligned(player.obj.pos, enemy.obj.pos)) {
				enemy.enemyNext = ENEMY_TRANSITION;
				enemy.isCollision = false;
				enemy.timePassed = 0.0f;
			}
			else {
				// If there's a collision, update target position and perform charge attack
				if (enemy.isCollision) {
					enemy.isCollision = false;
					enemy.targetPosition = (enemy.targetPosition == ENEMY_LEFT) ? ENEMY_RIGHT : ENEMY_LEFT;
				}
				Attack_Charge(enemy, enemy.targetPosition, 80.f);
			}
			break;
		case ENEMY_TRANSITION:
			// Lock on to player's position
			if (enemy.targetPosition == ENEMY_DEFAULT) {
				enemy.targetPosition = (enemy.obj.pos.x >= player.obj.pos.x) ? ENEMY_LEFT : ENEMY_RIGHT;
			}
			enemy.isAttacking = true;
			enemy.isCollision = false;
			if (enemy.timePassed >= 1.0f) {
				enemy.timePassed = 0.0f;
				enemy.enemyNext = ENEMY_ATTACK;
			}
			break;
		case ENEMY_ATTACK:
			// Perform charge attack
			Attack_Charge(enemy, enemy.targetPosition, 200.f);
			if (enemy.isCollision) {
				// Reset enemy state after collision
				enemy.isCollision = false;
				enemy.isAttacking = false;
				enemy.targetPosition = ENEMY_DEFAULT;
				enemy.timePassed = 0.0f;
				enemy.speed = 80.f;
				enemy.enemyNext = ENEMY_IDLE;
			}
			break;
		default:
			break;
		}
	}





	//for gravity
	enemy.obj.pos.y += enemy.velocity.y * (f32)AEFrameRateControllerGetFrameTime();

	enemy.enemyCurrent = enemy.enemyNext;
}



