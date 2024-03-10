#pragma once
#include "Player.h"
#include "AEEngine.h"

class Camera {
public:
	Camera(AEVec2 player);
	~Camera();

	void UpdatePos(Player player, f32 gameMinWidth, f32 gameMaxWidth, f32 gameMinHeight, f32 gameMaxHeight);
	void CameraShake();
	void LookAhead(AEVec2 locationPlayer);

	f32  GetCameraScreenX() const;
	f32  GetCameraScreenY() const;

	AEVec2 GetCameraWorldPoint() const;
private:
	bool lookAhead;
	bool lookBack;
	bool cameraOnHold;

	f64 shakeTimer;
	f64 lookbackTimer;
	
	f32 screenX;
	f32 screenY;

	AEVec2 worldCoordinate;
	AEVec2 lookAheadDir;
};