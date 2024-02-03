#include "GameStateManager.h"
#include "Level1.h"
#include "GameLobby.h"

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
	case MainMenu:
		fpLoad = Level1_Load;
		fpInitialize = Level1_Initialize;
		fpUpdate = Level1_Update;
		fpDraw = Level1_Draw;
		fpFree = Level1_Free;
		fpUnload = Level1_Unload;
		break;
	case GameLobby:
		fpLoad = GameLobby_Load;
		fpInitialize = GameLobby_Initialize;
		fpUpdate = GameLobby_Update;
		fpDraw = GameLobby_Draw;
		fpFree = GameLobby_Free;
		fpUnload = GameLobby_Unload;
		break;
	case Area1:
		break;
	case Quit:
		break;
	default:
		break;
	}
}
