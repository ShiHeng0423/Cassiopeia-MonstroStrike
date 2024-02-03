#pragma once
#include "Utils.h"

struct Player {
	Object obj;

	bool isFacingRight;

	AEVec2 expectedLocation;

	f32 lookAheadMutliplier;
};

Player* PlayerInitialize(const char* filename, AEVec2 scale, AEVec2 location, AEVec2 speed, bool isFacingRight); 
void PlayerUpdate(Player& player);