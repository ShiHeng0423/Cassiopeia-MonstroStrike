/*!************************************************************************
  \file					main.h
  \project name			Monstrostrike
  \primary author		Teng Shi Heng (95%)
  \secondary authors	Johny Yong Jun Siang (5%, added loading of map transition objects/textures and grid textures)
  \brief				The header of the main file that include externs of managers such as audio and game managers, also include
						the fontID.


All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#pragma once
#include "AEEngine.h"
#include "AudioManager.h"
#include "GameManager.h"

extern s8 fontID;
extern AudioManager* audioManager;
extern GameManager* gameManager;