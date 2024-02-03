#pragma once
#include "Utils.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "Armor_add.h"
#include "Weapon.h"
#include "CollisionShape.h" //To add AABB boxes
>>>>>>> parent of 725b5c2 (Revert "Merge branch 'main' into Jian-Wei")
=======
#include "Armor_add.h"
#include "Weapon.h"
>>>>>>> parent of 462cee0 (Revert "Updated the weapon struct and the armor struct")

struct Player {
	Object obj;

	bool isFacingRight;
	bool onFloor; //Added to check entity on floor, hence can jump

	AEVec2 expectedLocation;
	AEVec2 velocity; //Added for movement - Johny
	AEVec2 collisionNormal;
	f32 lookAheadMutliplier;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> parent of 462cee0 (Revert "Updated the weapon struct and the armor struct")

	Armor equippedArmor;

	Weapon equippedWeapon;
<<<<<<< HEAD
	
	
	//Gravity affection
	f32 mass;
	
	//Collision boxes
	AABB collisionBox;
	AABB boxHeadFeet;
	AABB boxArms;


>>>>>>> parent of 725b5c2 (Revert "Merge branch 'main' into Jian-Wei")
=======
>>>>>>> parent of 462cee0 (Revert "Updated the weapon struct and the armor struct")
};

Player* PlayerInitialize(const char* filename, AEVec2 scale, AEVec2 location, AEVec2 speed, bool isFacingRight); 
void PlayerUpdate(Player& player);