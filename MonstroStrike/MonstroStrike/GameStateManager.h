#pragma once
enum GameStates {
	SPLASHSCREEN = 0,
	MAINMENU,
	GAME_LOBBY,
	AREA1,
	QUIT,

	GAMESTATE_NONE
};


typedef void(*GameStateCycle)(void);

extern int current, next;

extern GameStateCycle fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;

void GSM_Initialize(int startingState);

void GSM_Update();