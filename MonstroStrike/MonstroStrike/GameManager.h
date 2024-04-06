/*!************************************************************************
  \file					GamerManager.h
  \project name			Monstrostrike
  \primary author		Teng Shi heng (100%)
  \brief				File containing declarations of class GameManager
						that is responsible for holding the player statistics
						throughout the entire game so that there is only one 
						Player class created at any time of the game, updating the
						DebuggerManager, and providing access to player 
						stats and debugger functionalities.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/
#pragma once

#include "Player.h"
#include "DebuggerManager.h"

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