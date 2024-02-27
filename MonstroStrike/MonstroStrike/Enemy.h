#pragma once
#include "Utils.h"
#include "Player.h"
#include "CollisionShape.h"
#include <vector>

enum ENEMY_DIR {
	ENEMY_DEFAULT = 0,
	ENEMY_LEFT,
	ENEMY_RIGHT,
};

enum ENEMY_STATES
{
	ENEMY_IDLE = 0,
	ENEMY_CHASE,
	ENEMY_ATTACK,
	ENEMY_TRANSITION,
};

enum ENEMY_TYPES 
{
	ENEMY_JUMPER = 0,
	ENEMY_CHARGER,
	ENEMY_FLY,
	ENEMY_PASSIVE,
	ENEMY_BOSS1,
	ENEMY_BOSS2,
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
	Object obj;
	AEGfxTexture* angrytex;

	AEVec2 starting_position;	//startinglocation whr enemy spawns
	AEVec2 waypoint;			//waypoints are for enemy idle back n forth points
	int target_position;		//some enemies need target a specific location
	bool loop_idle;

	int enemyCurrent;
	int enemyNext;
	int enemyType;
	
	bool isAlive;
	bool isShooting;
	bool isCollision;

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


	bool onFloor; //Added to check entity on floor, hence can jump
	//Gravity affection
	AEVec2 collisionNormal; 

	//Collision boxes
	AABB collisionBox;
	AABB boxHeadFeet;
	AABB boxArms;

	std::vector<Bullet> bullets;	// Shared vector container for bullets



	EnemyPart wing1, wing2;
};





void Enemy_Load(int enemy_type, std::vector<Enemy>& vecEnemy); //loads the sprite
void Enemy_Init(AEVec2 scale, AEVec2 location, int startingState, Enemy& enemy);
void Enemy_Update_Choose(Enemy& enemy, struct Player& player);







void ENEMY_JUMPER_Update(Enemy& enemy, struct Player& player);
void ENEMY_CHARGER_Update(Enemy& enemy, struct Player& player);
void ENEMY_FLY_Update(Enemy& enemy, struct Player& player);
void ENEMY_BOSS_Update(Enemy& enemy, struct Player& player);
void ENEMY_BOSSWING1_Update(Enemy& enemy, struct Player& player);
void ENEMY_BOSSWING2_Update(Enemy& enemy, struct Player& player);



//(EnemyUtils)-------------------------------------------------------------------------
void MoveTowards(Enemy& moving_entity, AEVec2 target_position);
bool CanFire(Enemy& enemy);
bool CanPartFire(EnemyPart& part);
void SpawnBullet(AEVec2& enemy_position, AEVec2& player_position, std::vector<Bullet>& bullets);
void DrawBullets(Enemy& enemy, AEGfxVertexList* pWhiteSquareMesh);

void Attack_Charge(Enemy& enemy, int target_position);
bool areAligned(AEVec2 player_position, AEVec2 enemy_position);	//checks if player and enemy y position are the same
void Jump(Enemy& enemy, f32 value);
//(EnemyUtils)-------------------------------------------------------------------------
