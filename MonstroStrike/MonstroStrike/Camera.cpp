#include "Camera.h"

#define camXBoundary (500.f)
#define camYBoundary (10.f)
#define camFollowupSpeedX (0.05f)

Camera::Camera(AEVec2 player)
{
	s32 cursor_x = 0;
	s32 cursor_y = 0;
	AEInputGetCursorPosition(&cursor_x, &cursor_y);
	this->screen_X = cursor_x - AEGfxGetWindowWidth() * 0.5f;
	this->screen_Y = AEGfxGetWindowHeight() * 0.5f - cursor_x;
	this->screen_X = this->screen_X - AEGfxGetWindowWidth() * 0.5f;
	this->screen_Y = AEGfxGetWindowHeight() * 0.5f - this->screen_Y;
	this->world_Coordinate = player;
	AEGfxSetCamPosition(this->world_Coordinate.x, this->world_Coordinate.y);
	look_Ahead.x = look_Ahead.y = 0;

	lookAhead = false;
	lookBack = false;
	cameraOnHold = false;
	shake_Timer = 0.f; 
	lookback_Timer = 0.f;
}

void Camera::UpdatePos(Player player,f32 gameMinWidth, f32 gameMaxWidth, f32 gameMinHeight,f32 gameMaxHeight)
{
	s32 cursor_x = 0;
	s32 cursor_y = 0;
	AEInputGetCursorPosition(&cursor_x, &cursor_y);
	this->screen_X = cursor_x - AEGfxGetWindowWidth() * 0.5f;
	this->screen_Y = AEGfxGetWindowHeight() * 0.5f - cursor_x;

	if (lookAhead)
	{
		AEVec2 desiredCamLocation{ 0,0 };
		AEGfxGetCamPosition(&desiredCamLocation.x, &desiredCamLocation.y);
		AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &look_Ahead, 0.2f);
		AEGfxSetCamPosition(desiredCamLocation.x, desiredCamLocation.y);
		if (AEVec2Distance(&desiredCamLocation, &look_Ahead) < 1.0f && lookback_Timer <= 0.f && !cameraOnHold)
		{
			lookback_Timer = 5.0f;
			cameraOnHold = true;
		}
		else
		{
			lookback_Timer -= AEFrameRateControllerGetFrameTime();
			if (cameraOnHold && lookback_Timer <= 0.f)
			{
				cameraOnHold = false;
				lookAhead = false;
				lookBack = true;
			}
		}
	}
	else if (lookBack)
	{
		AEVec2 desiredCamLocation{ 0,0 };
		AEGfxGetCamPosition(&desiredCamLocation.x, &desiredCamLocation.y);
		AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &this->world_Coordinate, 0.05f);
		AEGfxSetCamPosition(desiredCamLocation.x, desiredCamLocation.y);
		if (AEVec2Distance(&desiredCamLocation, &this->world_Coordinate) < 1.0f)
		{
			lookBack = false;
		}
	}
	else
	{
		shake_Timer -= AEFrameRateControllerGetFrameTime();
		if (shake_Timer > 0.f)
		{
			f32 randX = (AERandFloat() - 0.5f) * 100.f;
			f32 randY = (AERandFloat() - 0.5f) * 100.f;
			AEVec2 desiredCamLocation{ this->world_Coordinate.x + randX,this->world_Coordinate.y + randY };
			AEVec2Lerp(&this->world_Coordinate, &this->world_Coordinate, &desiredCamLocation, 0.05f);
		}

		if ((player.expectedLocation.x > this->world_Coordinate.x + camXBoundary) && player.isFacingRight)
		{
			AEVec2 desiredCamLocation{ this->world_Coordinate.x + camXBoundary,this->world_Coordinate.y };
			AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &player.expectedLocation, camFollowupSpeedX);
			this->world_Coordinate.x = desiredCamLocation.x - camXBoundary;
		}
		else if ((player.expectedLocation.x < this->world_Coordinate.x - camXBoundary) && !player.isFacingRight)
		{
			AEVec2 desiredCamLocation{ this->world_Coordinate.x - camXBoundary,this->world_Coordinate.y };
			AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &player.expectedLocation, camFollowupSpeedX);
			this->world_Coordinate.x = desiredCamLocation.x + camXBoundary;
		}
		if (player.expectedLocation.y > this->world_Coordinate.y + camYBoundary)
		{
			AEVec2 desiredCamLocation{ this->world_Coordinate.x, this->world_Coordinate.y + camYBoundary };
			AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &player.expectedLocation, camFollowupSpeedX);
			this->world_Coordinate.y = desiredCamLocation.y - camYBoundary;
		}
		else if (player.expectedLocation.y < this->world_Coordinate.y - camYBoundary)
		{
			AEVec2 desiredCamLocation{ this->world_Coordinate.x,this->world_Coordinate.y - camYBoundary };
			AEVec2Lerp(&desiredCamLocation, &desiredCamLocation, &player.expectedLocation, camFollowupSpeedX);
			this->world_Coordinate.y = desiredCamLocation.y + camYBoundary;
		}
		AEGfxSetCamPosition(this->world_Coordinate.x, this->world_Coordinate.y);
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

void Camera::LookAhead(AEVec2 Loc)
{
	look_Ahead.x = Loc.x;
	look_Ahead.y = Loc.y;
	lookAhead = true;
}

void Camera::CameraShake()
{
	shake_Timer = 0.3f;
}

Camera::~Camera() {

}

f32 Camera::GetCameraScreenX() const
{
	return this->screen_X;
}

f32 Camera::GetCameraScreenY() const 
{
	return this->screen_Y;
}

AEVec2 Camera::GetCameraWorldPoint() const
{
	return this->world_Coordinate;
	
}