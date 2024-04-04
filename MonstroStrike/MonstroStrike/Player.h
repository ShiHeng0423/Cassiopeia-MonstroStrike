#pragma once
#include "Utils.h"
#include "Weapon.h"
#include "CollisionShape.h" //To add AABB boxes
#include "Armor.h"
#include "Weapon.h"

f32 const PlayerMaxBasehealth = 100.f;

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

	Armor_System::Armor equippedArmor;

	Weapon equippedWeapon;
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
	bool heldCombo = false;
	f32 gravityForce;

	//Player Stats
	f32 maxHealth;
	f32 currHealth;
	f32 attack;
	f32 defence;

	bool isConversation;

	Armor_System::Armor piece[4];
};

Player* PlayerInitialize(const char* fileName, AEVec2 scale, AEVec2 location, AEVec2 speed, bool isFacingRight); 
void PlayerUpdate(Player& player, bool isInventoryOpen);