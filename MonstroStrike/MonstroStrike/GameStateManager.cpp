enum GameStates {
	MainMenu = 0,
	GameLobby,
	Area1,
	Area2
};


typedef void(*GameStateCycle)(void);

extern int current, next;

extern GameStateCycle fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;