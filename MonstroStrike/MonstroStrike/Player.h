#pragma once
#include "Utils.h"
#include "Armor_add.h"
#include "Weapon.h"
#include "CollisionShape.h" //To add AABB boxes
#include "Armor_add.h"
#include "Weapon.h"

class Player {

public:

	//Player();
	//~Player();

	Object obj;
	AEVec2 prevPos;

	bool isFacingRight;
	bool onFloor; //Added to check entity on floor, hence can jump
	bool isAttacking;
	bool isFalling;

	AEVec2 expectedLocation;
	AEVec2 velocity; //Added for movement - Johny
	AEVec2 collisionNormal;
	f32 lookAheadMutliplier;

	Armor equippedArmor;

	Weapon *equippedWeapon;
	//Gravity affection
	f32 mass;
	
	//Collision boxes
	AABB prevcollisionBox;
	AABB collisionBox;
	AABB boxHeadFeet;
	AABB boxArms;

	f32 attackTime;
	bool burningEffect;
	int comboTrig;
	float comboTime;
	int comboState;
	f32 gravityForce;

	//Player Stats
	f32 maxHealth;
	f32 currHealth;
	f32 attack;
	f32 defence;
};

Player* PlayerInitialize(const char* fileName, AEVec2 scale, AEVec2 location, AEVec2 speed, bool isFacingRight); 
void PlayerUpdate(Player& player, bool isInventoryOpen);