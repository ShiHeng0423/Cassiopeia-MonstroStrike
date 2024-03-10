#pragma once
#include "Utils.h"
#include "Player.h"
#include "CollisionShape.h"
#include <vector>






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
	ENEMY_PASSIVE,
	ENEMY_BOSS1,
};


 
struct Bullet {
	Object obj;

	float bulletSpeed;
	AEVec2 bulletVel;
	int lifetime;
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
	f32 health;

};


struct Enemy {

	EnemyPart wing1, wing2;
	Object obj;
	AEGfxTexture* angryTex;

	AEVec2 startingPosition;	//startinglocation whr enemy spawns
	AEVec2 lastPosition;		//only used by flying enemy
	AEVec2 wayPoint;			//wayPoints are for enemy idle back n forth points
	s8 targetPosition;		//some enemies need target a specific location
	bool loopIdle;

	s8 enemyCurrent;
	s8 enemyNext;
	s8 enemyType;
	
	s8 attackState;
	
	bool isAlive;
	bool isShooting;
	bool isCollision;
	bool isFlying;
	bool isFalling;
	bool isCollidedWithPlayer;
	bool isRecoil;

	f32 timePassed;				//use to "pause" the enemy 

//(stats)----------------------------------------------
	f32 speed;
	f32 lineOfSight;
	f32 shootingRange;
	f32 fireRate;
	f32 timeSinceLastFire;
	f32 health;

	//Gravity affection
	f32 mass;
	f32 gravityForce;
	AEVec2 velocity;			//speed is the scalar of the velocity
//(stats)----------------------------------------------

	f32 stuckTimer; 
	bool onFloor; //Added to check entity on floor, hence can jump
	//Gravity affection
	AEVec2 collisionNormal; 

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
void Enemy_Update_Choose(Enemy& enemy, struct Player& player);







void ENEMY_JUMPER_Update(Enemy& enemy, struct Player& player);
void ENEMY_CHARGER_Update(Enemy& enemy, struct Player& player);
void ENEMY_FLY_Update(Enemy& enemy, struct Player& player);
void ENEMY_BOSS_Update(Enemy& enemy, struct Player& player);



