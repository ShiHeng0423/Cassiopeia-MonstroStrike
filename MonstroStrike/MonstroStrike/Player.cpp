#include "Player.h"
#include "AEEngine.h"
#include "Physics.h"
#include "Armor_add.h"
#include "Weapon.h"
#include "TriggerAttack.h"
#include <iostream>
#include <chrono>
#include <queue>
#include <functional>  // for std::function
#define CAM_X_BOUNDARY (250.f)
#define CAM_FOLLOW_UP_SPEED_X (0.05f)
// Define a clock type for high-resolution time measurement
using Clock = std::chrono::high_resolution_clock;

// Store the time point of the last input
auto lastInputTime = Clock::now();
auto currentTime = Clock::now();
auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastInputTime).count() / 1000.0; // Convert to seconds

constexpr f32 comboWindowDuration = 1.0f;
constexpr f32 PRESS_THRESHOLD = 0.5f;
//Attack hold and atack release

bool undealtTriggerInput = false; // identifier
bool isReleased = true;
bool if_first_input = false;

using AnimationFunction = std::function<void(Player&, float)>;

auto triggeredTime = Clock::now();
auto releasedTime = Clock::now();
auto comboTime = Clock::now();

#pragma region AnimationQueue

//std::queue<anima> con_anima;
//
//class anima
//{
// public:
//	bool is_playing = false;
//	
//
//};


#pragma endregion



Player* PlayerInitialize(const char* filename, AEVec2 scale ,AEVec2 location, AEVec2 speed, bool isFacingRight)
{
	//for every anima in the con_anima
	//if there is an anima in the con_anima
	//execute the function pointer (play_animation)

	Player *player = new Player;
	player->obj.img.pTex = AEGfxTextureLoad(filename);
	player->obj.speed = speed;

	AEVec2Set(&player->obj.pos, location.x, location.y);
	AEVec2Set(&player->velocity, 0.f, 0.f); //Begin with no velocity
	AEVec2Set(&player->obj.img.scale, scale.x, scale.y);
	AEVec2Set(&player->expectedLocation, 0.f, 0.f);

	player->isFacingRight = isFacingRight;
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
	AEVec2Set(&player->equippedWeapon.scale, 20.f, 20.f);
	player->attackTime = 1.f;
	player->isAttacking = false;
	player->comboTrig= 0;
	player->comboTime = 0.0f;
	player->comboState = 0;
	//std::cout << "Player has been equipped with a " << player->equippedWeapon.name << std::endl;

	player->burningEffect = false;
	player->isConversation = false;


	//Player Stats
	f32 maxHealth = 500.f;
	f32 currHealth = maxHealth;
	f32 attack = 100.f;
	f32 defence = 50.f;

	return player;

}


void PlayerUpdate(Player& player, bool isInventoryOpen)
{

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

	// Apply velocity constraints
	player.velocity.x = AEClamp(player.velocity.x, -5.f, 5.f);


	// Calculate the desired location
	AEVec2 desiredLocation{ player.velocity.x * player.lookAheadMutliplier , 0.f };
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

	ApplyGravity(&player.velocity, player.mass, &player.onFloor, &player.gravityForce, &player.isFalling); //Velocity passed in must be modifiable, mass can be adjusted if needed to
	//
	//std::cout << "Player on floor: " << player.onFloor << std::endl;
	//std::cout << "Player vel y: " << fabsf(player.velocity.y) << std::endl;
	//std::cout << "Player gravity force: " << player.gravityForce << std::endl;

	//Player position update

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

	if (undealtTriggerInput)
	{
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - triggeredTime).count() / 1000.0; // Convert to seconds
		if (elapsedTime >= PRESS_THRESHOLD && !isReleased)
		{
			if (player.comboState == 1) //held
			{
				//std::cout << "Triggered and not released" << std::endl;
				//std::cout << "Combo state is 1" << std::endl;

				std::cout << "hold" << std::endl;
				player.attackTime -= AEFrameRateControllerGetFrameTime() * 100.f;
				f32 attackProgress = 1.0f - (player.attackTime / comboWindowDuration);
				UpdateWeaponHitBoxHeld(&player, player.isFacingRight, &player.equippedWeapon, attackProgress);
				player.comboTime = 0.0f; // Reset combo time
				player.comboState = 0;   // Reset combo state

			}

		}
		if (elapsedTime < PRESS_THRESHOLD && isReleased) //Trigger
		{
			//code will come here once per each trigger
			//but you want this part of code to be called til the attack is done.
			//then you need to make separate function and call it on here.

			//Only happens in 1 frame

			player.attackTime -= AEFrameRateControllerGetFrameTime() * 3.f; //Constant here is speed scaling
			f32 attackProgress = 1.0f - (player.attackTime / comboWindowDuration);
			UpdateWeaponHitBoxTrig(&player, player.isFacingRight, &player.equippedWeapon, attackProgress);

			if (player.comboState != 3)
			{
				player.comboState++;
				player.comboTime += (float)elapsedTime;

			}

			else
			{
				player.comboState = 0;
				player.comboTime = 0.0f;
				std::cout << "Left mouse button triggered for " << elapsedTime << " seconds." << std::endl;

				////
				comboTime = Clock::now();
				undealtTriggerInput = false;

			}


		}
	}
	//reset
	if (!undealtTriggerInput && player.comboState > 1)
	{
		elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - comboTime).count() / 1000.0; // Convert to seconds
		if (elapsedTime > comboWindowDuration)
		{
			player.isAttacking = false;
			player.equippedWeapon.weaponHIT = false;
			player.comboTime = 0.0f; // Reset combo time
			player.comboState = 0;   // Reset combo state
			std::cout << "resetting combo" << std::endl;
		}

	}
	//else
	//{
	//	// Combo window expired + hold window
	//	player.isAttacking = false;
	//	player.equippedWeapon.weaponHIT = false;
	//	player.comboTime = 0.0f; // Reset combo time
	//	player.comboState = 0;   // Reset combo state

	//}


	//
	//else
	//{
	//	// No input, reset weapon position
	//	player.equippedWeapon.position.x = player.obj.pos.x;
	//	player.equippedWeapon.position.y = player.obj.pos.y + player.obj.img.scale.y * 0.5f;
	//	player.attackTime = 1.f;
	//}

	//if (holding_down)
	//{
	//	//auto currentTime = Clock::now();
	//	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastInputTime).count() / 1000.0; // Convert to seconds

	//	lastInputTime = currentTime; // Update last input time
	//	// Update hit box triggered
	//	if (player.comboTime <= comboWindowDuration)
	//	{
	//		// Combo is within the window
	//		player.attackTime -= AEFrameRateControllerGetFrameTime() * 3.f; //Constant here is speed scaling
	//		f32 attackProgress = 1.0f - (player.attackTime / 1.f);
	//		UpdateWeaponHitBoxTrig(&player, player.isFacingRight, &player.equippedWeapon, attackProgress);
	//		// Use elapsedTime for all time-related calculations
	//		
	//		if (player.comboState != 3)
	//		{
	//			player.comboState++;
	//			player.comboTime += elapsedTime;
	//			std::cout << "Left mouse button triggered for " << elapsedTime << " seconds." << std::endl;
	//			holding_down = false;
	//		}

	//		else
	//		{
	//			player.comboState = 0;
	//			player.comboTime = 0.0f;
	//			std::cout << "Left mouse button triggered for " << elapsedTime << " seconds." << std::endl;
	//			holding_down = false;
	//		}


	//	}
	//	else if (player.comboTime > comboWindowDuration * 2 / 3 )
	//	{
	//		std::cout << "Left mouse button held for " << elapsedTime << " seconds." << std::endl;
	//		player.isAttacking = false;
	//		player.equippedWeapon.weaponHIT = false;
	//		player.comboTime = 0.0f; // Reset combo time
	//		player.comboState = 0;   // Reset combo state



	//	}
	//	else
	//	{
	//		// Combo window expired
	//		player.isAttacking = false;
	//		player.equippedWeapon.weaponHIT = false;
	//		player.comboTime = 0.0f; // Reset combo time
	//		player.comboState = 0;   // Reset combo state

	//	}
	//}
	//else
	//{
	//		// No input, reset weapon position
	//		player.equippedWeapon.position.x = player.obj.pos.x;
	//		player.equippedWeapon.position.y = player.obj.pos.y + player.obj.img.scale.y * 0.5f;
	//		player.attackTime = 1.f;
	//}
}
