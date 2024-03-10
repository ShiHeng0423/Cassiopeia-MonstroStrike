#include "GameStateManager.h"
#include "GameState_Level1.h"
#include "GameState_GameLobby.h"
#include "GameState_Mainmenu.h"
#include "GameState_SplashScreen.h"

int current = 0, previous = 0, next = 0;

GameStateCycle fpLoad = nullptr, fpInitialize = nullptr, fpUpdate = nullptr, fpDraw = nullptr, fpFree = nullptr, fpUnload = nullptr;

void GSM_Initialize(int startingState)
{
	current = previous = next = startingState;
}

void GSM_Update()
{
	switch (current)
	{
	case SPLASHSCREEN:
		fpLoad = SplashScreen_Load;
		fpInitialize = SplashScreen_Initialize;
		fpUpdate = SplashScreen_Update;
		fpDraw = SplashScreen_Draw;
		fpFree = SplashScreen_Free;
		fpUnload = SplashScreen_Unload;
		break;
	case MAINMENU:
		fpLoad = Mainmenu_Load;
		fpInitialize = Mainmenu_Initialize;
		fpUpdate = Mainmenu_Update;
		fpDraw = Mainmenu_Draw;
		fpFree = Mainmenu_Free;
		fpUnload = Mainmenu_Unload;
		break;
	case GAME_LOBBY:
		fpLoad = GameLobby_Load;
		fpInitialize = GameLobby_Initialize;
		fpUpdate = GameLobby_Update;
		fpDraw = GameLobby_Draw;
		fpFree = GameLobby_Free;
		fpUnload = GameLobby_Unload;
		break;
	case AREA1:
		fpLoad = Level1_Load;
		fpInitialize = Level1_Initialize;
		fpUpdate = Level1_Update;
		fpDraw = Level1_Draw;
		fpFree = Level1_Free;
		fpUnload = Level1_Unload;
		break;
	case QUIT:
		break;
	default:
		break;
	}
}
