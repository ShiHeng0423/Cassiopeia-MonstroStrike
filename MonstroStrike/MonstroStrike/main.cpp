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
// ---------------------------------------------------------------------------
// main

namespace {
	s32 cursorX, cursorY; //Mouse coordinate
	const f32 friction = 0.95f; //Friction, const for now unless some tile add friction
}

struct Player {
	AEMtx33 scale;
	AEMtx33 rotation;
	AEMtx33 translation;
	AEMtx33 transformation;

	AEVec2 size;
	AEVec2 position;
	AEVec2 velocity;
	f32 mass;

	AABB collisionBox;
	AABB boxHeadFeet;
	AABB arms;

	bool canJump;
}player;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 0, 60, true, NULL);

	// Changing the window title
	AESysSetWindowTitle("MonstroStrike");

	//// reset the system modules
	//AESysReset();

	//Load Fonts
	s8 pFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 72);
	//2D vector create

	GSM_Initialize(GameStates::MainMenu);

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

			// Informing the system about the loop's end
			AESysFrameEnd();
		}
		fpFree();
		fpUnload();
		current = next;
	}
	/*-----------Freeing Images and others----------*/

	AEGfxDestroyFont(pFont);
	//Resizing vector, clear content, then resize it to 0
	
	// free the system
	AESysExit();
}

