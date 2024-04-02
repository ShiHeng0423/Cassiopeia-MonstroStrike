#pragma once

//capable of showing the following things
//fps
//change player to be invincible (either hp to test weapon dmg or weapon dmg max to test for armo etc)
//see camera's tracking on player
//teleport player to the corresponding map
#include "Player.h"
#include <AEEngine.h>

class DebuggerManager
{
	DebuggerManager();
	~DebuggerManager();
	void RenderDebuggerUI();

private:
	Player* playerInfo;
};