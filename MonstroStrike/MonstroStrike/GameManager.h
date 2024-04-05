#pragma once

#include "Player.h"
#include "DebuggerManager.h"

//game manager
//hold player data
//inventory


class GameManager {

public:
	GameManager();
	~GameManager();

	void Update();
	
	//Update/Get Player Stats etc.
	Player* GetPlayer();
	DebuggerManager* getDebugManager();

	void Render();

private:
	bool debuggerModeOn;

	Player* player;
	DebuggerManager* debug;
};