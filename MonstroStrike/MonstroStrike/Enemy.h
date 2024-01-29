#pragma once
#include "Utils.h"
#include "Player.h"



enum ENEMY_STATES
{
	ENEMY_IDLE = 0,
	ENEMY_CHASE,
	ENEMY_SHOOT,
	ENEMY_RETURN

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
};





Enemy* ENEMY_Init(const char* filename, AEVec2 scale, AEVec2 location, AEVec2 speed, int startingState);
void ENEMY_Update(Enemy& enemy, Player& player);
void Enemy_Free(Enemy* enemy);

void MoveTowards(AEVec2& moving_entity, AEVec2 target_position, f32 speed);
bool CanFire(f32 fireRate);