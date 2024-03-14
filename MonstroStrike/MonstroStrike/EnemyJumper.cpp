#include "Enemy.h"
#include "EnemyUtils.h"



void ENEMY_JUMPER_Update(Enemy& enemy, class Player& player) {
    const f32 frameTime = (f32)AEFrameRateControllerGetFrameTime();
    f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);
    enemy.isCollidedWithPlayer = AABBvsAABB(enemy.collisionBox, player.collisionBox);

    // Update enemy state based on health
    if (enemy.health <= 0) {
        enemy.isAlive = false;
        return;
    }

    // Handle collision with player
    if (enemy.isCollidedWithPlayer) {
        if (!enemy.hasDealtDmg) {
            enemy.hasDealtDmg = true;
            std::cout << "Hit!\n";
        }
    }
    else {
        enemy.hasDealtDmg = false;
    }


    switch (enemy.enemyCurrent) {
    case ENEMY_IDLE:
        // Check if player is within line of sight
        if (distanceFromPlayer <= enemy.lineOfSight) {
            enemy.enemyNext = ENEMY_TRANSITION;
            enemy.isCollision = false;
            enemy.timePassed = 0.0f;
            enemy.loopIdle = false;
        }
        else {
            enemy.enemyNext = ENEMY_IDLE;
            enemy.timePassed += frameTime;

            if (enemy.loopIdle) {
                if (enemy.timePassed >= 2.0f) {
                    enemy.timePassed = 0.0f;
                    if (enemy.onFloor) {
                        Jump(enemy, 500.f);
                    }
                }
                if (!enemy.onFloor) {
                    MoveTowards(enemy, enemy.wayPoint);
                }

                if (AEInRange(enemy.obj.pos.x, enemy.wayPoint.x, 2.0f)) {
                    enemy.loopIdle = false;
                }
            }

            if (AEInRange(enemy.obj.pos.x, enemy.startingPosition.x, 1.0f) && !enemy.loopIdle) {
                enemy.loopIdle = true;
            }

            if (!AEInRange(enemy.obj.pos.x, enemy.startingPosition.x, 1.0f) && !enemy.loopIdle) {
                if (enemy.timePassed >= 2.0f) {
                    enemy.timePassed = 0.0f;
                    if (enemy.onFloor) {
                        Jump(enemy, 500.f);
                    }
                }
                if (!enemy.onFloor) {
                    MoveTowards(enemy, enemy.startingPosition);
                }
            }
        }
        break;
    case ENEMY_TRANSITION:
        if (enemy.targetPosition == ENEMY_DEFAULT) {
            // Determine which direction to charge towards
            enemy.targetPosition = (enemy.obj.pos.x >= player.obj.pos.x) ? ENEMY_LEFT : ENEMY_RIGHT;
        }
        enemy.timePassed += frameTime;
        enemy.isAttacking = true;
        if (enemy.timePassed >= 1.0f) {
            enemy.timePassed = 0.0f;
            enemy.enemyNext = ENEMY_ATTACK;
            enemy.speed = 100.f;
        }
        break;
    case ENEMY_ATTACK:
        enemy.timePassed += frameTime;
        if (distanceFromPlayer < enemy.lineOfSight) {
            if (enemy.timePassed >= 1.5f) {
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
            enemy.isAttacking = false;
            enemy.timePassed = 0.f;
            enemy.enemyNext = ENEMY_IDLE;
            enemy.speed = 80.f;
        }
        break;
    default:
        break;
    }

    // Apply gravity
    enemy.obj.pos.y += enemy.velocity.y * frameTime;

    enemy.enemyCurrent = enemy.enemyNext;
}


