#include "Player.h"
#include "AEEngine.h"
#include "Armor_add.h"
#include "Weapon.h"
#include <iostream>
#include "Physics.h"
#include "Armor_add.h"
#include "Weapon.h"
#include <iostream>


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
	player->onFloor = false; //Set as false first, will be set as true when ground detected
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

	////Just for fun
	//if (AEInputCheckTriggered(AEVK_LSHIFT) && !isDashing && dashCooldown <= 0.0f) {
	//	//isDashing = true;
	//	//dashCooldown = dashCooldownTime;

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
			player.velocity.x += player.obj.speed.x * (f32)AEFrameRateControllerGetFrameTime();
			player.isFacingRight = true;
		}
		else if (AEInputCheckCurr(AEVK_A)) {
			player.velocity.x -= player.obj.speed.x * (f32)AEFrameRateControllerGetFrameTime();
			player.isFacingRight = false;
		}
	//}

	// Apply velocity constraints
	player.velocity.x = AEClamp(player.velocity.x, -10.f, 10.f);

	// Update dash cooldown
	//if (dashCooldown > 0.0f) {
	//	dashCooldown -= (f32)AEFrameRateControllerGetFrameTime();
	//	if (dashCooldown <= 0.0f) {
	//		isDashing = false;
	//	}
	//}


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
		armor.armorType = Armor::Type::First;
		armor.leather.defense = 20;

		player.equippedArmor = armor;
		std::string armorName = "Leather Armor";

		std::cout << "Equipped " << armorName << "!" << std::endl;
	}

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
		player.velocity.y = 400.f;
	}
	ApplyGravity(&player.velocity, player.mass); //Velocity passed in must be modifiable, mass can be adjusted if needed to

	//Player position update
	player.obj.pos.y += player.velocity.y * (f32)AEFrameRateControllerGetFrameTime();
	player.obj.pos.x += player.velocity.x;

	//Resetting main AABB box...
	player.collisionBox.minimum.x = player.obj.pos.x - player.obj.img.scale.x * 0.5f;
	player.collisionBox.minimum.y = player.obj.pos.y - player.obj.img.scale.y * 0.5f;
	player.collisionBox.maximum.x = player.obj.pos.x + player.obj.img.scale.x * 0.5f;
	player.collisionBox.maximum.y = player.obj.pos.y + player.obj.img.scale.y * 0.5f;

	//Vertical
	f32 verticalOffset = player.obj.img.scale.y * 0.01f;

	// Vertical box
	player.boxHeadFeet = player.collisionBox;
	player.boxHeadFeet.minimum.y -= verticalOffset;
	player.boxHeadFeet.maximum.y += verticalOffset;

	f32 horizontalOffset = player.obj.img.scale.x * 0.01f;

	// Horizontal box
	player.boxArms = player.collisionBox;
	player.boxArms.minimum.x -= horizontalOffset;
	player.boxArms.maximum.x += horizontalOffset;

	//Update player weapon hit box

	//Weapon hit box update only
	if (player.isAttacking)
	{
		if (player.attackTime > 0)
		{
			player.attackTime -= (f32)AEFrameRateControllerGetFrameTime();
			f32 attackProgress = 1.0f - (player.attackTime / 1.0f);
			UpdateWeaponHitBox(&player, player.isFacingRight, &player.equippedWeapon, attackProgress);
		}
		else //Reach 0
		{
			player.attackTime = 1.f;
			player.isAttacking = false;
			player.equippedWeapon.weaponHIT = false;
		}
	}
	else
	{
		player.equippedWeapon.position.x = player.obj.pos.x;
		player.equippedWeapon.position.y = player.obj.pos.y + player.obj.img.scale.y * 0.5f;
	}
}