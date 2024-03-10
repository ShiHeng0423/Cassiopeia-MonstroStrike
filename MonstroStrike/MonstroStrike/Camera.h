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
	bool lookahead;
	bool lookback;
	bool cameraOnHold;

	f64 shake_timer;
	f64 lookback_timer;
	
	f32 screen_x;
	f32 screen_y;

	AEVec2 world_coordinate;
	AEVec2 look_ahead;
};