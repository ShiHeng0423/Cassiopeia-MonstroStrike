#pragma once
/*!************************************************************************
  \file					GameState_Level1.h
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (35%, added initialisations of grids and traps etc, map transitions, particle effects)
  \secondary authors	Teng Shi Heng (35%, player, menu UI, Camera),
						Goh Jun Jie (15%, enemies),
						Keith Ng Hon Yew (15%, inventory)
  \brief				File containing declarations of functions related to the level 1 part A room state. 
						Such as loading, initializing, update, drawing etc... of the level.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/
void Level1_A_Load();
void Level1_A_Initialize();
void Level1_A_Update();
void Level1_A_Draw();
void Level1_A_Free();
void Level1_A_Unload();
