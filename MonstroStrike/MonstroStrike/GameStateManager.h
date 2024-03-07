#pragma once
enum GameStates {
	SplashScreen = 0,
	MainMenu,
	GameLobby,
	Area1,
	Quit,

	GAMESTATE_NONE
};


typedef void(*GameStateCycle)(void);

extern int current, next;

extern GameStateCycle fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;

void GSM_Initialize(int startingState);

void GSM_Update();