#pragma once
#include "Utils.h"
#include "Player.h"



enum ENEMY_STATES
{
	ENEMY_IDLE = 0,
	ENEMY_CHASE,
	ENEMY_SHOOT,

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



struct Enemy {
	Object obj = { 0 };

	AEVec2 starting_position = { 0 }; //startinglocation whr enemy spawns
	AEVec2 waypoint1 = { 0 };	//waypoints are for enemy idle back n forth points
	bool loop_idle = true;

	f32 speed = 80.f;
	f32 lineOfSight = 350;
	f32 shootingRange = 250;
	f32 fireRate = 5.0f;

	int enemyCurrent = ENEMY_IDLE;
	int enemyNext = ENEMY_IDLE;
	int enemyType = ENEMY_JUMPER;





	bool onFloor; //Added to check entity on floor, hence can jump
	//Gravity affection
	f32 mass;
	AEVec2 velocity; //speed is the scalar of the velocity
	AEVec2 collisionNormal; 
	//Collision boxes
	AABB collisionBox;
	AABB boxHeadFeet;
	AABB boxArms;


};





Enemy* ENEMY_Init(AEVec2 scale, AEVec2 location, int enemy_type, int startingState);
void ENEMY_Update(Enemy& enemy, Player& player);
void ENEMY1_Update(Enemy& enemy, Player& player);
void Enemy_Free(Enemy* enemy);
void Enemy_Update_Choose(Enemy& enemy, Player& player);

void MoveTowards(AEVec2& moving_entity, AEVec2 target_position, f32 speed);
bool CanFire(f32 fireRate);