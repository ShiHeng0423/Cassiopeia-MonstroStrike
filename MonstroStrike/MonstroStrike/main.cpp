/*!*****************************************************************************
\file    main.cpp
\author  
\par     
\par     Course:  csd1451s24
\par     Section: A
\par     Software Engineering Project 2
\date    1-27-2024

\brief
*******************************************************************************/

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
// ---------------------------------------------------------------------------
// main

s8 fontID;
AudioManager* audioManager;

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

	GSM_Initialize(GameStates::MainMenu);
	fontID = AEGfxCreateFont("Assets/liberation-mono.ttf", 72);
	MapTransitionLoad(); //Placed here to share its usage for all the states (Similar logic to font)
	audioManager = new AudioManager();

	while (current != GameStates::Quit)
	{
		GSM_Update();
		fpLoad();
		fpInitialize();
		Inventory::Load_Inventory();

		// Game Loop
		while (current == next)
		{
			// Informing the system about the loop's start
			AESysFrameStart();

			fpUpdate();
			fpDraw();

			if (0 == AESysDoesWindowExist())
				next = GameStates::Quit;

			// Informing the system about the loop's end
			AESysFrameEnd();
		}
		fpFree();
		fpUnload();
		current = next;
	}

	MapTransitionUnload();//Unload Map Transition image here
	AEGfxDestroyFont(fontID);
	delete audioManager;
	// free the system
	AESysExit();
}
