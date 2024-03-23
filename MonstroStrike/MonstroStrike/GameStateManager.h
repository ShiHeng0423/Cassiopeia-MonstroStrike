#pragma once
enum GameStates {
	SPLASHSCREEN = 0,
	MAINMENU,
	GAME_LOBBY,
	AREA1,
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