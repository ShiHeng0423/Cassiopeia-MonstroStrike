#include "Player.h"
#include "AEEngine.h"

#define camXBoundary (250.f)
#define camFollowupSpeedX (0.05f)


Player* PlayerInitialize(const char* filename, AEVec2 scale ,AEVec2 location, AEVec2 speed, bool isFacingRight)
{
	Player *player = new Player;
	player->obj.img.pTex = AEGfxTextureLoad(filename);
	player->obj.speed = speed;

	AEVec2Set(&player->obj.pos, location.x, location.y);

	AEVec2Set(&player->obj.img.scale, scale.x, scale.y);
	AEVec2Set(&player->expectedLocation, 0.f, 0.f);

	player->isFacingRight = isFacingRight;
	player->lookAheadMutliplier = 50.f;

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
}