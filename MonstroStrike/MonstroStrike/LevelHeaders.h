/*!************************************************************************
  \file					LevelHeaders.h
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (100%)
  \brief				The file that includes all the neccessary files
						for a game level.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#pragma once

//Headers necessary for a level
#include "AEEngine.h"
#include "GameStateManager.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyUtils.h"
#include <iostream>

#include "CSVMapLoader.h"
#include "GridTypesList.h"
#include "Physics.h"
#include "CollisionShape.h" //For Vertical + Horizontal collision
#include <string>
#include "Camera.h"
#include "PauseMenuManager.h"
#include <vector>

#include "MapPlatformGenerate.h"
#include "NonPlayableCharacters.h"

#include "ParticleSystem.h"

#include "Inventory.h"
#include "Crafting.h"
#include "main.h"
#include "MissionList.h"
#include "MapTraps.h"
