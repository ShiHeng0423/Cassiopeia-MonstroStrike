#pragma once
#include "Player.h"
#include "AEEngine.h"

class Camera {
public:
	Camera(AEVec2);
	~Camera();

	void UpdatePos(Player, f32, f32, f32, f32);
	void CameraShake();
	void LookAhead(AEVec2);

	f32  GetCameraScreenX() const;
	f32  GetCameraScreenY() const;

	AEVec2 GetCameraWorldPoint() const;
private:
	bool lookAhead;
	bool lookBack;
	bool cameraOnHold;

	f64 shake_Timer;
	f64 lookback_Timer;
	
	f32 screen_X;
	f32 screen_Y;

	AEVec2 world_Coordinate;
	AEVec2 look_Ahead;
};