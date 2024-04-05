#pragma once
#include "AEEngine.h"
#include "Utils.h"
#include "Player.h"
#include "CollisionShape.h"
#include "Inventory.h"
#include <vector>
#include <iostream>
#include <cmath> // For abs function

extern AEGfxTexture* bulletTex;
extern AEGfxTexture* enemyJumperDropTex;
extern AEGfxTexture* enemyChargerDropTex;
extern AEGfxTexture* enemyFlyDropTex;
extern AEGfxTexture* enemyBoss1DropTex;


//(EnemyUtils)-------------------------------------------------------------------------
void MoveTowards(Enemy& movingEntity, AEVec2 targetPosition);
void MoveTowardsFLY(Enemy& movingEntity, AEVec2 targetPosition);	//only for the flying enemy
bool CanFire(Enemy& enemy);
bool CanPartFire(EnemyPart& part);
void SpawnBullet(AEVec2& enemyPosition, AEVec2& playerPosition, std::vector<Bullet>& bullets);
void DrawBullets(Enemy& enemy, AEGfxVertexList* pWhiteSquareMesh);

void Attack_Charge(Enemy& enemy, s8 targetPosition, f32 speed);
bool AreAligned(AEVec2 playerPosition, AEVec2 enemyPosition);	//checks if player and enemy y position are the same
void Jump(Enemy& enemy, f32 value);
void IsStuck(Enemy& enemy);
bool ReachedPos(Enemy& enemy, AEVec2 wayPoint);
void EnemyLootSpawn(Enemy& enemy, std::vector<EnemyDrops>& vecCollectables);
void DrawEnemyLoot(std::vector<EnemyDrops>& vecCollectables, AEGfxVertexList* pWhiteSquareMesh);
void DrawEnemyHp(Enemy& enemy, AEGfxVertexList* pWhitesqrMesh, AEGfxVertexList* pMeshRed);
void DrawEnemyHpParts(EnemyPart& enemy, AEGfxVertexList* pWhitesqrMesh);
void DrawBossHp(Enemy& enemy, AEGfxVertexList* pWhitesqrMesh, AEGfxVertexList* pMeshRed);
//(EnemyUtils)-------------------------------------------------------------------------


