#include "Camera.h"

#define camXBoundary (500.f)
#define camYBoundary (10.f)
#define camFollowupSpeedX (0.05f)

Camera::Camera(AEVec2 player)
{
	AEInputGetCursorPosition(&this->screen_x, &this->screen_y);
	this->screen_x = this->screen_x - AEGfxGetWindowWidth() * 0.5f;
	this->screen_y = AEGfxGetWindowHeight() * 0.5f - this->screen_y;
	this->world_coordinate = player;
	AEGfxSetCamPosition(this->world_coordinate.x, this->world_coordinate.y);
	look_ahead.x = look_ahead.y = 0;

	lookahead = false;
	lookback = false;
	cameraOnHold = false;
	shake_timer = 0.f; 
	lookback_timer = 0.f;
}

void Camera::UpdatePos(Player player)
{
	AEInputGetCursorPosition(&this->screen_x, &this->screen_y);
	this->screen_x = this->screen_x - AEGfxGetWindowWidth() * 0.5f;
	this->screen_y = AEGfxGetWindowHeight() * 0.5f - this->screen_y;

	if (lookahead)
	{
		AEVec2 desiredCamLocation{ 0,0 };
		AEGfxGetCamPosition(&desiredCamLocation.x, &desiredCamLocation.y);
		AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &look_ahead, 0.2f);
		AEGfxSetCamPosition(desiredCamLocation.x, desiredCamLocation.y);
		if (AEVec2Distance(&desiredCamLocation, &look_ahead) < 1.0f && lookback_timer <= 0.f && !cameraOnHold)
		{
			lookback_timer = 5.0f;
			cameraOnHold = true;
		}
		else
		{
			lookback_timer -= AEFrameRateControllerGetFrameTime();
			if (cameraOnHold && lookback_timer <= 0.f)
			{
				cameraOnHold = false;
				lookahead = false;
				lookback = true;
			}
		}
	}
	else if (lookback)
	{
		AEVec2 desiredCamLocation{ 0,0 };
		AEGfxGetCamPosition(&desiredCamLocation.x, &desiredCamLocation.y);
		AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &this->world_coordinate, 0.05f);
		AEGfxSetCamPosition(desiredCamLocation.x, desiredCamLocation.y);
		if (AEVec2Distance(&desiredCamLocation, &this->world_coordinate) < 1.0f)
		{
			lookback = false;
		}
	}
	else
	{
		shake_timer -= AEFrameRateControllerGetFrameTime();
		if (shake_timer > 0.f)
		{
			f32 randX = (AERandFloat() - 0.5f) * 100.f;
			f32 randY = (AERandFloat() - 0.5f) * 100.f;
			AEVec2 desiredCamLocation{ this->world_coordinate.x + randX,this->world_coordinate.y + randY };
			AEVec2Lerp(&this->world_coordinate, &this->world_coordinate, &desiredCamLocation, 0.05f);
		}

		if ((player.expectedLocation.x > this->world_coordinate.x + camXBoundary) && player.isFacingRight)
		{
			AEVec2 desiredCamLocation{ this->world_coordinate.x + camXBoundary,this->world_coordinate.y };
			AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &player.expectedLocation, camFollowupSpeedX);
			this->world_coordinate.x = desiredCamLocation.x - camXBoundary;
		}
		else if ((player.expectedLocation.x < this->world_coordinate.x - camXBoundary) && !player.isFacingRight)
		{
			AEVec2 desiredCamLocation{ this->world_coordinate.x - camXBoundary,this->world_coordinate.y };
			AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &player.expectedLocation, camFollowupSpeedX);
			this->world_coordinate.x = desiredCamLocation.x + camXBoundary;
		}
		if (player.expectedLocation.y > this->world_coordinate.y + camYBoundary)
		{
			AEVec2 desiredCamLocation{ this->world_coordinate.x, this->world_coordinate.y + camYBoundary };
			AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &player.expectedLocation, camFollowupSpeedX);
			this->world_coordinate.y = desiredCamLocation.y - camYBoundary;
		}
		else if (player.expectedLocation.y < this->world_coordinate.y - camYBoundary)
		{
			AEVec2 desiredCamLocation{ this->world_coordinate.x,this->world_coordinate.y - camYBoundary };
			AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &player.expectedLocation, camFollowupSpeedX);
			this->world_coordinate.y = desiredCamLocation.y + camYBoundary;
		}
		AEGfxSetCamPosition(this->world_coordinate.x, this->world_coordinate.y);
	}
}

void Camera::LookAhead(AEVec2 Loc)
{
	look_ahead.x = Loc.x;
	look_ahead.y = Loc.y;
	lookahead = true;
}

void Camera::CameraShake()
{
	shake_timer = 0.3f;
}

Camera::~Camera() {

}

s32 Camera::GetCameraScreenX()
{
	return this->screen_x;
}

s32 Camera::GetCameraScreenY()
{
	return this->screen_y;
}

AEVec2 Camera::GetCameraWorldPoint()
{
	return this->world_coordinate;
}