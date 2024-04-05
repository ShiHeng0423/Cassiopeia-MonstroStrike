/*!************************************************************************
  \file					DebuggerManager.h
  \project name			Monstrostrike
  \primary author		Teng Shi heng (100%)
  \brief				File containing the declaration of class DebuggerManager
						where its responsible for allowing the developer
						to test certain parts game mechanics faster and
						game demo during presentations.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#pragma once
#include "Player.h"
#include <AEEngine.h>
 
//Debugger Functionalites
enum DebuggerFunction
{
	FPS,							//displaying game fps
	POSITION,						//displaying player current position
	IMMORTAL,						//make player immune to damage
	MAX_POWER,						//make player able to one hit kill enemies
	TELEPORT_LEVEL_UP,				//make player able to transverse up the level
	TELEPORT_LEVEL_DOWN,			//make player able to transverse down the level
	TELEPORT_CONFIRMATION,			//confirmation button to transverse the player level
	GOD_MODE_FILE,					//enable user to load the god mode file (full inventory list)

	DEBUGGER_TOTAL
};

//for player area transversing
enum AreaNumber
{
	AREA_0,				//game lobby
	AREA_1,				//Level 1
	AREA_2,				//Level B
	AREA_3,				//Level C
	AREA_4,				//Level D
	AREA_5,				//Level E
	AREA_6,				//Level F
	AREA_LAST,			//Boss Area

	AREA_TOTAL
};

class DebuggerManager
{
public:
	DebuggerManager(Player* player);
	~DebuggerManager();
	void Update();
	void RenderDebuggerUI();

	static int& GetCurrentArea();

	void setDebugClose();
private:
	bool openDebugPanel;
	static int currArea;
	Player* playerInfo;
};