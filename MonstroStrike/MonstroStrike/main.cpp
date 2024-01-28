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
#include "AEEngine.h"
#include "GameStateManager.h"
// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, true, NULL);

	// Changing the window title
	AESysSetWindowTitle("MonstroStrike");

	GSM_Initialize(GameStates::MainMenu);

	while (current != GameStates::Quit)
	{
		GSM_Update();
		fpLoad();
		fpInitialize();

		// Game Loop
		while (current == next)
		{
			// Informing the system about the loop's start
			AESysFrameStart();

			fpUpdate();
			fpDraw();

			// Informing the system about the loop's end
			AESysFrameEnd();
		}
		fpFree();
		fpUnload();
	}
	// free the system
	AESysExit();
}
