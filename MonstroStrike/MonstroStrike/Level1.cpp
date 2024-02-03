#include "Level1.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "TransformMatrix.h"
#include "Player.h"
#include <iostream>
#include "CSVMapLoader.h"
#include "GridTypesList.h"
#include "Physics.h"
#include "CollisionShape.h" //For Verticl + Horizontal collision

AEGfxVertexList* pWhiteSquareMesh1;
AEGfxVertexList* pWhiteSquareMesh;
AEGfxTexture* background;
Player* player;
AEGfxVertexList* pLineMesh;

void Level1_Load()
{
	player = PlayerInitialize("Assets/Kronii_Pixel.png", { 100.f,100.f }, { 0.f,0.f }, { 10.f,0.f }, true);
	background = AEGfxTextureLoad("Assets/background.jpg");

	AEGfxMeshStart();

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


	AEGfxMeshStart();
	AEGfxVertexAdd(-0.5f, 0.f, 0xFF000000, 1.0f, 1.0f);
	AEGfxVertexAdd(0.5f, 0.f, 0xFF000000, 1.0f, 1.0f);
	pLineMesh = AEGfxMeshEnd();
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
	if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
	{
		next = GameStates::Quit;
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

	AEVec2 cam;
	AEGfxGetCamPosition(&cam.x, &cam.y);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(ObjectTransformationMatrixSet(cam.x, cam.y , 0.f, AEGfxGetWindowWidth(), 1.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(ObjectTransformationMatrixSet(cam.x, cam.y, 0.5f * PI, AEGfxGetWindowWidth(), 1.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

}

void Level1_Free()
{
}

void Level1_Unload()
{
}
