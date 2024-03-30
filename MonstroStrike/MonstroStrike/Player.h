#pragma once
#include "Utils.h"
#include "Weapon.h"
#include "CollisionShape.h" //To add AABB boxes
#include "Armor.h"
#include "Weapon.h"
#include "CSVMapLoader.h"
#include "StatusEffect.h"

#include <vector>

f32 const PlayerMaxBasehealth = 100.f;


class Player {


public:

	Player(AEVec2 scale, AEVec2 location, AEVec2 speed, bool playerFacingRight);
	~Player();

	void Update(bool isInventoryOpen);
	void CheckPlayerGridCollision(Grids2D** gridMap, int maxRow, int maxCol);
	void RenderPlayer();
	void RenderPlayerStatUI();

	//Camera Effects
	AEVec2& GetCameraExpectedPosition();
	bool IsPlayerFacingRight();

	//Weapon & Armor
	Armor_System::Armor_Set& GetArmorSet();
	Weapon_System::Weapon_Set& GetWeaponSet();

	//Player Health
	f32& GetMaxHealth();
	f32& GetCurrentHealth();

	//combo system
	int& GetComboState();
	bool& GetIsPlayerAttacking();

	//Get Player Sprite Data
	AEVec2& GetPlayerCurrentPosition();
	AEVec2& GetPlayerScale();

	//Get If Player currently talking to npc
	bool& GetIsTalkingToNpc();

	//Collision boxes Data
	AABB& GetPlayerBoxHeadFeet();
	AABB& GetPlayerBoxArm();
	AABB& GetPlayerCollisionBox();

	AEVec2& GetPlayerVelocity();
	AEVec2& GetPlayerCollisionNormal();

	f32& GetGravityOnPlayer();
	
	bool& GetIsPlayerOnFloor();
	bool& GetIsPlayerFalling();

	void OnPlayerDeath();
	std::vector<std::pair<Status_Effect_System::Status_Effect, Status_Effect_System::Status_Effect_Source>> playerStatusEffectList;
private:

	//Sprite Data
	Object obj;
	AEVec2 prevPos;

	//Gravity affection
	AEVec2 velocity; //Added for movement - Johny
	f32 mass;
	f32 gravityForce;
	bool onFloor; //Added to check entity on floor, hence can jump
	bool isFalling;
	
	//Collision boxes
	AEVec2 collisionNormal;
	AABB prevcollisionBox;
	AABB collisionBox;
	AABB boxHeadFeet;
	AABB boxArms;

	//Attack Combo
	f32 attackTime;
	float comboTime;
	int comboTrig;
	int comboState;
	bool isAttacking;

	//is Player currently interacting with NPC
	bool isConversation;

	//Camera Follow
	AEVec2 expectedLocation;
	f32 lookAheadMutliplier;
	bool isFacingRight;

	//Player Stats
	f32 maxHealth;
	f32 currHealth;
	f32 attack;

	//List Of Status Effect
	//std::vector<std::pair<Status_Effect_System::Status_Effect, Status_Effect_System::Status_Effect_Source>> playerStatusEffectList;

	//Player Armor & Weapon
	Armor_System::Armor_Set armorSet;
	Weapon_System::Weapon_Set weaponSet;

	//player texture
	AEGfxTexture* FacingLeft;
	AEGfxTexture* FacingRight;
};

