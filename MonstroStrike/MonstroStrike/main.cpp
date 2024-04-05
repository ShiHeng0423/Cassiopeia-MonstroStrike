/*!************************************************************************
  \file					main.cpp
  \project name			Monstrostrike
  \primary author		Teng Shi Heng (95%)
  \secondary authors	Johny Yong Jun Siang (5%)
  \brief				The main file responsible for initializing the game engine and managing game states. Some functions,
						such as loading for MapTransition, GridTexture, in order for the texture to be able to utilise the texture
						throughout the maps without reloading it.


All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include <crtdbg.h> // To check for memory leaks
#include "GridTypesList.h"
#include "CSVMapLoader.h"
#include "AEEngine.h"
#include <iostream>
#include "CollisionShape.h"
#include "Physics.h"
#include "GameStateManager.h"
#include "Inventory.h"
#include "MapTransition.h"
#include "main.h"
#include "MissionList.h"
// ---------------------------------------------------------------------------
// main

s8 fontID;
AudioManager* audioManager;
GameManager* gameManager;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, true, NULL);
	//AESysSetFullScreen(1);
	// Changing the window title
	AESysSetWindowTitle("MonstroStrike");

	//// reset the system modules
	//AESysReset();

	GSM_Initialize(GameStates::SPLASHSCREEN);
	fontID = AEGfxCreateFont("Assets/liberation-mono.ttf", 72);
	MapTransitionLoad(); //Placed here to share its usage for all the states (Similar logic to font)
	GridTextureLoad(); //Placed here to share its usage for all states

	audioManager = new AudioManager();
	gameManager = new GameManager();
	while (current != GameStates::QUIT)
	{
		GSM_Update();
		fpLoad();
		fpInitialize();

		// Game Loop
		while (current == next)
		{
			// Informing the system about the loop's start
			AESysFrameStart();
			gameManager->Update();
			fpUpdate();
			fpDraw();
			gameManager->Render();
			if (0 == AESysDoesWindowExist())
				next = GameStates::QUIT;

			// Informing the system about the loop's end
			AESysFrameEnd();
		}
		fpFree();
		fpUnload();
		previous = current;
		current = next;
	}

	MapTransitionUnload();//Unload Map Transition image here
	GridTextureUnload();
	AEGfxDestroyFont(fontID);
	missionSystem.CleanMemory();
	delete audioManager;
	delete gameManager;
	// free the system
	AESysExit();
}
