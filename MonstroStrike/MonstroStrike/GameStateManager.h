#pragma once
enum GameStates {
	MainMenu = 0,
	GameLobby,
	Area1,
	Quit
};


typedef void(*GameStateCycle)(void);

extern int current, next;

extern GameStateCycle fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;

void GSM_Initialize(int startingState);

void GSM_Update();