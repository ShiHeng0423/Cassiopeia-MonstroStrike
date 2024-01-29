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
// ---------------------------------------------------------------------------
// main

namespace {
	s32 cursorX, cursorY; //Mouse coordinate
	const f32 friction = 0.95f;
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
	AEVec2 velocity;

	f32 mass;

	GRID_TYPES typeOfGrid;

	bool someoneOnTop;

	AABB collisionBox; //Rectangle collision box

} grids2D[MAP_ROW_SIZE][MAP_COLUMN_SIZE];

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
	AABB checkGround;

	bool onFloor;
	bool isJump;
}player;

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

	//Positioning the grid
	theGrids.position.x = -AEGfxGetWindowWidth() * 0.5f + theGrids.size.x * (theGrids.colIndex - 0.5f);
	theGrids.position.y = AEGfxGetWindowHeight() * 0.5f - theGrids.size.x * (theGrids.rowIndex - 0.5f);

	AEMtx33Trans(&theGrids.translation, theGrids.position.x, theGrids.position.y);

	theGrids.transformation = { 0 };
	AEMtx33Concat(&theGrids.transformation, &theGrids.rotation, &theGrids.scale);
	AEMtx33Concat(&theGrids.transformation, &theGrids.translation, &theGrids.transformation);

	// Initialize AABB based on the grid's position and size
	theGrids.collisionBox.minimum.x = theGrids.position.x - theGrids.size.x * 0.5f;
	theGrids.collisionBox.minimum.y = theGrids.position.y - theGrids.size.y * 0.5f;
	theGrids.collisionBox.maximum.x = theGrids.position.x + theGrids.size.x * 0.5f;
	theGrids.collisionBox.maximum.y = theGrids.position.y + theGrids.size.y * 0.5f;

	//Implementing physics test
	theGrids.mass = 2.0f;
	theGrids.someoneOnTop = false;
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

	// *Color follows ARGB format (Alpha, Red, Green, Blue)
//Yellow Mesh
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, 0xFFFFFF00, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);  // top-left: blue

	AEGfxVertexList* pMeshBlack = AEGfxMeshEnd();
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

	//Temporary player
	player.position.x = 100;
	player.position.y = 400;
	player.size.x = AEGfxGetWindowWidth() * 0.025f; // *1.f means cover whole width, *0.1f means 10 tiles per 1600px width map, * 0.01f means 100 tiles
	player.size.y = AEGfxGetWindowWidth() * 0.025f;
	player.scale = { 0 };
	AEMtx33Scale(&player.scale, player.size.x, player.size.y);

	player.rotation = { 0 };
	AEMtx33Rot(&player.rotation, 0);

	AEMtx33Trans(&player.translation, player.position.x, player.position.y);

	player.transformation = { 0 };
	AEMtx33Concat(&player.transformation, &player.rotation, &player.scale);
	AEMtx33Concat(&player.transformation, &player.translation, &player.transformation);

	//Implementing physics test
	player.mass = 80.0f;
	player.onFloor = false;
	//Initializing grid data
	for (s16 rows = 0; rows < MAP_ROW_SIZE; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_SIZE; cols++)
		{
			switch (gameMap[rows][cols].symbol) //For checking the map symbol from the csv file
			{
			case 0:
				grids2D[rows][cols].typeOfGrid = EMPTY;
				break;
			case 1:
				grids2D[rows][cols].typeOfGrid = NORMAL_GROUND;
				break;
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

		//Just for limitation
		if (player.velocity.x > 200.f)
		{
			player.velocity.x = 200.f;
		}
		else if (player.velocity.x < -200.f)
		{
			player.velocity.x = -200;
		}


		if (AEInputCheckCurr(VK_LEFT))
		{
			player.velocity.x -= 25.f;
		}
		if (AEInputCheckCurr(VK_RIGHT))
		{
			player.velocity.x += 25.f;
		}
		 
		if (AEInputCheckTriggered(VK_SPACE) && player.onFloor)
		{
			std::cout << "JUMP" << std::endl;
			player.onFloor = false;
			player.velocity.y = 400.f;
			std::cout << player.velocity.y << std::endl;
		}

		if (player.onFloor)
		{
			player.velocity.x *= friction;
		}
		ApplyGravity(player); //Gravity application


		//Update player position via velocity
		player.position.x += player.velocity.x * static_cast<f32>(AEFrameRateControllerGetFrameTime());
		player.position.y += player.velocity.y * static_cast<f32>(AEFrameRateControllerGetFrameTime());

		//Resetting AABB box...
		player.collisionBox.minimum.x = player.position.x - player.size.x * 0.5f;
		player.collisionBox.minimum.y = player.position.y - player.size.y * 0.5f;
		player.collisionBox.maximum.x = player.position.x + player.size.x * 0.5f;
		player.collisionBox.maximum.y = player.position.y + player.size.y * 0.5f;

		//For printing the grids every frame
		for (s16 rows = 0; rows < MAP_ROW_SIZE; rows++)
		{
			for (s16 cols = 0; cols < MAP_COLUMN_SIZE; cols++)
			{
				switch (grids2D[rows][cols].typeOfGrid)
				{
				case NORMAL_GROUND:
					
					//Give up, ask shi heng to use ray casting instead
					if (AABBvsAABB(player.collisionBox, grids2D[rows][cols].collisionBox)) //Collision true
					{
						//Between player and grid only
						AEVec2 collisionNormal = NormalizeValue(player.collisionBox, grids2D[rows][cols].collisionBox);

						if (collisionNormal.x == 1 || collisionNormal.x == -1) // Vertical collision (floor)
						{
							ResolveHorizontalCollision(player, grids2D[rows][cols], &collisionNormal);
						}
						else // Horizontal collision
						{
							ResolveVerticalCollision(player, grids2D[rows][cols], &collisionNormal);
						}
					}
					AEGfxSetTransform(grids2D[rows][cols].transformation.m);
					AEGfxMeshDraw(pMeshYellow, AE_GFX_MDM_TRIANGLES);
					break;
				case EMPTY:
					AEGfxSetTransform(grids2D[rows][cols].transformation.m);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
					break;
				}
			}
		}

		AEMtx33Trans(&player.translation, player.position.x, player.position.y);
		AEGfxSetTransform(player.transformation.m);
		AEMtx33Concat(&player.transformation, &player.rotation, &player.scale);
		AEMtx33Concat(&player.transformation, &player.translation, &player.transformation);
		AEGfxMeshDraw(pMeshBlack, AE_GFX_MDM_TRIANGLES);


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
	AEGfxMeshFree(pMeshBlack);
	AEGfxDestroyFont(pFont);

	//Resizing vector, clear content, then resize it to 0
	gameMap.clear();
	gameMap.resize(0);
	/*-----------------------------------*/
	// free the system
	AESysExit();
}

