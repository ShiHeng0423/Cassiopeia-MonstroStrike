/*!************************************************************************
  \file					Player.h
  \project name			Monstrostrike
  \primary author		Teng Shi heng (55%)
  \secondary author		Johny Yong Jun Siang (30% - traps inflicts status effect on player, Collision, gravity system),
						Choo Jian Wei (15% - Player Combat System) 

  \brief				File containing the class Player that represents the 
						player entity in the game. 
						
						It handles player movement, collision detection, 
						rendering of player and its relevant statistics UI, 
						health management, combat mechanics, player
						interaction status.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#pragma once
#include "Utils.h"
#include "Weapon.h"
#include "CollisionShape.h" //To add AABB boxes
#include "Armor.h"
#include "Weapon.h"
#include "CSVMapLoader.h"
#include "StatusEffect.h"
#include <vector>

int const PlayerMaxBasehealth = 100;

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
	int& GetMaxHealth();
	int& GetCurrentHealth();
	void DamageToPlayer(int damageValue);
	void RecoverHpToPlayer(int recoveryValue);

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
	f32& GetFrictionOnPlayer();

	bool& GetIsPlayerOnFloor();
	bool& GetIsPlayerKillBoss();

	void SetPlayerPoisoned(bool getPoison);
	void SetPlayerSlowed(bool getSlow);
	bool& GetPlayerJustDied();
	bool& GetPlayerHeldCombo();

	
	void OnPlayerDeath();
	std::vector<std::pair<Status_Effect_System::Status_Effect, Status_Effect_System::Status_Effect_Source>> playerStatusEffectList;

	bool GetDebugModeImmortal();
	void SetDebugModeImmortal(bool);

	bool GetDebugModeOverpower();
	void SetDebugModeOverpower(bool);

private:

	//Sprite Data
	Object obj;
	AEVec2 prevPos;

	//Gravity affection
	AEVec2 velocity; //Added for movement - Johny
	f32 mass;
	f32 gravityForce;
	f32 friction;

	bool onFloor; //Added to check entity on floor, hence can jump
	bool killedBoss;
	bool justDied;

	//Status effects
	bool isPoisoned;
	bool isSlowed;

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
	bool heldCombo;
	bool isAttacking;

	//is Player currently interacting with NPC
	bool isConversation;
	
	//Camera Follow
	AEVec2 expectedLocation;
	f32 lookAheadMutliplier;
	bool isFacingRight;

	//Player Stats
	int maxHealth;
	int currHealth;
	int attack;
	f32 currStatusCD;
	f32 currStatusMaxCD;

	//List Of Status Effect
	//std::vector<std::pair<Status_Effect_System::Status_Effect, Status_Effect_System::Status_Effect_Source>> playerStatusEffectList;

	//Player Armor & Weapon
	Armor_System::Armor_Set armorSet;
	Weapon_System::Weapon_Set weaponSet;

	//player texture
	AEGfxTexture* FacingLeft;
	AEGfxTexture* FacingRight;

	//debugging ability
	bool immortalHp;
	bool maxAttackPower;
};

