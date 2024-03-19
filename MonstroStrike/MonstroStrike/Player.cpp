#include "Player.h"
#include "AEEngine.h"
#include "Physics.h"
#include "Armor_add.h"
#include "Weapon.h"
#include "TriggerAttack.h"
#include <iostream>
#include <chrono>
#include <functional> 
#define CAM_X_BOUNDARY (250.f)
#define CAM_FOLLOW_UP_SPEED_X (0.05f)
// Define a clock type for high-resolution time measurement
using Clock = std::chrono::high_resolution_clock;

// Store the time point of the last input
auto lastInputTime = Clock::now();
auto currentTime = Clock::now();
auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastInputTime).count() / 1000.0;
// Convert to seconds

constexpr f32 comboWindowDuration = 1.0f;
constexpr f32 PRESS_THRESHOLD = 0.5f;
//Attack hold and atack release

bool undealtTriggerInput = false; // identifier
bool isReleased = true;
bool if_first_input = false;


bool isFirstInput = true;
bool delayFlag = false;
bool isTriggerAttacking = false;
auto triggeredTime = Clock::now();
auto releasedTime = Clock::now();
auto comboTime = Clock::now();
auto delayTime = Clock::now();
float threshold = 2.0f; //Testing purposes. Change during creation threshold


Player* PlayerInitialize(const char* filename, AEVec2 scale, AEVec2 location, AEVec2 speed, bool isFacingRight)
{
	Player *player = new Player;
	player->obj.img.pTex = AEGfxTextureLoad(filename);
	player->obj.speed = speed;

	AEVec2Set(&player->obj.pos, location.x, location.y);
	AEVec2Set(&player->velocity, 0.f, 0.f); //Begin with no velocity
	AEVec2Set(&player->obj.img.scale, scale.x, scale.y);
	AEVec2Set(&player->expectedLocation, 0.f, 0.f);

	player->isFacingRight = true;
	player->lookAheadMutliplier = 50.f;
	player->onFloor = true; //Set as false first, will be set as true when ground detected
	player->isFalling = false;
	player->mass = 60.f;

	//Initializing collision box starting position
	player->collisionBox.minimum.x = player->obj.pos.x - player->obj.img.scale.x * 0.5f;
	player->collisionBox.minimum.y = player->obj.pos.y - player->obj.img.scale.y * 0.5f;
	player->collisionBox.maximum.x = player->obj.pos.x + player->obj.img.scale.x * 0.5f;
	player->collisionBox.maximum.y = player->obj.pos.y + player->obj.img.scale.y * 0.5f;

	AEVec2Set(&player->boxArms.maximum, 0.f, 0.f);
	AEVec2Set(&player->boxHeadFeet.maximum, 0.f, 0.f);
	AEVec2Set(&player->collisionNormal, 0.f, 0.f);

	player->equippedWeapon = createWeapon("Sword", location.x,location.y);
	//
	player->attackTime = 1.f;
	player->isAttacking = false;
	player->comboTrig = 0;
	player->comboTime = 0.0f;
	player->comboState = 0;
	//std::cout << "Player has been equipped with a " << player->equippedWeapon.name << std::endl;

	//Initializes weapon's position

	//// change the hitbox
	//AEVec2Set(&player->equippedWeapon->Scale, 30.f, 20.f);
	player->burningEffect = false;


	//Player Stats
	f32 maxHealth = 500.f;
	f32 currHealth = maxHealth;
	f32 attack = 100.f;
	f32 defence = 50.f;

	return player;

}


void PlayerUpdate(Player& player, bool isInventoryOpen)
{
	if (AEInputCheckTriggered(AEVK_B))
	{
		player.equippedWeapon = createWeapon("Short-Sword", player.expectedLocation.x, player.expectedLocation.y);
		std::cout << "Now equipped with a " << player.equippedWeapon->name << std::endl;

<<<<<<< Updated upstream
	}
	
=======
	//This is the update for weapon equipped

	if (AEInputCheckTriggered(AEVK_B))
	{
		player.equippedWeapon = createWeapon("Short-Sword", player.expectedLocation.x, player.expectedLocation.y);
		std::cout << "Now equipped with a " << player.equippedWeapon->name << std::endl;

	}
>>>>>>> Stashed changes
	if (AEInputCheckTriggered(AEVK_N))
	{
		player.equippedWeapon = createWeapon("Broad-Sword", player.expectedLocation.x, player.expectedLocation.y);
		std::cout << "Now equipped with a " << player.equippedWeapon->name << std::endl;
	}
	if (AEInputCheckTriggered(AEVK_M))
	{
		player.equippedWeapon = createWeapon("GreatSword", player.expectedLocation.x, player.expectedLocation.y);
		std::cout << "Now equipped with a " << player.equippedWeapon->name << std::endl;
	}
<<<<<<< Updated upstream
=======


>>>>>>> Stashed changes
	if (player.isFalling)
	{
		std::cout << "FELL\n";
	}

	if (AEInputCheckCurr(AEVK_D) && !isInventoryOpen)
	{
		player.velocity.x += player.obj.speed.x * (f32)AEFrameRateControllerGetFrameTime();
		player.isFacingRight = true;
	}
	else if (AEInputCheckCurr(AEVK_A) && !isInventoryOpen)
	{
		player.velocity.x -= player.obj.speed.x * (f32)AEFrameRateControllerGetFrameTime();
		player.isFacingRight = false;
	}
	//}

	// Apply velocity constraints
	player.velocity.x = AEClamp(player.velocity.x, -5.f, 5.f);


	// Calculate the desired location
	AEVec2 desiredLocation{player.velocity.x * player.lookAheadMutliplier, 0.f};
	AEVec2Add(&player.expectedLocation, &player.obj.pos, &desiredLocation);

	//For friction
	if (player.onFloor) //Means confirm on floor
	{
		player.velocity.x *= 0.85f; //Friction application
	}

	//Start of armor equip
	if (AEInputCheckTriggered(AEVK_1))
	{
		Player player;
		Armor armor;
		armor.armorType = Armor::Type::FIRST;
		armor.leather.defense = 20;

		player.equippedArmor = armor;
		std::string armorName = "Leather Armor";

		std::cout << "Equipped " << armorName << "!" << std::endl;
	}

	//std::cout << "FPS: " << AEFrameRateControllerGetFrameRate() << std::endl;
	if (AEInputCheckTriggered(AEVK_2))
	{
		Player player;
		Armor armor;
		armor.armorType = Armor::Type::SECOND;
		armor.leather.defense = 50;

		player.equippedArmor = armor;
		std::string armorName = "Steel Armor";

		std::cout << "Equipped " << armorName << "!" << std::endl;
	}

	// End of armor equip

	//For jumping
	if (AEInputCheckTriggered(VK_SPACE) && player.onFloor && !isInventoryOpen)
	{
		player.onFloor = false;
		player.velocity.y = 700.f;
	}

	ApplyGravity(&player.velocity, player.mass, &player.onFloor, &player.gravityForce, &player.isFalling);


	player.prevPos = player.obj.pos;
	player.prevcollisionBox = player.collisionBox;
	player.obj.pos.y += player.velocity.y * (f32)AEFrameRateControllerGetFrameTime();
	player.obj.pos.x += player.velocity.x;

	//Resetting main AABB box...
	player.collisionBox.minimum.x = player.obj.pos.x - player.obj.img.scale.x * 0.5f;
	player.collisionBox.minimum.y = player.obj.pos.y - player.obj.img.scale.y * 0.5f;
	player.collisionBox.maximum.x = player.obj.pos.x + player.obj.img.scale.x * 0.5f;
	player.collisionBox.maximum.y = player.obj.pos.y + player.obj.img.scale.y * 0.5f;

	//Vertical
	f32 verticalOffset = player.obj.img.scale.y * 0.025f;

	// Vertical box
	player.boxHeadFeet = player.collisionBox;
	player.boxHeadFeet.minimum.y -= verticalOffset;
	player.boxHeadFeet.maximum.y += verticalOffset;

	f32 horizontalOffset = player.obj.img.scale.x * 0.025f;

	// Horizontal box
	player.boxArms = player.collisionBox;
	player.boxArms.minimum.x -= horizontalOffset;
	player.boxArms.maximum.x += horizontalOffset;

	//std::cout << "Collision Normal Y: " << player.collisionNormal.y << std::endl;
	//Update player weapon hit box

	/*Weapon hit box update only*/


	if (AEInputCheckTriggered(AEVK_LBUTTON) && !isInventoryOpen)
	{
		triggeredTime = Clock::now();
		undealtTriggerInput = true;
		isReleased = false;
		if_first_input = true;

	}
	if (AEInputCheckReleased(AEVK_LBUTTON) && !isInventoryOpen)
	{
		isReleased = true;
	}
	//reset
	if (!undealtTriggerInput)
	{
		elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - comboTime).count() / 1000.0; // Convert to seconds
		if (elapsedTime > comboWindowDuration)
		{
			player.isAttacking = false;
			player.equippedWeapon->weaponHIT = false;
			player.comboTime = 0.0f; // Reset combo time
			player.comboState = 0;   // Reset combo state

		}

	}
	if (undealtTriggerInput)
	{
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - triggeredTime).count() /
			1000.0; // Convert to seconds
		if (elapsedTime >= PRESS_THRESHOLD && !isReleased)
		{
			if (player.comboState == 2) //held
			{

				f32 attackProgress = 1.0f - (player.attackTime / comboWindowDuration);
				UpdateWeaponHitBoxHeld(&player, player.isFacingRight, player.equippedWeapon, attackProgress);
				player.isAttacking = true;



			}
			comboTime = Clock::now();
			undealtTriggerInput = false;

		}
		if (elapsedTime < PRESS_THRESHOLD && isReleased) //Trigger (Here is flag for initialization)
		{

			f32 attackProgress = 1.0f - (player.attackTime / comboWindowDuration);
			UpdateWeaponHitBoxTrig(&player, player.isFacingRight, player.equippedWeapon, attackProgress);
			player.isAttacking = true;

			if (player.comboState < 2)
			{

				player.comboState++;
				player.comboTime += (float)elapsedTime;

			}

			else
			{

				player.comboState = 0;
				player.comboTime = 0.0f;


			}
			comboTime = Clock::now();
			undealtTriggerInput = false;


		}
	}
	

<<<<<<< Updated upstream
	
	


#pragma region Camera Section
	//Camera region
	//AEVec2 cam;
	//AEGfxGetCamPosition(&cam.x, &cam.y);

	////150.f refers to the cam boundary;
	//if ((player.expectedLocation.x > cam.x + CAM_X_BOUNDARY) && player.isFacingRight)
	//{
	//	AEVec2 desiredCamLocation{cam.x + CAM_X_BOUNDARY, 0.f};
	//	AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &player.expectedLocation, CAM_FOLLOW_UP_SPEED_X);
	//	AEGfxSetCamPosition(desiredCamLocation.x - CAM_X_BOUNDARY, cam.y);
	//}
	//else if ((player.expectedLocation.x < cam.x - CAM_X_BOUNDARY) && !player.isFacingRight)
	//{
	//	AEVec2 desiredCamLocation{cam.x - CAM_X_BOUNDARY, 0.f};
	//	AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &player.expectedLocation, CAM_FOLLOW_UP_SPEED_X);
	//	AEGfxSetCamPosition(desiredCamLocation.x + CAM_X_BOUNDARY, cam.y);
	//}
#pragma endregion
=======
>>>>>>> Stashed changes
}
