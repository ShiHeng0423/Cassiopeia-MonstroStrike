#ifndef ENEMY_UTILS_H
#define ENEMY_UTILS_H
#include "AEEngine.h"
#include "Utils.h"
#include "Player.h"
#include "CollisionShape.h"
#include <vector>

extern AEGfxTexture* bulletTex;



//(EnemyUtils)-------------------------------------------------------------------------
void MoveTowards(Enemy& moving_entity, AEVec2 targetPosition);
void MoveTowardsFLY(Enemy& moving_entity, AEVec2 targetPosition);	//only for the flying enemy
bool CanFire(Enemy& enemy);
bool CanPartFire(EnemyPart& part);
void SpawnBullet(AEVec2& enemy_position, AEVec2& player_position, std::vector<Bullet>& bullets);
void DrawBullets(Enemy& enemy, AEGfxVertexList* pWhiteSquareMesh);

void Attack_Charge(Enemy& enemy, s8 targetPosition, f32 speed);
bool areAligned(AEVec2 player_position, AEVec2 enemy_position);	//checks if player and enemy y position are the same
void Jump(Enemy& enemy, f32 value);
void isStuck(Enemy& enemy);
bool reachedPos(Enemy& enemy, AEVec2 wayPoint);
//(EnemyUtils)-------------------------------------------------------------------------


#endif ENEMY_UTILS_H