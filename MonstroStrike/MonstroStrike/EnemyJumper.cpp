#include "Enemy.h"
#include "EnemyUtils.h"
#include "Player.h"
#include "AEEngine.h"
#include "Physics.h"

#include <iostream>


//void ENEMY_JUMPER_Update(Enemy& enemy, struct Player& player)
//{
//	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);
//
//	if (enemy.health <= 0)
//	{
//		enemy.isAlive = false;
//	}
//
//	switch (enemy.enemyCurrent)
//	{
//	case ENEMY_IDLE:
//
//		if (distanceFromPlayer <= enemy.lineOfSight) {
//			enemy.enemyNext = ENEMY_TRANSITION;
//			enemy.isCollision = false;
//			enemy.timePassed = 0.0f;
//			enemy.loop_idle = false;
//		}
//		else {
//			enemy.enemyNext = ENEMY_IDLE;
//			enemy.timePassed += (f32)AEFrameRateControllerGetFrameTime();
//			if (enemy.loop_idle) {
//
//
//				if (enemy.timePassed >= 2.0f) {
//					enemy.timePassed = 0.0f;
//					if (enemy.onFloor) {
//						Jump(enemy, 500.f);
//					}
//				}
//				if (!enemy.onFloor) {
//					MoveTowards(enemy, enemy.waypoint);
//				}
//
//				if ((enemy.obj.pos.x >= enemy.waypoint.x - 2.0f) && (enemy.obj.pos.x <= enemy.waypoint.x + 2.0f)) {
//					enemy.loop_idle = false;
//				}
//			}
//			if ((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) {
//				enemy.loop_idle = true;
//			}
//			if (!((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) && !(enemy.loop_idle)) {
//
//				if (enemy.timePassed >= 2.0f) {
//					enemy.timePassed = 0.0f;
//					if (enemy.onFloor) {
//						Jump(enemy, 500.f);
//					}
//				}
//				if (!enemy.onFloor) {
//					MoveTowards(enemy, enemy.starting_position);
//				}
//
//
//
//
//			}
//		}
//		break;
//	case ENEMY_TRANSITION:
//
//		//Lock on
//		if (enemy.target_position == ENEMY_DEFAULT) {	//finding which direction to charge towards
//			if (enemy.obj.pos.x >= player.obj.pos.x) {
//				enemy.target_position = ENEMY_LEFT;
//			}
//			if (enemy.obj.pos.x <= player.obj.pos.x) {
//				enemy.target_position = ENEMY_RIGHT;
//			}
//		}
//
//
//
//		enemy.timePassed += (f32)AEFrameRateControllerGetFrameTime();
//		enemy.isShooting = true;
//
//		if (enemy.timePassed >= 1.0f) {
//			enemy.timePassed = 0.0f;
//			enemy.enemyNext = ENEMY_ATTACK;
//		}
//
//		break;
//	case ENEMY_ATTACK:
//		enemy.timePassed += (f32)AEFrameRateControllerGetFrameTime();
//
//
//
//		if (distanceFromPlayer < enemy.lineOfSight) {
//
//			if (enemy.timePassed >= 1.f) {
//				enemy.timePassed = 0.0f;
//				if (enemy.onFloor) {
//					Jump(enemy, 600.f);
//				}
//			}
//			if (!enemy.onFloor) {
//				MoveTowards(enemy, player.obj.pos);
//			}
//			
//		}
//		else {
//			enemy.isShooting = false;
//			enemy.timePassed = 0.f;
//			enemy.enemyNext = ENEMY_IDLE;
//		}
//
//
//
//		break;
//	default:
//		break;
//	}
//
//	//for gravity
//	enemy.obj.pos.y += enemy.velocity.y * (f32)AEFrameRateControllerGetFrameTime();
//
//	enemy.enemyCurrent = enemy.enemyNext;
//}


void ENEMY_JUMPER_Update(Enemy& enemy, struct Player& player) {
    const f32 frameTime = (f32)AEFrameRateControllerGetFrameTime();
    const f32 jumpForce = 500.f;

    // Update enemy state based on health
    if (enemy.health <= 0) {
        enemy.isAlive = false;
        return;
    }

    // Calculate distance from player
    f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);

    switch (enemy.enemyCurrent) {
    case ENEMY_IDLE:
        // Check if player is within line of sight
        if (distanceFromPlayer <= enemy.lineOfSight) {
            enemy.enemyNext = ENEMY_TRANSITION;
            enemy.isCollision = false;
            enemy.timePassed = 0.0f;
            enemy.loop_idle = false;
        }
        else {
            enemy.enemyNext = ENEMY_IDLE;
            enemy.timePassed += frameTime;

            if (enemy.loop_idle) {
                if (enemy.timePassed >= 2.0f) {
                    enemy.timePassed = 0.0f;
                    if (enemy.onFloor) {
                        Jump(enemy, jumpForce);
                    }
                }
                if (!enemy.onFloor) {
                    MoveTowards(enemy, enemy.waypoint);
                }

                if (AEInRange(enemy.obj.pos.x, enemy.waypoint.x, 2.0f)) {
                    enemy.loop_idle = false;
                }
            }

            if (AEInRange(enemy.obj.pos.x, enemy.starting_position.x, 1.0f) && !enemy.loop_idle) {
                enemy.loop_idle = true;
            }

            if (!AEInRange(enemy.obj.pos.x, enemy.starting_position.x, 1.0f) && !enemy.loop_idle) {
                if (enemy.timePassed >= 2.0f) {
                    enemy.timePassed = 0.0f;
                    if (enemy.onFloor) {
                        Jump(enemy, jumpForce);
                    }
                }
                if (!enemy.onFloor) {
                    MoveTowards(enemy, enemy.starting_position);
                }
            }
        }
        break;
    case ENEMY_TRANSITION:
        if (enemy.target_position == ENEMY_DEFAULT) {
            // Determine which direction to charge towards
            enemy.target_position = (enemy.obj.pos.x >= player.obj.pos.x) ? ENEMY_LEFT : ENEMY_RIGHT;
        }
        enemy.timePassed += frameTime;
        enemy.isShooting = true;
        if (enemy.timePassed >= 1.0f) {
            enemy.timePassed = 0.0f;
            enemy.enemyNext = ENEMY_ATTACK;
        }
        break;
    case ENEMY_ATTACK:
        enemy.timePassed += frameTime;
        if (distanceFromPlayer < enemy.lineOfSight) {
            if (enemy.timePassed >= 1.f) {
                enemy.timePassed = 0.0f;
                if (enemy.onFloor) {
                    Jump(enemy, 600.f);
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
        }
        break;
    default:
        break;
    }

    // Apply gravity
    enemy.obj.pos.y += enemy.velocity.y * frameTime;

    enemy.enemyCurrent = enemy.enemyNext;
}