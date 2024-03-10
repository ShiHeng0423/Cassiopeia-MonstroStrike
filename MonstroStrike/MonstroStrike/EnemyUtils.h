#ifndef ENEMY_UTILS_H
#define ENEMY_UTILS_H
#include "AEEngine.h"
#include "Utils.h"
#include "Player.h"
#include "CollisionShape.h"
#include <vector>

extern AEGfxTexture* bulletTex;

//(EnemyUtils)-------------------------------------------------------------------------
void MoveTowards(Enemy& movingEntity, AEVec2 targetPosition);
void MoveTowardsFLY(Enemy& movingEntity, AEVec2 targetPosition);	//only for the flying enemy
bool CanFire(Enemy& enemy);
bool CanPartFire(EnemyPart& part);
void SpawnBullet(AEVec2& enemyPosition, AEVec2& playerPosition, std::vector<Bullet>& bullets);
void DrawBullets(Enemy& enemy, AEGfxVertexList* pWhiteSquareMesh);

void Attack_Charge(Enemy& enemy, s8 targetPosition, f32 speed);
bool areAligned(AEVec2 playerPosition, AEVec2 enemyPosition);	//checks if player and enemy y position are the same
void Jump(Enemy& enemy, f32 value);
void IsStuck(Enemy& enemy);
bool ReachedPos(Enemy& enemy, AEVec2 wayPoint);
//(EnemyUtils)-------------------------------------------------------------------------


#endif ENEMY_UTILS_H