#pragma once
#include "Utils.h"
#include "Armor_add.h"
#include "Weapon.h"
#include "CollisionShape.h" //To add AABB boxes
#include "Armor_add.h"
#include "Weapon.h"

struct Player {
	Object obj;

	bool isFacingRight;
	bool onFloor; //Added to check entity on floor, hence can jump
	bool isAttacking;

	AEVec2 expectedLocation;
	AEVec2 velocity; //Added for movement - Johny
	AEVec2 collisionNormal;
	f32 lookAheadMutliplier;

	Armor equippedArmor;

	Weapon equippedWeapon;
	//Gravity affection
	f32 mass;
	
	//Collision boxes
	AABB collisionBox;
	AABB boxHeadFeet;
	AABB boxArms;

	f32 attackTime;
	bool burningEffect;
	int combo_trig;
	float comboTime;
	int comboState;
};

Player* PlayerInitialize(const char* filename, AEVec2 scale, AEVec2 location, AEVec2 speed, bool isFacingRight); 
void PlayerUpdate(Player& player);