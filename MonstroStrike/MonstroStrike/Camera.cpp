#include "Camera.h"

#define camXBoundary (500.f)
#define camYBoundary (10.f)
#define camFollowupSpeedX (0.05f)

Camera::Camera(AEVec2 player)
{
	s32 cursor_x = 0;
	s32 cursor_y = 0;
	AEInputGetCursorPosition(&cursor_x, &cursor_y);

	this->screenX = cursor_x - AEGfxGetWindowWidth() * 0.5f;
	this->screenY = AEGfxGetWindowHeight() * 0.5f - cursor_x;

	this->screenX = this->screenX - AEGfxGetWindowWidth() * 0.5f;
	this->screenY = AEGfxGetWindowHeight() * 0.5f - this->screenY;

	this->worldCoordinate = player;
	AEGfxSetCamPosition(this->worldCoordinate.x, this->worldCoordinate.y);

	AEVec2Set(&currLookAheadDir, 0, 0);
	AEVec2Set(&expectedLookAheadDir, 0, 0);

	lookAhead = false;
	lookBack = false;
	cameraOnHold = false;

	shakeTimer = 0.f; 
	lookbackTimer = 0.f;
}

void Camera::UpdatePos(class Player* player,f32 gameMinWidth, f32 gameMaxWidth, f32 gameMinHeight,f32 gameMaxHeight)
{
	s32 cursor_x = 0;
	s32 cursor_y = 0;
	AEInputGetCursorPosition(&cursor_x, &cursor_y);
	this->screenX = cursor_x - AEGfxGetWindowWidth() * 0.5f;
	this->screenY = AEGfxGetWindowHeight() * 0.5f - cursor_x;

	if (lookAhead)
	{
		AEVec2 desiredCamLocation{ 0,0 };

		if (AEVec2Distance(&currLookAheadDir, &desiredCamLocation) == 0)
		{
			AEGfxGetCamPosition(&desiredCamLocation.x, &desiredCamLocation.y);
			currLookAheadDir = desiredCamLocation;
		}
		else
			AEGfxGetCamPosition(&desiredCamLocation.x, &desiredCamLocation.y);

		AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &expectedLookAheadDir, 0.05f);
		AEVec2Lerp(&currLookAheadDir, &currLookAheadDir, &expectedLookAheadDir, 0.05f);

		AEGfxSetCamPosition(desiredCamLocation.x, desiredCamLocation.y);
		if (AEVec2Distance(&currLookAheadDir, &expectedLookAheadDir) < 1.0f && lookbackTimer <= 0.f && !cameraOnHold)
		{
			lookbackTimer = 2.0f;
			cameraOnHold = true;
		}
		else
		{
			lookbackTimer -= (f32)AEFrameRateControllerGetFrameTime();
			if (cameraOnHold && lookbackTimer <= 0.f)
			{
				cameraOnHold = false;
				lookAhead = false;
				lookBack = true;
				AEVec2Set(&currLookAheadDir, 0, 0);
			}
		}
	}
	else if (lookBack)
	{
		AEVec2 desiredCamLocation{ 0,0 };
		if (AEVec2Distance(&currLookAheadDir, &desiredCamLocation) == 0)
		{
			AEGfxGetCamPosition(&desiredCamLocation.x, &desiredCamLocation.y);
			currLookAheadDir = desiredCamLocation;
		}
		else
			AEGfxGetCamPosition(&desiredCamLocation.x, &desiredCamLocation.y);

		AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &this->worldCoordinate, 0.05f);
		AEVec2Lerp(&currLookAheadDir, &currLookAheadDir, &this->worldCoordinate, 0.05f);

		AEGfxSetCamPosition(desiredCamLocation.x, desiredCamLocation.y);
		if (AEVec2Distance(&currLookAheadDir, &this->worldCoordinate) < 1.0f)
		{
			lookBack = false;
		}
	}
	else
	{
		shakeTimer -= (f32)AEFrameRateControllerGetFrameTime();
		if (shakeTimer > 0.f)
		{
			f32 randX = (AERandFloat() - 0.5f) * 100.f;
			f32 randY = (AERandFloat() - 0.5f) * 100.f;
			AEVec2 desiredCamLocation{ this->worldCoordinate.x + randX,this->worldCoordinate.y + randY };
			AEVec2Lerp(&this->worldCoordinate, &this->worldCoordinate, &desiredCamLocation, 0.05f);
		}

		if ((player->expectedLocation.x > this->worldCoordinate.x + camXBoundary) && player->isFacingRight)
		{
			AEVec2 desiredCamLocation{ this->worldCoordinate.x + camXBoundary,this->worldCoordinate.y };
			AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &player->expectedLocation, camFollowupSpeedX);
			this->worldCoordinate.x = desiredCamLocation.x - camXBoundary;
		}
		else if ((player->expectedLocation.x < this->worldCoordinate.x - camXBoundary) && !player->isFacingRight)
		{
			AEVec2 desiredCamLocation{ this->worldCoordinate.x - camXBoundary,this->worldCoordinate.y };
			AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &player->expectedLocation, camFollowupSpeedX);
			this->worldCoordinate.x = desiredCamLocation.x + camXBoundary;
		}
		if (player->expectedLocation.y > this->worldCoordinate.y + camYBoundary)
		{
			AEVec2 desiredCamLocation{ this->worldCoordinate.x, this->worldCoordinate.y + camYBoundary };
			AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &player->expectedLocation, camFollowupSpeedX);
			this->worldCoordinate.y = desiredCamLocation.y - camYBoundary;
		}
		else if (player->expectedLocation.y < this->worldCoordinate.y - camYBoundary)
		{
			AEVec2 desiredCamLocation{ this->worldCoordinate.x,this->worldCoordinate.y - camYBoundary };
			AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &player->expectedLocation, camFollowupSpeedX);
			this->worldCoordinate.y = desiredCamLocation.y + camYBoundary;
		}

		AEGfxSetCamPosition(this->worldCoordinate.x, this->worldCoordinate.y);
	}

	f32 tmpx,tmpy;
	AEGfxGetCamPosition(&tmpx, &tmpy);
	if (tmpx - AEGfxGetWindowWidth() * 0.5f < gameMinWidth)
	{
		tmpx = gameMinWidth + AEGfxGetWindowWidth() * 0.5f;
	}
	else if (tmpx + AEGfxGetWindowWidth() * 0.5f > gameMaxWidth)
	{
		tmpx = gameMaxWidth - AEGfxGetWindowWidth() * 0.5f;
	}
	if (tmpy + AEGfxGetWindowHeight() * 0.5f > gameMaxHeight)
	{
		tmpy = gameMaxHeight - AEGfxGetWindowHeight() * 0.5f;
	}
	else if (tmpy - AEGfxGetWindowHeight() * 0.5f < gameMinHeight)
	{
		tmpy = gameMinHeight + AEGfxGetWindowHeight() * 0.5f;
	}

	AEGfxSetCamPosition(tmpx, tmpy);
}

void Camera::LookAhead(AEVec2 locationPlayer)
{
	expectedLookAheadDir.x = locationPlayer.x;
	expectedLookAheadDir.y = locationPlayer.y;
	lookAhead = true;
}

void Camera::CameraShake()
{
	shakeTimer = 0.3f;
}

Camera::~Camera() {

}

f32 Camera::GetCameraScreenX() const
{
	return this->screenX;
}

f32 Camera::GetCameraScreenY() const 
{
	return this->screenY;
}

AEVec2 Camera::GetCameraWorldPoint() const
{
	return this->worldCoordinate;
	
}