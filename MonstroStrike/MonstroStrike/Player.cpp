/*!************************************************************************
  \file					Player.cpp
  \project name			Monstrostrike
  \primary author		Teng Shi heng (100%)
  \secondary author		Johny Yong Jun Siang (30% - traps inflicts status effect on player, Collision, gravity system),
						Choo Jian Wei (15% - Player Combat System)
  \brief				File containing the definitions of functions declared
						Player.h files.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#include "AEEngine.h"
#include "Inventory.h"
#include "Player.h"
#include "Physics.h"
#include "Weapon.h"
#include "TriggerAttack.h"
#include "MapTransition.h"
#include "ParticleSystem.h"
#include "main.h"
#include "GameStateManager.h"

#include <chrono>
#include <queue>
#include <functional>  // for std::function

namespace
{

#pragma region Attack Combo
	// Define a clock type for high-resolution time measurement
	using Clock = std::chrono::high_resolution_clock;

	// Store the time point of the last input
	auto lastInputTime = Clock::now();
	auto currentTime = Clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastInputTime).count() / 1000.0; // Convert to seconds

#pragma region AnimationQueue


	constexpr f32 comboWindowDuration = 1.0f;
	constexpr f32 PRESS_THRESHOLD = 0.5f;
	//Attack hold and atack release

	bool undealtTriggerInput = false; // identifier
	bool isReleased = true;
	bool if_first_input = false;
	
	auto triggeredTime = Clock::now();
	auto releasedTime = Clock::now();
	auto comboTime = Clock::now();
#pragma endregion

	static f32 particleEmissionRate = 0.f;

	//meshes
	AEGfxVertexList* pMeshRed;
	AEGfxVertexList* pWhiteSquareMesh;

	AEGfxTexture* HealthBorder;
	AEGfxTexture* gearDisplayBorder;

	//status effect sprite
	AEGfxTexture* se_Burning;
	AEGfxTexture* se_Regen;
	AEGfxTexture* se_Lifesteal;
	AEGfxTexture* se_Poison;
	AEGfxTexture* se_Slow;


	//Attack animation
	AEGfxTexture* swordthrustTex;
	AEGfxTexture* revswordthrustTex;
	AEGfxTexture* swordmultithrustTex;
	AEGfxTexture* revswordmultithrustTex;
	AEGfxTexture* swordslashTex;
	AEGfxTexture* revswordslashTex;

	bool displayStatusEffect;
	std::string statusEffectDescription;
}


Player::Player(AEVec2 scale, AEVec2 location, AEVec2 speed, bool playerFacingRight)
{
	//Meshes & Texture
	HealthBorder = AEGfxTextureLoad("Assets/panelInset_beige.png");
	FacingLeft = AEGfxTextureLoad("Assets/PlayerLeft.png");
	FacingRight = AEGfxTextureLoad("Assets/PlayerRight.png");
	gearDisplayBorder = AEGfxTextureLoad("Assets/panel_brown.png");

	se_Burning = AEGfxTextureLoad("Assets/StatusEffects/Status_BurningEffect.png");
	se_Regen = AEGfxTextureLoad("Assets/StatusEffects/Status_Regen.png");
	se_Lifesteal = AEGfxTextureLoad("Assets/StatusEffects/Status_LifeSteal.png");
	se_Poison = AEGfxTextureLoad("Assets/StatusEffects/Status_Poison.png");
	se_Slow = AEGfxTextureLoad("Assets/StatusEffects/Status_Slow.png");


	//Jian Wei (Moved from level 1 to player by Johny)
	swordthrustTex = AEGfxTextureLoad("Assets/Attacks/Sword thrust.png");
	revswordthrustTex = AEGfxTextureLoad("Assets/Attacks/Sword thrustrev.png");
	swordmultithrustTex = AEGfxTextureLoad("Assets/Attacks/sword mutiple thrust.png");
	revswordmultithrustTex = AEGfxTextureLoad("Assets/Attacks/Swordmutiplethrustrev.png");
	swordslashTex = AEGfxTextureLoad("Assets/Attacks/SwordSlash.png");
	revswordslashTex = AEGfxTextureLoad("Assets/Attacks/Sword Slashrev.png");

	pWhiteSquareMesh = GenerateSquareMesh(0xFFFFFFFF);
	pMeshRed = GenerateSquareMesh(0xFFFF0000);

	obj.speed = speed;

	AEVec2Set(&obj.pos, location.x, location.y);
	AEVec2Set(&prevPos, location.x, location.y);
	AEVec2Set(&obj.scale, scale.x, scale.y);

	//Gravity affection
	mass = 60.f;
	gravityForce = 0.f;
	friction = 0.85f;

	onFloor = true; //Set as false first, will be set as true when ground detected
	killedBoss = false;
	isPoisoned = false;
	isSlowed = false;
	justDied = false;
	heldCombo = false;

	AEVec2Set(&velocity, 0.f, 0.f); //Begin with no velocity	

	//camera
	AEVec2Set(&expectedLocation, 0.f, 0.f);
	isFacingRight = playerFacingRight;
	lookAheadMutliplier = 50.f;

	//Initializing collision box starting position
	collisionBox.minimum.x = obj.pos.x - obj.scale.x * 0.5f;
	collisionBox.minimum.y = obj.pos.y - obj.scale.y * 0.5f;
	collisionBox.maximum.x = obj.pos.x + obj.scale.x * 0.5f;
	collisionBox.maximum.y = obj.pos.y + obj.scale.y * 0.5f;

	prevcollisionBox = collisionBox;

	AEVec2Set(&boxArms.maximum, 0.f, 0.f);
	AEVec2Set(&boxHeadFeet.maximum, 0.f, 0.f);
	AEVec2Set(&collisionNormal, 0.f, 0.f);

	//Attack Combo
	isAttacking = false;
	attackTime	= 1.f;
	comboTrig	= 0;
	comboTime	= 0.0f;
	comboState	= 0;

	//is Player currently interacting with NPC
	isConversation = false;

	//Player Stats
	maxHealth = 100;
	currHealth = maxHealth;
	attack = 100;
	currStatusMaxCD = 3.f;
	currStatusCD = currStatusMaxCD;

	immortalHp = false;
	maxAttackPower = false;
}

Player::~Player()
{
	AEGfxTextureUnload(FacingRight);
	AEGfxTextureUnload(FacingLeft);
	AEGfxTextureUnload(HealthBorder);
	AEGfxTextureUnload(gearDisplayBorder);

	AEGfxTextureUnload(se_Burning);
	AEGfxTextureUnload(se_Lifesteal);
	AEGfxTextureUnload(se_Regen);
	AEGfxTextureUnload(se_Poison);
	AEGfxTextureUnload(se_Slow);

	AEGfxMeshFree(pMeshRed);
	AEGfxMeshFree(pWhiteSquareMesh);

	//Jian Wei (Moved from Level 1 to player by Johny)
	AEGfxTextureUnload(swordthrustTex);
	AEGfxTextureUnload(revswordthrustTex);
	AEGfxTextureUnload(swordmultithrustTex);
	AEGfxTextureUnload(revswordmultithrustTex);
	AEGfxTextureUnload(swordslashTex);
	AEGfxTextureUnload(revswordslashTex);

}

void Player::Update(bool isInventoryOpen)
{
	// Player Movement
	if (AEInputCheckCurr(AEVK_D) && !isInventoryOpen)
	{
		velocity.x += obj.speed.x * (f32)AEFrameRateControllerGetFrameTime();
		isFacingRight = true;
		

		if (particleEmissionRate > 0.1f)
		{
			particleEmissionRate = 0.f;
			ParticleEmit(1, obj.pos.x, obj.pos.y,
				obj.scale.x * 0.25f, obj.scale.y * 0.25f, 0.f, PARTICLE_TRAILING, this);
		}
		else
		{
			particleEmissionRate += (f32)AEFrameRateControllerGetFrameTime();
		}

	}
	else if (AEInputCheckCurr(AEVK_A) && !isInventoryOpen)
	{
		velocity.x -= obj.speed.x * (f32)AEFrameRateControllerGetFrameTime();
		isFacingRight = false;
		
		if (particleEmissionRate > 0.1f)
		{
			particleEmissionRate = 0.f;
			ParticleEmit(1, obj.pos.x, obj.pos.y,
				obj.scale.x * 0.25f, obj.scale.y * 0.25f, 0.f, PARTICLE_TRAILING, this);
		}
		else
		{
			particleEmissionRate += (f32)AEFrameRateControllerGetFrameTime();
		}

	}
	
	if (isFacingRight) {
		obj.pTex = FacingRight;
	}
	else {
		obj.pTex = FacingLeft;
	}


	// Apply velocity constraints
	velocity.x = AEClamp(velocity.x, -5.f, 5.f);

	AEVec2 desiredLocation{ velocity.x * lookAheadMutliplier , 0.f };
	AEVec2Add(&expectedLocation, &obj.pos, &desiredLocation);

	//For friction
	if (onFloor) //Means confirm on floor
		velocity.x *= friction; //Friction application

	//For jumping
	if (AEInputCheckTriggered(VK_SPACE) && onFloor && !isInventoryOpen)
	{
		onFloor = false;
		velocity.y = 700.f;		
		ParticleEmit(10, obj.pos.x, obj.pos.y,
		obj.scale.x * 0.25f, obj.scale.y * 0.25f, 0.f, PARTICLE_JUMP, this);
		//audioManager->PlayAudio(false, ATTACK_SLASH_SFX);
	}

	//Apply Gravity
	ApplyGravity(&velocity, mass, &onFloor, &gravityForce); //Velocity passed in must be modifiable, mass can be adjusted if needed to

	//Player position update
	prevPos = obj.pos;
	prevcollisionBox = collisionBox;
	obj.pos.y += velocity.y * (f32)AEFrameRateControllerGetFrameTime();
	obj.pos.x += velocity.x;

	//Resetting main AABB box...
	collisionBox.minimum.x = obj.pos.x - obj.scale.x * 0.5f;
	collisionBox.minimum.y = obj.pos.y - obj.scale.y * 0.5f;
	collisionBox.maximum.x = obj.pos.x + obj.scale.x * 0.5f;
	collisionBox.maximum.y = obj.pos.y + obj.scale.y * 0.5f;

	//Vertical
	f32 verticalOffset = obj.scale.y * 0.025f;

	// Vertical box
	boxHeadFeet = collisionBox;
	boxHeadFeet.minimum.y -= verticalOffset;
	boxHeadFeet.maximum.y += verticalOffset;

	f32 horizontalOffset = obj.scale.x * 0.025f;

	// Horizontal box
	boxArms = collisionBox;
	boxArms.minimum.x -= horizontalOffset;
	boxArms.maximum.x += horizontalOffset;


	//Update player weapon hit box
	/*Weapon hit box update only*/
	if (weaponSet.rarity < Weapon_System::NO_GRADE)
	{
		if (AEInputCheckTriggered(AEVK_LBUTTON) && !isInventoryOpen)
		{
			triggeredTime = Clock::now();
			undealtTriggerInput = true;
			isReleased = false;
			if_first_input = true;
			//audioManager->PlayAudio(false, ATTACK_SLASH_SFX);
		}
		if (AEInputCheckReleased(AEVK_LBUTTON) && !isInventoryOpen)
		{
			isReleased = true;
		}
		//reset
		if (!undealtTriggerInput)
		{
			elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - ::comboTime).count() / 1000.0; // Convert to seconds
			if (elapsedTime > comboWindowDuration)
			{
				weaponSet.weaponHIT = false;
				comboState = 0;
				comboTime = 0.f;
				isAttacking = false;
			}
		}
		if (undealtTriggerInput)
		{
			double g_elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - triggeredTime).count() / 1000.0; // Convert to seconds
				
			if (g_elapsedTime >= PRESS_THRESHOLD && !isReleased)
			{
				if (comboState == 2) //held
				{
					f32 attackProgress = 1.0f - (attackTime / comboWindowDuration);
					UpdateWeaponHitBoxHeld(this, isFacingRight, &weaponSet, attackProgress);
					comboState = 0;
					isAttacking = true;
				}
				::comboTime = Clock::now();
				undealtTriggerInput = false;
				heldCombo = false;
			}

			if (g_elapsedTime < PRESS_THRESHOLD && isReleased) //Trigger (Here is flag for initialization)
			{
				f32 attackProgress = 1.0f - (attackTime / comboWindowDuration);
				UpdateWeaponHitBoxTrig(this, isFacingRight, &weaponSet, attackProgress);
				isAttacking = true;

				if (comboState < 2)
				{
					comboState++;
					comboTime += (float)g_elapsedTime;
				}

				else
				{
					comboState = 0;
					comboTime = 0.0f;
				}
				::comboTime = Clock::now();
				undealtTriggerInput = false;


			}
		}
	}

	Armor_Effect_Update(*this);

	if (isPoisoned)
	{
		if (currStatusCD > 0.f)
		{
			currStatusCD -= (f32)AEFrameRateControllerGetFrameTime();

			// Apply damage over time every specified interval
			const f32 damageInterval = 2.0f; 
			static f32 timeSinceLastDamage = 0.0f;
			timeSinceLastDamage += (f32)AEFrameRateControllerGetFrameTime();
			if (timeSinceLastDamage >= damageInterval)
			{
				const int poisonDamage = (int)(0.1 * (double)currHealth);
				DamageToPlayer(poisonDamage);
				if (currHealth < 1)
				{
					currHealth = 1;
				}
				timeSinceLastDamage = 0.0f; // Reset timer
			}
		}
		else
		{
			isPoisoned = false; // Set status to false
			size_t index = 0;
			for (std::pair<Status_Effect_System::Status_Effect, Status_Effect_System::Status_Effect_Source> effect :
				playerStatusEffectList)
			{
				if (effect.first == Status_Effect_System::Status_Effect::POISON)
				{
					std::cout << index << std::endl;
					break;
				}
				index++;
			}
			playerStatusEffectList.erase(playerStatusEffectList.begin() + index);
			currStatusCD = currStatusMaxCD; // Reset cooldown
		}
	}
	if (isSlowed)
	{
		if (currStatusCD > 0.f)
		{
			currStatusCD -= (f32)AEFrameRateControllerGetFrameTime();
			friction = 0.5f;
		}
		else
		{
			isSlowed = false; //Set status to false
			size_t index = 0;
			for (std::pair<Status_Effect_System::Status_Effect, Status_Effect_System::Status_Effect_Source> effect :
				playerStatusEffectList)
			{
				if (effect.first == Status_Effect_System::Status_Effect::SLOW)
				{
					std::cout << index << std::endl;
					break;
				}
				index++;
			}
			playerStatusEffectList.erase(playerStatusEffectList.begin() + index);
			friction = 0.85f;
			currStatusCD = currStatusMaxCD; //reset cd
		}
	}

	if (currHealth <= 0.f)
	{
		OnPlayerDeath();
	}

	displayStatusEffect = false;
	if (playerStatusEffectList.size() > 0)
	{
		s32 cx, cy;
		AEInputGetCursorPosition(&cx, &cy);
		AEVec2 mousePos{ 0, 0 };
		mousePos.x = cx - AEGfxGetWindowWidth() * 0.5f;
		mousePos.y = AEGfxGetWindowHeight() * 0.5f - cy;

		f32 x, y;
		AEGfxGetCamPosition(&x, &y);

		size_t buff_index = 0;
		for (std::pair<Status_Effect_System::Status_Effect, Status_Effect_System::Status_Effect_Source> effect : playerStatusEffectList)
		{
			AEVec2 translateOrigin = { AEGfxGetWinMinX() + 100.f + buff_index++ * 50.f, AEGfxGetWinMaxY() - 100.f };
			translateOrigin.x -= x;
			translateOrigin.y -= y;
			if (AETestPointToRect(&mousePos, &translateOrigin, 25.f, 25.f))
			{
				switch (effect.first)
				{
				case Status_Effect_System::BURNING:
					statusEffectDescription = "Atk x2, Hp -5 per 3s";
					break;
				case Status_Effect_System::LIFE_STEAL:
					statusEffectDescription = "Lifesteal";
					break;
				case Status_Effect_System::REGEN:
					statusEffectDescription = "Hp +10 per 2s";
					break;
				case Status_Effect_System::POISON:
					statusEffectDescription = "Hp -10% per 2s";
					break;
				case Status_Effect_System::SLOW:
					statusEffectDescription = "Speed -15%";
					break;
				default:
					break;
				}
				displayStatusEffect = true;
				break;
			}
		}
	}
}

//Render player sprite
void Player::RenderPlayer()
{
	if (isSlowed && isPoisoned)
	{
		AEGfxSetColorToMultiply(0.8f, 0.f, 0.f, 1.f);
	}
	else if (isPoisoned)
	{
		AEGfxSetColorToMultiply(0.7f, 0.f, 0.7f, 1.f);
	}
	else if (isSlowed)
	{
		AEGfxSetColorToMultiply(0.4f, 0.4f, 0.4f, 1.f);
	}

#pragma region is_attacking

	if (isAttacking)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTexture* weaponTexture = nullptr;

		if (weaponSet.name == "Short-Sword")
		{
			if (heldCombo)
				weaponTexture = isFacingRight ? swordmultithrustTex : revswordmultithrustTex;
			else
				weaponTexture = isFacingRight ? swordthrustTex : revswordthrustTex;
		}
		else
		{
			weaponTexture = isFacingRight ? swordslashTex : revswordslashTex;
		}

		if (weaponTexture != nullptr)
		{
			AEGfxTextureSet(weaponTexture, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(weaponSet.position.x,
				weaponSet.position.y, 0.f,
				weaponSet.scale.x,
				weaponSet.scale.y).m);

			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}

		isAttacking = false;
	}
#pragma endregion

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(obj.pTex, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(obj.pos.x, obj.pos.y, 0.f, obj.scale.x, obj.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f); //Reset color multiplied
}

//Render Player Health bar and Current equipped weapons
void Player::RenderPlayerStatUI()
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxTextureSet(gearDisplayBorder, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(AEGfxGetWinMinX() + 200.f,
		AEGfxGetWinMaxY() - 60.f, 0.f,
		400.f, 120.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	//Health Border
	//AEGfxTextureSet(HealthBorder, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(AEGfxGetWinMinX() + 225.f, AEGfxGetWinMaxY() - 60.f, 0, 300.f, 25.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	//Health Bar
	AEGfxSetTransform(ObjectTransformationMatrixSet(AEGfxGetWinMinX() + 225.f - ((1.f - ((float)currHealth / (float)maxHealth)) * 150.f), AEGfxGetWinMaxY() - 60.f, 0, (currHealth * 300.f) / maxHealth, 25.f).m);
	AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	//Health Text
	std::string str = "Player Stats";
	f32 width, height;
	AEGfxGetPrintSize(fontID, str.c_str(), 0.3f, &width, &height);
	AEGfxPrint(fontID, str.c_str(), -width / 2 - 0.8f, -height / 2 + 0.95f, 0.3f, 0, 0, 0, 1);

	str = "HP";
	AEGfxGetPrintSize(fontID, str.c_str(), 0.3f, &width, &height);
	AEGfxPrint(fontID, str.c_str(), -width / 2 - 0.95f, -height / 2 + 0.86f, 0.3f, 0, 0, 0, 1);

	str = "Buff";
	AEGfxGetPrintSize(fontID, str.c_str(), 0.3f, &width, &height);
	AEGfxPrint(fontID, str.c_str(), -width / 2 - 0.95f, -height / 2 + 0.78f, 0.3f, 0, 0, 0, 1);

	size_t buff_index = 0;
	for (std::pair<Status_Effect_System::Status_Effect, Status_Effect_System::Status_Effect_Source> effect : playerStatusEffectList)
	{
		switch (effect.first)
		{
		case Status_Effect_System::BURNING:
			AEGfxTextureSet(se_Burning, 0, 0);
			break;
		case Status_Effect_System::LIFE_STEAL:
			AEGfxTextureSet(se_Lifesteal, 0, 0);
			break;
		case Status_Effect_System::REGEN:
			AEGfxTextureSet(se_Regen, 0, 0);
			break;
		case Status_Effect_System::POISON:
			AEGfxTextureSet(se_Poison, 0, 0);
			break;
		case Status_Effect_System::SLOW:
			AEGfxTextureSet(se_Slow, 0, 0);
			break;
		default:
			break;
		}
		AEGfxSetTransform(ObjectTransformationMatrixSet(AEGfxGetWinMinX() + 100.f + buff_index++ * 50.f, AEGfxGetWinMaxY() - 100.f, 0, 25.f, 25.f).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	}

	str = std::to_string((int)currHealth) + "/" + std::to_string((int)maxHealth);
	AEGfxGetPrintSize(fontID, str.c_str(), 0.35f, &width, &height);
	AEGfxPrint(fontID, str.c_str(), -width / 2 - 0.75f, -height / 2 + 0.865f, 0.35f, 0, 0, 0, 1);

	AEGfxTextureSet(gearDisplayBorder, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(AEGfxGetWinMinX() + 225.f,
		AEGfxGetWinMinY() + 60.f, 0.f,
		450.f, 125.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	int index = 1;
	for (ButtonGearUI button : Inventory::equipmentDisplay)
	{
		AEGfxTextureSet(button.img.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(AEGfxGetWinMinX() - 15.f + 80.f * index++,
			AEGfxGetWinMinY() + 50.f, 0.f,
			button.img.scale.x * 0.8f, button.img.scale.y * 0.8f).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	}

	str = "Gears Equipped";
	AEGfxGetPrintSize(fontID, str.c_str(), 0.3f, &width, &height);
	AEGfxPrint(fontID, str.c_str(), -width / 2 - 0.85f, -height / 2 - 0.775f, 0.3f, 1, 1, 1, 1);

	if (displayStatusEffect)
	{
		s32 cx, cy;
		AEInputGetCursorPosition(&cx, &cy);
		AEVec2 mousePos{ 0, 0 };
		mousePos.x = cx - AEGfxGetWindowWidth() * 0.5f;
		mousePos.y = AEGfxGetWindowHeight() * 0.5f - cy;

		AEGfxTextureSet(HealthBorder, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(AEGfxGetWinMinX() + cx + 155.f, AEGfxGetWinMaxY() - cy, 0.f, 270.f, 30.f).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxPrint(fontID, statusEffectDescription.c_str(), mousePos.x / (AEGfxGetWindowWidth() * 0.5f) + 0.03f, mousePos.y / (AEGfxGetWindowHeight() * 0.5f)-0.02f, 0.3f, 1, 1, 1, 1);
	}
}

//Get Player Armor Set
Armor_System::Armor_Set& Player::GetArmorSet()
{
	return armorSet;
}

//Get Player Weapon Set
Weapon_System::Weapon_Set& Player::GetWeaponSet()
{
	return weaponSet;
}

//Get Camera Expected Location After Player move
AEVec2& Player::GetCameraExpectedPosition()
{
	return expectedLocation;
}

//Get Player current direction facing
bool Player::IsPlayerFacingRight()
{
	return isFacingRight;
}

//Player Max Health
int& Player::GetMaxHealth()
{
	return maxHealth;
}
//Player Current Health

int& Player::GetCurrentHealth()
{
	return currHealth;
}

void Player::DamageToPlayer(int damageValue)
{
	if (!immortalHp)
		currHealth -= damageValue;
}


void Player::RecoverHpToPlayer(int recoveryValue)
{
	currHealth = min(currHealth + recoveryValue, maxHealth);
}

f32& Player::GetFrictionOnPlayer()
{
	return friction;
}

int& Player::GetComboState()
{
	return comboState;
}


AEVec2& Player::GetPlayerCurrentPosition()
{
	return obj.pos;
}

AEVec2& Player::GetPlayerScale()
{
	return obj.scale;
}

bool& Player::GetIsPlayerAttacking()
{
	return isAttacking;
}

bool& Player::GetIsTalkingToNpc()
{
	return isConversation;
}

void Player::SetPlayerPoisoned(bool getPoison)
{
	if (!isPoisoned)
		playerStatusEffectList.push_back({ Status_Effect_System::Status_Effect::POISON, Status_Effect_System::Status_Effect_Source::TRAPS});
	 isPoisoned = getPoison;
}

void Player::SetPlayerSlowed(bool getSlow)
{
	if (!isSlowed)
		playerStatusEffectList.push_back({ Status_Effect_System::Status_Effect::SLOW, Status_Effect_System::Status_Effect_Source::TRAPS });
	isSlowed = getSlow;
}

AABB& Player::GetPlayerCollisionBox()
{
	return collisionBox;
}

void Player::CheckPlayerGridCollision(Grids2D** gridMap, int maxRow, int maxCol)
{
	int playerIndexY = (int)((AEGfxGetWindowHeight() * 0.5f - obj.pos.y) / (gridMap[0][0].size.x));
	int rangeRow = (int)(obj.scale.x * 2 / gridMap[0][0].size.x);

	for (int i = 0; i <= rangeRow; i++)
	{
		if (playerIndexY >= maxRow)
			return;

		int playerIndexX = (int)((obj.pos.x + AEGfxGetWindowWidth() * 0.5f) / (gridMap[0][0].size.x));
		int rangeCol = (int)(obj.scale.x * 2 / gridMap[0][0].size.x);

		for (int j = 0; j <= rangeCol; j++)
		{
			if (playerIndexX >= maxCol)
				return;

			switch (gridMap[playerIndexY][playerIndexX].typeOfGrid)
			{
			case NORMAL_GROUND:
				//Collision check
				//Resolve + Vertical Collision only for entity x (wall or ground)
				//Check vertical box (Head + Feet) 
				if (AABBvsAABB(boxHeadFeet, gridMap[playerIndexY][playerIndexX].collisionBox))
				{
					collisionNormal = AABBNormalize(boxHeadFeet,
						gridMap[playerIndexY][playerIndexX].collisionBox);
					ResolveVerticalCollision(boxHeadFeet, gridMap[playerIndexY][playerIndexX].collisionBox,
						&collisionNormal, &obj.pos,
						&velocity);
				}

				//Check horizontal box (Left arm -> Right arm)
				if (AABBvsAABB(boxArms, gridMap[playerIndexY][playerIndexX].collisionBox))
				{
					collisionNormal = AABBNormalize(boxArms,
						gridMap[playerIndexY][playerIndexX].collisionBox);
					ResolveHorizontalCollision(boxArms, gridMap[playerIndexY][playerIndexX].collisionBox,
						&collisionNormal, &obj.pos,
						&velocity);
				}
				break;
			case MAP_TRANSITION_GRID_1:
				if (AABBvsAABB(collisionBox, gridMap[playerIndexY][playerIndexX].collisionBox))
				{
					//std::cout << "Collided\n";MainMenu_Song
					if (!transitionalImageOBJ.active)
					{
						switch (current)
						{
						case GameStates::GAME_LOBBY:
							transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1_A);
							break;
						case GameStates::AREA1_A:
							transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, GAME_LOBBY);
							break;
						case GameStates::AREA1_B:
							transitionalImageOBJ.PlayMapTransition(TRANSITION_UP, AREA1_A);
							break;
						case GameStates::AREA1_C:
							transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1_B);
							break;
						case GameStates::AREA1_D:
							transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1_C);
							break;
						case GameStates::AREA1_E:
							transitionalImageOBJ.PlayMapTransition(TRANSITION_RIGHT, AREA1_D);
							break;
						case GameStates::AREA1_F:
							transitionalImageOBJ.PlayMapTransition(TRANSITION_RIGHT, AREA1_E);
							break;
						default:
							break;
						}
					}
				}
				break;
			case MAP_TRANSITION_GRID_2:
				if (AABBvsAABB(collisionBox, gridMap[playerIndexY][playerIndexX].collisionBox))
				{
					//std::cout << "Collided\n";MainMenu_Song
					
					if (!transitionalImageOBJ.active)
					{
						switch (current)
						{
						case GameStates::AREA1_A:
							transitionalImageOBJ.PlayMapTransition(TRANSITION_UP, AREA1_B);
							break;
						case GameStates::AREA1_C:
							transitionalImageOBJ.PlayMapTransition(TRANSITION_RIGHT, AREA1_D);
							break;
						case GameStates::AREA1_D:
							transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1_E);
							break;
						case GameStates::AREA1_E:
							transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1_F);
							break;
						case GameStates::AREA1_F:
							transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA_BOSS);
							break;
						default:
							break;
						}
					}
				}
				break;
			case MAP_TRANSITION_GRID_3:
				if (AABBvsAABB(collisionBox, gridMap[playerIndexY][playerIndexX].collisionBox))
				{
					//std::cout << "Collided\n";MainMenu_Song
					if (!transitionalImageOBJ.active)
					{
						switch (current)
						{
						case GameStates::AREA1_B:
							transitionalImageOBJ.PlayMapTransition(TRANSITION_RIGHT, AREA1_C);
							break;
						default:
							break;
						}
					}
				}
				break;
			case MAP_TRANSITION_GRID_4:
				if (AABBvsAABB(collisionBox, gridMap[playerIndexY][playerIndexX].collisionBox))
				{
					//std::cout << "Collided\n";MainMenu_Song
					if (!transitionalImageOBJ.active)
					{
						switch (current)
						{
						case GameStates::AREA1_A:
							transitionalImageOBJ.PlayMapTransition(TRANSITION_UP, AREA1_B);
							break;
						default:
							break;
						}
					}
				}
				break;
			case LAVA_GRID:
				if (AABBvsAABB(collisionBox, gridMap[playerIndexY][playerIndexX].collisionBox))
				{
					OnPlayerDeath();
				}
				break;
			case RETURN_PORTAL_GRID:
				if (AABBvsAABB(collisionBox, gridMap[playerIndexY][playerIndexX].collisionBox) && killedBoss)
				{
					if (!transitionalImageOBJ.active)
						transitionalImageOBJ.PlayMapTransition(TRANSITION_UP, GAME_LOBBY);
				}
				break;
			case EMPTY:
				break;
			}
			playerIndexX += 1;
		}
		playerIndexY += 1;
	}
}

AABB& Player::GetPlayerBoxHeadFeet()
{
	return boxHeadFeet;
}

AABB& Player::GetPlayerBoxArm()
{
	return boxArms;
}

AEVec2& Player::GetPlayerVelocity()
{
	return velocity;
}

AEVec2& Player::GetPlayerCollisionNormal()
{
	return collisionNormal;
}

bool& Player::GetIsPlayerOnFloor()
{
	return onFloor;
}

f32& Player::GetGravityOnPlayer()
{
	return gravityForce;
}

bool& Player::GetIsPlayerKillBoss()
{
	return killedBoss;
}

bool& Player::GetPlayerJustDied()
{
	return justDied;
}

bool& Player::GetPlayerHeldCombo()
{
	return heldCombo;
}

void Player::OnPlayerDeath() {
	//Return to lobby
	if (!transitionalImageOBJ.active)
	{
		transitionalImageOBJ.PlayMapTransition(TRANSITION_UP, GAME_LOBBY);
		GetCurrentHealth() = GetMaxHealth();
		justDied = true;
	}
}

bool Player::GetDebugModeImmortal()
{
	return immortalHp;
}

void Player::SetDebugModeImmortal(bool isImmortal)
{
	immortalHp = isImmortal;
}

bool Player::GetDebugModeOverpower()
{
	return maxAttackPower;
}

void Player::SetDebugModeOverpower(bool isMaxPower)
{
	maxAttackPower = isMaxPower;
}
