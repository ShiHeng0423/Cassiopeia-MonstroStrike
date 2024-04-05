/*!************************************************************************
  \file					GameState_Manager.h
  \project name			Monstrostrike
  \primary author		Teng Shi heng (100%)
  \brief				This file defines a game state manager for handling
						different states of the game. 
						
						The GameStates enum defines the different game states
						of the game.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#pragma once
enum GameStates {
	SPLASHSCREEN = 0,
	MAINMENU,
	GAME_LOBBY,
	AREA1_A,
	AREA1_B,
	AREA1_C,
	AREA1_D,
	AREA1_E,
	AREA1_F,
	AREA_BOSS,

	QUIT,

	GAMESTATE_NONE
};


typedef void(*GameStateCycle)(void);

extern int current, previous, next;

extern GameStateCycle fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;

void GSM_Initialize(int startingState);

void GSM_Update();