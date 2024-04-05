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



//capable of showing the following things
//fps
//change player to be invincible (either hp to test weapon dmg or weapon dmg max to test for armo etc)
//see camera's tracking on player
//teleport player to the corresponding map
#include "Player.h"
#include <AEEngine.h>

enum DebuggerFunction
{
	FPS,
	POSITION,
	IMMORTAL,
	MAX_POWER,
	TELEPORT_LEVEL_UP,
	TELEPORT_LEVEL_DOWN,
	TELEPORT_CONFIRMATION,
	GOD_MODE_FILE,

	DEBUGGER_TOTAL
};

enum AreaNumber
{
	AREA_0,
	AREA_1,
	AREA_2,
	AREA_3,
	AREA_4,
	AREA_5,
	AREA_6,
	AREA_LAST,

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