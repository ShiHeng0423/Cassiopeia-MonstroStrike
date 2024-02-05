#include "Player.h"
#include "AEEngine.h"
#include "Armor_add.h"
#include "Weapon.h"
#include <iostream>
#include "Physics.h"
#include "Armor_add.h"
#include "Weapon.h"
#include <iostream>
#define camXBoundary (250.f)
#define camFollowupSpeedX (0.05f)


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

	std::cout << "Player has been equipped with a " << player->equippedWeapon.name << std::endl;
	return player;
}

void PlayerUpdate(Player& player)
{
	//X-Axis control
	if (AEInputCheckCurr(AEVK_D))
	{
		// Move right
		player.velocity.x += 20.f * AEFrameRateControllerGetFrameTime();
		player.isFacingRight = true;
	}
	else if (AEInputCheckCurr(AEVK_A))
	{
		// Move left
		player.velocity.x -= 20.f * AEFrameRateControllerGetFrameTime();
		player.isFacingRight = false;
	}

	// Apply velocity constraints
	player.velocity.x = AEClamp(player.velocity.x, -10.f, 10.f);

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
	player.obj.pos.y += player.velocity.y * AEFrameRateControllerGetFrameTime();
	player.obj.pos.x += player.velocity.x;

	//Resetting main AABB box...
	player.collisionBox.minimum.x = player.obj.pos.x - player.obj.img.scale.x * 0.25f;
	player.collisionBox.minimum.y = player.obj.pos.y - player.obj.img.scale.y * 0.25f;
	player.collisionBox.maximum.x = player.obj.pos.x + player.obj.img.scale.x * 0.25f;
	player.collisionBox.maximum.y = player.obj.pos.y + player.obj.img.scale.y * 0.25f;

	//Making a cross...

	//Vertical
	player.boxHeadFeet = player.collisionBox; // Get original collision box size
	player.boxHeadFeet.minimum.y -= player.obj.img.scale.y * 0.25f;
	player.boxHeadFeet.maximum.y += player.obj.img.scale.y * 0.25f;

	//Horizontal
	player.boxArms = player.collisionBox;
	player.boxArms.minimum.x -= player.obj.img.scale.x * 0.25f;
	player.boxArms.maximum.x += player.obj.img.scale.x * 0.25f;
	//Update player weapon hit box

	if (player.isAttacking)
	{
		if (player.attackTime > 0)
		{
			player.attackTime -= AEFrameRateControllerGetFrameTime();
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