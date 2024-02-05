#include "Level1.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "TransformMatrix.h"
#include "Player.h"
#include "Enemy.h"
#include <iostream>

#include "CSVMapLoader.h"
#include "GridTypesList.h"
#include "Physics.h"
#include "CollisionShape.h" //For Verticl + Horizontal collision

AEGfxVertexList* pWhiteSquareMesh;
AEGfxVertexList* pLineMesh;
AEGfxVertexList* pMeshYellow;
AEGfxVertexList* pMeshRed;
AEGfxVertexList* pMeshGrey;

Grids2D grids2D[MAP_ROW_SIZE][MAP_COLUMN_SIZE]; //Initializing map
std::vector<std::vector<MapCell>> gameMap(MAP_ROW_SIZE, std::vector<MapCell>(MAP_COLUMN_SIZE)); //Map for this level
Enemy* enemy[2];
Player* player;
AEGfxTexture* background;
void Level1_Load()
{

	enemy[0] = ENEMY_Init({100.f,100.f}, {500.f,0.f}, ENEMY_JUMPER, ENEMY_IDLE);
	enemy[1] = ENEMY_Init({100.f,100.f }, {-500.f,0.f}, ENEMY_FLY, ENEMY_IDLE);
	player = PlayerInitialize("Assets/Playerplaceholder.png", { 80.f,80.f }, { 0.f,0.f }, { 10.f,0.f }, true);
	background = AEGfxTextureLoad("Assets/background.jpg");
	const char* fileName = "Assets/GameMap.csv"; //Change name as per level
	//Load map
	if (MapLoader(fileName, gameMap, MAP_ROW_SIZE, MAP_COLUMN_SIZE))
	{
		PrintMap(gameMap, MAP_ROW_SIZE, MAP_COLUMN_SIZE); //Just for checking if the map data is stored properly
	}
	AEGfxMeshStart();

#pragma region Mesh Creations
	// *Color follows ARGB format (Alpha, Red, Green, Blue)
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFa9a9a9, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, 0xFFa9a9a9, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, 0xFFa9a9a9, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFa9a9a9, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, 0xFFa9a9a9, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, 0xFFa9a9a9, 0.0f, 0.0f);  // top-left: blue

	pMeshGrey = AEGfxMeshEnd();

	// *Color follows ARGB format (Alpha, Red, Green, Blue)
	//Red Mesh
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, 0xFFFF0000, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFF0000, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, 0xFFFF0000, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);  // top-left: blue

	 pMeshRed = AEGfxMeshEnd();

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

	pMeshYellow = AEGfxMeshEnd();

	//Drawing square
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh
	pWhiteSquareMesh = AEGfxMeshEnd();

	//Drawing line
	AEGfxMeshStart();
	AEGfxVertexAdd(-0.5f, 0.f, 0xFF000000, 1.0f, 1.0f);
	AEGfxVertexAdd(0.5f, 0.f, 0xFF000000, 1.0f, 1.0f);
	pLineMesh = AEGfxMeshEnd();
#pragma endregion

}

void Level1_Initialize()
{
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
	//Drawing first instance
	for (s16 rows = 0; rows < MAP_ROW_SIZE; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_SIZE; cols++)
		{
			grids2D[rows][cols].rowIndex = rows;
			grids2D[rows][cols].colIndex = cols;

			if (grids2D[rows][cols].typeOfGrid == NORMAL_GROUND)
			{
				InitializeGrid(grids2D[rows][cols]);
				//AEGfxSetTransform(grids2D[rows][cols].transformation.m);
				//AEGfxMeshDraw(pMeshYellow, AE_GFX_MDM_TRIANGLES);
			}
			else if (grids2D[rows][cols].typeOfGrid == EMPTY) //Maybe can just don't draw at all
			{
				InitializeGrid(grids2D[rows][cols]);
				//AEGfxSetTransform(grids2D[rows][cols].transformation.m);
				//AEGfxMeshDraw(pMeshGrey, AE_GFX_MDM_TRIANGLES);
			}
		}
	}
}

void Level1_Update()
{
	PlayerUpdate(*player);

	for (int i = 0; i < 2; ++i) {
		Enemy_Update_Choose(*enemy[i], *player);
	}







	if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
	{
		next = GameStates::Quit;
	}

	//For printing the grids every frame
	for (s16 rows = 0; rows < MAP_ROW_SIZE; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_SIZE; cols++)
		{
			switch (grids2D[rows][cols].typeOfGrid)
			{
			case NORMAL_GROUND:

				//Collision check
				//Resolve + Vertical Collision only for entity x (wall or ground)
				//Check vertical box (Head + Feet) 
				if (AABBvsAABB(player->boxHeadFeet, grids2D[rows][cols].collisionBox)) {
					 player->collisionNormal = AABBNormalize(player->boxHeadFeet, grids2D[rows][cols].collisionBox);

					 ResolveVerticalCollision(player->boxHeadFeet, grids2D[rows][cols].collisionBox, &player->collisionNormal, &player->obj.pos,
						 &player->velocity, &player->onFloor);
				}

				//Check horizontal box (Left arm -> Right arm)
				if (AABBvsAABB(player->boxArms, grids2D[rows][cols].collisionBox)) {
					player->collisionNormal = AABBNormalize(player->boxArms, grids2D[rows][cols].collisionBox);

					ResolveHorizontalCollision(player->boxArms, grids2D[rows][cols].collisionBox, &player->collisionNormal, &player->obj.pos,
						&player->velocity, &player->onFloor);
				}
				break;
			case EMPTY:
				break;
			}
		}
	}
}

void Level1_Draw()
{
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(background, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, 0.f, 0.f, 1920.f, 1080.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(player->obj.img.pTex, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(player->obj.pos.x,player->obj.pos.y,0.f, player->obj.img.scale.x, player->obj.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	for (int i = 0; i < 2; ++i) {
		AEGfxTextureSet(enemy[i]->obj.img.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(enemy[i]->obj.pos.x, enemy[i]->obj.pos.y, 0.f, enemy[i]->obj.img.scale.x, enemy[i]->obj.img.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	}




	AEVec2 cam;
	AEGfxGetCamPosition(&cam.x, &cam.y);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(ObjectTransformationMatrixSet(cam.x, cam.y , 0.f, AEGfxGetWindowWidth(), 1.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(ObjectTransformationMatrixSet(cam.x, cam.y, 0.5f * PI, AEGfxGetWindowWidth(), 1.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	//For Grid Drawing
	for (s16 rows = 0; rows < MAP_ROW_SIZE; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_SIZE; cols++)
		{
			switch (grids2D[rows][cols].typeOfGrid)
			{
			case NORMAL_GROUND:
				AEGfxSetTransform(grids2D[rows][cols].transformation.m);
				AEGfxMeshDraw(pMeshYellow, AE_GFX_MDM_TRIANGLES);
				break;
			case EMPTY:
				//AEGfxSetTransform(grids2D[rows][cols].transformation.m);
				//AEGfxMeshDraw(pMeshGrey, AE_GFX_MDM_TRIANGLES);
				break;
			}
		}
	}

}

void Level1_Free()
{
	for (int i = 0; i < 2; ++i) {
		Enemy_Free(enemy[i]);
	}

	//Free vectors
	gameMap.clear();
	gameMap.resize(0);

	//Free meshes
	AEGfxMeshFree(pMeshGrey);
	AEGfxMeshFree(pMeshYellow);
	AEGfxMeshFree(pMeshRed);
	AEGfxMeshFree(pLineMesh);
	AEGfxMeshFree(pWhiteSquareMesh);

}

void Level1_Unload()
{
}
