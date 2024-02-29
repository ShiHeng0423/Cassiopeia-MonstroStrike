#pragma once
#pragma once
#include "Player.h"
#include "AEEngine.h"

class Camera {
public:
	Camera(AEVec2);
	~Camera();

	void UpdatePos(Player);
	void CameraShake();
	void LookAhead(AEVec2);

	s32 GetCameraScreenX();
	s32 GetCameraScreenY();

	AEVec2 GetCameraWorldPoint();
private:
	bool lookahead;
	bool lookback;
	bool cameraOnHold;

	f32 shake_timer;
	f32 lookback_timer;
	
	s32 screen_x;
	s32 screen_y;

	AEVec2 world_coordinate;
	AEVec2 look_ahead;
};