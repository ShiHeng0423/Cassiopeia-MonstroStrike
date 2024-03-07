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
#define camXBoundary (250.f)
#define camFollowupSpeedX (0.05f)
// Define a clock type for high-resolution time measurement
using Clock = std::chrono::high_resolution_clock;

// Store the time point of the last input
auto lastInputTime = Clock::now();
auto currentTime = Clock::now();
auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastInputTime).count() / 1000.0; // Convert to seconds

constexpr f32 comboWindowDuration = 1.0f;
constexpr f32 PRESS_THRESHOLD = 0.5f;
//Attack hold and atack release

bool if_there_is_undealt_trigger_input = false; // identifier
bool is_released = true;

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
	AEVec2Set(&player->equippedWeapon.Scale, 20.f, 20.f);
	player->attackTime = 1.f;
	player->isAttacking = false;
	player->combo_trig= 0;
	player->comboTime = 0.0f;
	player->comboState = 0;
	//std::cout << "Player has been equipped with a " << player->equippedWeapon.name << std::endl;

	player->burningEffect = false;
	return player;

}

void PlayerUpdate(Player& player)
{

	////X-Axis control
	//bool isDashing = false;
	//const f32 dashDuration = 0.2f; 
	//const f32 dashSpeedMultiplier = 20.0f;
	//const f32 dashCooldownTime = 1.0f; 
	//f32 dashCooldown = 0.0f;

	//if (player.velocity.y < player.gravityForce + 0.001f)
	//{
	//	std::cout << "TIE\n";
	//}

	//if (player.velocity.y < player.gravityForce - 0.001f)
	//{

	//	std::cout << "HI\n";
	//}

	if (player.isFalling)
	{
		std::cout << "FELL\n";
	}

	//Just for fun
	//if (AEInputCheckTriggered(AEVK_LSHIFT) && !isDashing && dashCooldown <= 0.0f) {
		//isDashing = true;
		//dashCooldown = dashCooldownTime;

	//	//// Determine dash direction based on current movement
	//	//if (AEInputCheckCurr(AEVK_D)) {
	//	//	player.velocity.x += player.obj.speed.x * dashSpeedMultiplier * AEFrameRateControllerGetFrameTime();
	//	//	player.isFacingRight = true;
	//	//}
	//	//else if (AEInputCheckCurr(AEVK_A)) {
	//	//	player.velocity.x -= player.obj.speed.x * dashSpeedMultiplier * AEFrameRateControllerGetFrameTime();
	//	//	player.isFacingRight = false;
	//	//}
	//}
	//else
	//{
		if (AEInputCheckCurr(AEVK_D)) {
			player.velocity.x += player.obj.speed.x;
			player.isFacingRight = true;
		}
		else if (AEInputCheckCurr(AEVK_A)) {
			player.velocity.x -= player.obj.speed.x;
			player.isFacingRight = false;
		}
	//}

	// Apply velocity constraints
	player.velocity.x = AEClamp(player.velocity.x, -5.f, 5.f);

	// Update dash cooldown
	//if (dashCooldown > 0.0f) {
	//	dashCooldown -= (f32)AEFrameRateControllerGetFrameTime();
	//	if (dashCooldown <= 0.0f) {
	//		isDashing = false;
	//	}
	//}


	// Calculate the desired location
	AEVec2 desiredLocation{ player.velocity.x * player.lookAheadMutliplier *AEFrameRateControllerGetFrameTime() , 0.f};
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
		armor.armorType = Armor::Type::First;
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
		armor.armorType = Armor::Type::Second;
		armor.leather.defense = 50;

		player.equippedArmor = armor;
		std::string armorName = "Steel Armor";

		std::cout << "Equipped " << armorName << "!" << std::endl;
	}

	// End of armor equip

	//For jumping
	if (AEInputCheckTriggered(VK_SPACE) && player.onFloor)
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

	//Weapon hit box update only
	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		triggeredTime = Clock::now();
		if_there_is_undealt_trigger_input = true;
		is_released = false;
	}
	if (AEInputCheckReleased(AEVK_LBUTTON))
	{
		is_released = true;
	}

	if (if_there_is_undealt_trigger_input)
	{
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - triggeredTime).count() / 1000.0; // Convert to seconds
		if (elapsedTime >= PRESS_THRESHOLD && !is_released)
		{
			if (player.comboState == 1) //held
			{
				//std::cout << "Triggered and not released" << std::endl;
				//std::cout << "Combo state is 1" << std::endl;

				std::cout << "hold" << std::endl;
				player.attackTime -= (f32)AEFrameRateControllerGetFrameTime() * 100.f;
				f32 attackProgress = 1.0f - (player.attackTime / comboWindowDuration);
				UpdateWeaponHitBoxHeld(&player, player.isFacingRight, &player.equippedWeapon, attackProgress);
				player.comboTime = 0.0f; // Reset combo time
				player.comboState = 0;   // Reset combo state

			}

		}
		if (elapsedTime < PRESS_THRESHOLD && is_released) //Trigger
		{
			//code will come here once per each trigger
			//but you want this part of code to be called til the attack is done.
			//then you need to make separate function and call it on here.

			//Only happens in 1 frame

			player.attackTime -= (f32)AEFrameRateControllerGetFrameTime() * 3.f; //Constant here is speed scaling
			f32 attackProgress = 1.0f - (player.attackTime / comboWindowDuration);
			UpdateWeaponHitBoxTrig(&player, player.isFacingRight, &player.equippedWeapon, attackProgress);

			if (player.comboState != 3)
			{
				player.comboState++;
				player.comboTime += elapsedTime;
				std::cout << "Left mouse button triggered for " << elapsedTime << " seconds." << std::endl;
				std::cout << "combo attack " + player.comboState << std::endl;
			}

			else
			{
				player.comboState = 0;
				player.comboTime = 0.0f;
				std::cout << "Left mouse button triggered for " << elapsedTime << " seconds." << std::endl;

				////
				comboTime = Clock::now();
				if_there_is_undealt_trigger_input = false;

			}


		}
		//reset
		if (!if_there_is_undealt_trigger_input && player.comboState > 1)
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


#pragma region Camera Section
	//Camera region
	AEVec2 cam;
	AEGfxGetCamPosition(&cam.x, &cam.y);

	//150.f refers to the cam boundary;
	if ((player.expectedLocation.x > cam.x + camXBoundary) && player.isFacingRight)
	{
		AEVec2 desiredCamLocation{ cam.x + camXBoundary,0.f };
		AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &player.expectedLocation, camFollowupSpeedX);
		AEGfxSetCamPosition(desiredCamLocation.x - camXBoundary, cam.y);
	}
	else if ((player.expectedLocation.x < cam.x - camXBoundary) && !player.isFacingRight)
	{
		AEVec2 desiredCamLocation{ cam.x - camXBoundary,0.f };
		AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &player.expectedLocation, camFollowupSpeedX);
		AEGfxSetCamPosition(desiredCamLocation.x + camXBoundary, cam.y);
	}
#pragma endregion
}
