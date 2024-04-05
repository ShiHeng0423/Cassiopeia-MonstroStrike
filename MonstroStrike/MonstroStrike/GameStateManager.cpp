/*!************************************************************************
  \file					GameStateManager.cpp
  \project name			Monstrostrike
  \primary author		Teng Shi heng (100%)
  \brief				File containing the definitions of functions declared
						GameStateManager.h files.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#include "GameStateManager.h"
#include "GameStateHeaders.h"
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
		fpLoad = Lobby_Load;
		fpInitialize = Lobby_Initialize;
		fpUpdate = Lobby_Update;
		fpDraw = Lobby_Draw;
		fpFree = Lobby_Free;
		fpUnload = Lobby_Unload;
		break;
	case AREA1_A:
		fpLoad = Level1_A_Load;
		fpInitialize = Level1_A_Initialize;
		fpUpdate = Level1_A_Update;
		fpDraw = Level1_A_Draw;
		fpFree = Level1_A_Free;
		fpUnload = Level1_A_Unload;
		break;
	case AREA1_B:
		fpLoad = Level1_B_Load;
		fpInitialize = Level1_B_Initialize;
		fpUpdate = Level1_B_Update;
		fpDraw = Level1_B_Draw;
		fpFree = Level1_B_Free;
		fpUnload = Level1_B_Unload;
		break;
	case AREA1_C:
		fpLoad = Level1_C_Load;
		fpInitialize = Level1_C_Initialize;
		fpUpdate = Level1_C_Update;
		fpDraw = Level1_C_Draw;
		fpFree = Level1_C_Free;
		fpUnload = Level1_C_Unload;
		break;
	case AREA1_D:
		fpLoad = Level1_D_Load;
		fpInitialize = Level1_D_Initialize;
		fpUpdate = Level1_D_Update;
		fpDraw = Level1_D_Draw;
		fpFree = Level1_D_Free;
		fpUnload = Level1_D_Unload;
		break;
	case AREA1_E:
		fpLoad = Level1_E_Load;
		fpInitialize = Level1_E_Initialize;
		fpUpdate = Level1_E_Update;
		fpDraw = Level1_E_Draw;
		fpFree = Level1_E_Free;
		fpUnload = Level1_E_Unload;
		break;
	case AREA1_F:
		fpLoad = Level1_F_Load;
		fpInitialize = Level1_F_Initialize;
		fpUpdate = Level1_F_Update;
		fpDraw = Level1_F_Draw;
		fpFree = Level1_F_Free;
		fpUnload = Level1_F_Unload;
		break;
	case AREA_BOSS:
		fpLoad = Level1_BOSS_Load;
		fpInitialize = Level1_BOSS_Initialize;
		fpUpdate = Level1_BOSS_Update;
		fpDraw = Level1_BOSS_Draw;
		fpFree = Level1_BOSS_Free;
		fpUnload = Level1_BOSS_Unload;
		break;
	case QUIT:
		break;
	default:
		break;
	}
}
