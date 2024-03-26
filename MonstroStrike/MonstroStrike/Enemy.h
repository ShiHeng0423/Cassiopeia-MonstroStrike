#pragma once
#include "Utils.h"
#include "Player.h"
#include "CollisionShape.h"
#include "AEEngine.h"
#include "Physics.h"
#include <vector>
#include <iostream>



enum EnemyDir {
	ENEMY_DEFAULT = 0,
	ENEMY_LEFT,
	ENEMY_RIGHT,
};

enum EnemyStates
{
	ENEMY_IDLE = 0,
	ENEMY_CHASE,
	ENEMY_ATTACK,
	ENEMY_TRANSITION,
};

enum EnemyAttackStates {

	ENEMY_ATTACK_DEFAULT = 0,
	ENEMY_ATTACK_CHARGE,
	ENEMY_ATTACK_JUMP,
	ENEMY_ATTACK_REVERSE,
	ENEMY_ATTACK_CHOOSING,
};

enum EnemyTypes 
{
	ENEMY_JUMPER = 0,
	ENEMY_CHARGER,
	ENEMY_FLY,
	ENEMY_BOSS1,
};

enum Drops {

	ENEMY_JUMPER_DROP = 0,
	ENEMY_CHARGER_DROP,
	ENEMY_FLY_DROP,
	ENEMY_BOSS1_DROP,
};

struct EnemyDrops {
	Object obj;
	Drops dropType;
	AABB collisionBox;
};
 
struct Bullet {
	Object obj;

	float bulletSpeed;
	AEVec2 bulletVel;
	int lifeTime;
	AABB collisionBox;
};

//for wings
struct EnemyPart {
	Object obj;
	AABB collisionBox;

	//stats
	bool isAlive;
	f32 Offset;
	f32 fireRate;
	f32 timeSinceLastFire;
	int health;

};


struct Enemy {

	EnemyPart wing1, wing2;		
	Object obj;
	AEGfxTexture* angryTex;

	AEVec2 startingPosition;	//startinglocation whr enemy spawns
	AEVec2 lastPosition;		//only used by flying enemy
	AEVec2 wayPoint;			//wayPoints are for enemy idle back n forth points
	s8 targetPosition;			//some enemies need target a specific location
	bool loopIdle;

	s8 enemyCurrent;			//enemy current state
	s8 enemyNext;				//enemy next state
	s8 enemyType;				//Type of enemy eg.charger,boss
	
	s8 attackState;				//only the boss use it
	
	bool isAlive;				//alive or dead
	bool isAttacking;			//in attack mode or not	
	bool isCollision;			
	bool isFlying;				//won't be affected by gravity	
	bool isFalling;				//for gravity function
	bool isCollidedWithPlayer;	//ideally use it to check AABB collision with player
	bool isRecoil;				//only the charger uses it
	bool hasDealtDmg;			//use to make enemy only deal 1 instance of dmg

	bool isVisible;

	f32 timePassed;				//use to "pause" the enemy 

//(stats)----------------------------------------------
	f32 speed;
	f32 lineOfSight;
	f32 shootingRange;
	f32 fireRate;
	f32 timeSinceLastFire;
	int health;
	int maxHealth;

	//Gravity affection
	f32 mass;
	f32 gravityForce;
	AEVec2 velocity;			//speed is the scalar of the velocity
//(stats)----------------------------------------------

	f32 stuckTimer;				//only fly enemy uses it
	bool onFloor;				//Added to check entity on floor, hence can jump
	AEVec2 collisionNormal;		//check with direction is the collision from

	//Collision boxes
	AABB collisionBox;
	AABB boxHeadFeet;
	AABB boxArms;

	std::vector<Bullet> bullets;	// Shared vector container for bullets
	AEVec2 spawnPoint;				//point the bullet spawns from

};





void Enemy_Load(s8 enemyType, std::vector<Enemy>& vecEnemy); //loads the sprite
void FreeEnemy(std::vector<Enemy>& vecEnemy);
void Enemy_Init(AEVec2 scale, AEVec2 location, s8 startingState, Enemy& enemy);
void EnemyUpdateChoose(Enemy& enemy, class Player& player, std::vector<EnemyDrops>& vecCollectables);

//the functions to use in levels
void AllEnemyUpdate(std::vector<Enemy>& vecEnemyVar, class Player& player, std::vector<EnemyDrops>& vecCollectables);
void AllEnemyNBulletCollisionCheck(std::vector<Enemy>& vecEnemyVar, AABB gridBoxAABB);
void AllEnemyDraw(std::vector<Enemy>& vecEnemyVar, AEGfxVertexList* pWhitesqrMesh, std::vector<EnemyDrops>& vecCollectables);






void ENEMY_JUMPER_Update(Enemy& enemy, class Player& player, std::vector<EnemyDrops>& vecCollectables);
void ENEMY_CHARGER_Update(Enemy& enemy, class Player& player, std::vector<EnemyDrops>& vecCollectables);
void ENEMY_FLY_Update(Enemy& enemy, class Player& player, std::vector<EnemyDrops>& vecCollectables);
void ENEMY_BOSS_Update(Enemy& enemy, class Player& player, std::vector<EnemyDrops>& vecCollectables);



