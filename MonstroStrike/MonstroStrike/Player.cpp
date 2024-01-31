#include "Player.h"
#include "AEEngine.h"
#include "Physics.h"

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
	player->canJump = false; //Set as false first, will be set as true when ground detected
	player->mass = 80.f;

	//Initializing collision box starting position
	player->collisionBox.minimum.x = player->obj.pos.x - player->obj.img.scale.x * 0.5f;
	player->collisionBox.minimum.y = player->obj.pos.y - player->obj.img.scale.y * 0.5f;
	player->collisionBox.maximum.x = player->obj.pos.x + player->obj.img.scale.x * 0.5f;
	player->collisionBox.maximum.y = player->obj.pos.y + player->obj.img.scale.y * 0.5f;

	return player;
}

void PlayerUpdate(Player& player)
{
	if (AEInputCheckCurr(AEVK_D))
	{
		player.obj.speed.x += AEFrameRateControllerGetFrameTime() * 5.f;
		player.obj.speed.x = AEClamp(player.obj.speed.x, -2.f, 2.f);
		player.isFacingRight = true;

		AEVec2 desiredLocation{ player.obj.speed.x * player.lookAheadMutliplier,0.f };
		AEVec2Add(&player.expectedLocation, &player.obj.pos, &desiredLocation);
	}
	else if (AEInputCheckCurr(AEVK_A))
	{
		player.obj.speed.x -= AEFrameRateControllerGetFrameTime() * 5.f;
		player.obj.speed.x = AEClamp(player.obj.speed.x, -2.f, 2.f);
		player.isFacingRight = false;

		AEVec2 desiredLocation{ player.obj.speed.x * player.lookAheadMutliplier,0.f };
		AEVec2Add(&player.expectedLocation, &player.obj.pos, &desiredLocation);
	}
	else
	{
		player.obj.speed.x -= player.obj.speed.x;
		AEVec2Lerp(&player.expectedLocation, &player.expectedLocation, &player.obj.pos, 0.01f);
	}
	player.obj.pos.x += player.obj.speed.x;
	//How the player can check to jump
	if (AEInputCheckTriggered(VK_SPACE) && player.canJump)
	{
		player.canJump = false;
		player.velocity.y = 400.f;
		std::cout << "Player jumped" << std::endl;
	}
	ApplyGravity(&player.velocity, player.mass); //Velocity passed in must be modifiable, mass can be adjusted if needed to
	player.obj.pos.y += player.velocity.y * AEFrameRateControllerGetFrameTime();

	//Resetting main AABB box...
	player.collisionBox.minimum.x = player.obj.pos.x - player.obj.img.scale.x * 0.5f;
	player.collisionBox.minimum.y = player.obj.pos.y - player.obj.img.scale.y * 0.5f;
	player.collisionBox.maximum.x = player.obj.pos.x + player.obj.img.scale.x * 0.5f;
	player.collisionBox.maximum.y = player.obj.pos.y + player.obj.img.scale.y * 0.5f;

	//Making a cross...

	//Vertical
	player.boxHeadFeet = player.collisionBox; // Get original collision box size
	player.boxHeadFeet.minimum.y -= player.obj.img.scale.y * 0.0001f;
	player.boxHeadFeet.maximum.y += player.obj.img.scale.y * 0.0001f;

	//Horizontal
	player.boxArms = player.collisionBox;
	player.boxArms.minimum.x -= player.obj.img.scale.x * 0.0001f;
	player.boxArms.maximum.x += player.obj.img.scale.x * 0.0001f;


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