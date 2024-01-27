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
// ---------------------------------------------------------------------------
// main

namespace {
	s32 cursorX, cursorY; //Mouse coordinate
}

struct Grids2D {

	AEMtx33 scale;
	AEMtx33 rotation;
	AEMtx33 translation;
	AEMtx33 transformation;

	s16 rowIndex;
	s16 colIndex;

	AEVec2 size;
	AEVec2 position;

	GRID_TYPES typeOfGrid;

} grids2D[MAP_ROW_SIZE][MAP_COLUMN_SIZE];


//This is for printing the map
void InitializeGrid(Grids2D& theGrids)
{
	//Setting up the scale
	theGrids.size.x = AEGfxGetWindowWidth() * 0.025f; // *1.f means cover whole width, *0.1f means 10 tiles per 1600px width map, * 0.01f means 100 tiles
	theGrids.size.y = AEGfxGetWindowWidth() * 0.025f;
	theGrids.scale = { 0 };
	AEMtx33Scale(&theGrids.scale, theGrids.size.x, theGrids.size.y);

	theGrids.rotation = { 0 };
	AEMtx33Rot(&theGrids.rotation, 0);

	//Printing the map
	theGrids.position.x = -AEGfxGetWindowWidth() * 0.5f + theGrids.size.x * (theGrids.colIndex - 0.5f);
	theGrids.position.y = AEGfxGetWindowHeight() * 0.5f - theGrids.size.x * (theGrids.rowIndex - 0.5f);

	AEMtx33Trans(&theGrids.translation, theGrids.position.x, theGrids.position.y);

	theGrids.transformation = { 0 };
	AEMtx33Concat(&theGrids.transformation, &theGrids.rotation, &theGrids.scale);
	AEMtx33Concat(&theGrids.transformation, &theGrids.translation, &theGrids.transformation);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int gGameRunning = 1;

	// Initialization of your own variables go here

	// Using custom window procedure
	nCmdShow = true;
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, true, NULL);

	// Changing the window title
	AESysSetWindowTitle("MonstroStrike");

	//// reset the system modules
	//AESysReset();

	AEGfxMeshStart();

	//Mesh Creation
#pragma region Drawing 1x1 mesh
	// *Color follows ARGB format (Alpha, Red, Green, Blue)
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFa9a9a9, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, 0xFFa9a9a9, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, 0xFFa9a9a9, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFa9a9a9, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, 0xFFa9a9a9, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, 0xFFa9a9a9, 0.0f, 0.0f);  // top-left: blue

	AEGfxVertexList* pMesh = AEGfxMeshEnd();

	// *Color follows ARGB format (Alpha, Red, Green, Blue)
	//Yellow Mesh
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, 0xFFFF0000, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFF0000, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, 0xFFFF0000, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);  // top-left: blue

	AEGfxVertexList* pMeshYellow = AEGfxMeshEnd();
#pragma endregion

	//Load Fonts
	s8 pFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 72);

	AEGfxTexture* pTex = AEGfxTextureLoad("Assets/border.png");

	//2D vector create
	std::vector<std::vector<MapCell>> gameMap(MAP_ROW_SIZE, std::vector<MapCell>(MAP_COLUMN_SIZE));
	const char* fileName = "Assets/GameMap.csv";

	if (MapLoader(fileName, gameMap, MAP_ROW_SIZE, MAP_COLUMN_SIZE))
	{
		PrintMap(gameMap, MAP_ROW_SIZE, MAP_COLUMN_SIZE); //This is just for checking if the map data is stored properly
	}

	//Initializing grid data
	for (s16 rows = 0; rows < MAP_ROW_SIZE; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_SIZE; cols++)
		{
			switch (gameMap[rows][cols].symbol) //For checking the map symbol from the csv file
			{
			case '0':
				grids2D[rows][cols].typeOfGrid = EMPTY;
				break;
			case '1':
				grids2D[rows][cols].typeOfGrid = NORMAL_GROUND;
				break;
				//Add more if needed to...
			default:
				break;
			}
		}
	}

	//For Initializing the grids
	for (s16 rows = 0; rows < MAP_ROW_SIZE; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_SIZE; cols++)
		{
			grids2D[rows][cols].rowIndex = rows;
			grids2D[rows][cols].colIndex = cols;

			if (grids2D[rows][cols].typeOfGrid == NORMAL_GROUND)
			{
				InitializeGrid(grids2D[rows][cols]);

				AEGfxSetTransform(grids2D[rows][cols].transformation.m);
				AEGfxMeshDraw(pMeshYellow, AE_GFX_MDM_TRIANGLES);
			}
			else if (grids2D[rows][cols].typeOfGrid == EMPTY)
			{

				InitializeGrid(grids2D[rows][cols]);
				AEGfxSetTransform(grids2D[rows][cols].transformation.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}
	}
#pragma region 1x1 Texture
	//	//1x1 Texture Initialization
	AEMtx33 borderScale = { 0 };
	AEMtx33 borderRotate = { 0 };
	AEMtx33 borderTranslate = { 0 };

	AEMtx33Scale(&borderScale, static_cast<f32>(AEGfxGetWindowWidth()), static_cast<f32>(AEGfxGetWindowHeight()));
	AEMtx33Rot(&borderRotate, 0);
	AEMtx33Trans(&borderTranslate, 0.f, 0.f);

	AEMtx33 borderTransform = { 0 };
	AEMtx33Concat(&borderTransform, &borderRotate, &borderScale);
	AEMtx33Concat(&borderTransform, &borderTranslate, &borderTransform);

#pragma endregion

	//The game loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		//deltaTime += AEFrameRateControllerGetFrameTime();

		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE); //This one only renders texture

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		// the rest of the components remain the same!
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f); //Careful of adding color
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		AEGfxTextureSet(pTex, 0, 0);

		AEInputGetCursorPosition(&cursorX, &cursorY);

		//Ignore this
		if (AEInputCheckCurr(VK_LEFT))
		{
			for (s16 rows = 0; rows < MAP_ROW_SIZE; rows++)
			{
				for (s16 cols = 0; cols < MAP_COLUMN_SIZE; cols++)
				{
					grids2D[rows][cols].position.x -= 10;
					AEMtx33Trans(&grids2D[rows][cols].translation, grids2D[rows][cols].position.x, grids2D[rows][cols].position.y);
					AEGfxSetTransform(grids2D[rows][cols].transformation.m);
					AEMtx33Concat(&grids2D[rows][cols].transformation, &grids2D[rows][cols].rotation, &grids2D[rows][cols].scale);
					AEMtx33Concat(&grids2D[rows][cols].transformation, &grids2D[rows][cols].translation, &grids2D[rows][cols].transformation);
				}
			}
		}
		else if (AEInputCheckCurr(VK_UP))
		{
			for (s16 rows = 0; rows < MAP_ROW_SIZE; rows++)
			{
				for (s16 cols = 0; cols < MAP_COLUMN_SIZE; cols++)
				{
					grids2D[rows][cols].position.y -= 10;
					AEMtx33Trans(&grids2D[rows][cols].translation, grids2D[rows][cols].position.x, grids2D[rows][cols].position.y);
					AEGfxSetTransform(grids2D[rows][cols].transformation.m);
					AEMtx33Concat(&grids2D[rows][cols].transformation, &grids2D[rows][cols].rotation, &grids2D[rows][cols].scale);
					AEMtx33Concat(&grids2D[rows][cols].transformation, &grids2D[rows][cols].translation, &grids2D[rows][cols].transformation);
				}
			}
		}

		//For printing the grids every frame
		for (s16 rows = 0; rows < MAP_ROW_SIZE; rows++)
		{
			for (s16 cols = 0; cols < MAP_COLUMN_SIZE; cols++)
			{

				if (grids2D[rows][cols].typeOfGrid == NORMAL_GROUND)
				{
					AEGfxSetTransform(grids2D[rows][cols].transformation.m);
					AEGfxMeshDraw(pMeshYellow, AE_GFX_MDM_TRIANGLES);
				}
				else if (grids2D[rows][cols].typeOfGrid == EMPTY)
				{
					AEGfxSetTransform(grids2D[rows][cols].transformation.m);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
				}
			}
		}

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}

	/*-----------Freeing Images and others----------*/
	AEGfxTextureUnload(pTex);
	AEGfxMeshFree(pMesh);
	AEGfxMeshFree(pMeshYellow);
	AEGfxDestroyFont(pFont);

	//Resizing vector, clear content, then resize it to 0
	gameMap.clear();
	gameMap.resize(0);
	/*-----------------------------------*/
	// free the system
	AESysExit();
}

