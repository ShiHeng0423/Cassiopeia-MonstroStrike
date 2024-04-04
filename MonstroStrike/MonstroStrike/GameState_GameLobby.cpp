#include "LevelHeaders.h"
#include "MapTransition.h"
#include "DebuggerManager.h"

namespace
{
	AEGfxVertexList* pLineMesh;
	AEGfxVertexList* pMeshYellow;
	AEGfxVertexList* pMeshRed;
	AEGfxVertexList* pMeshGrey;
	AEGfxVertexList* pWhiteSquareMesh;
	AEGfxVertexList* pGreenSquareMesh;


	Grids2D** grids2D;
	std::vector<std::vector<MapCell>> gameMap; //Map for this level
	std::vector<AEVec2> NPCPositions;

	Player* player;

	Camera* cam;
	PauseMenu_Manager* menu;
#pragma region UserInterface
	//User Health
	AEGfxVertexList* pMeshRedBar;
	AEGfxTexture* HealthBorder;

#pragma endregion UserInterface

	//bool inventory_open = false;
	// int snapBack = -1;

	int hp = 100;
	int gear_equipped = 0;

	float burningEffectDuration = 1.5f;
	f64 timer = 0.f;
}


void Lobby_Load()
{
	//Memory allocation
	grids2D = new Grids2D*[MAP_ROW_LOBBY_SIZE];
	for (int i = 0; i < MAP_ROW_LOBBY_SIZE; ++i)
	{
		grids2D[i] = new Grids2D[MAP_COLUMN_LOBBY_SIZE];
	}

	player = gameManager->GetPlayer();
	const char* fileName = "Assets/GameMaps/GameMap_Lobby.csv"; //Change name as per level

	//Load map
	if (MapLoader(fileName, gameMap, MAP_ROW_LOBBY_SIZE, MAP_COLUMN_LOBBY_SIZE))
	{
		PrintMap(gameMap, MAP_ROW_LOBBY_SIZE, MAP_COLUMN_LOBBY_SIZE);
		//Just for checking if the map data is stored properly
	}
	playerReference = player;
	Inventory::LoadInventory();
	Crafting::LoadRecipes();

	LoadNPC();
	ParticleLoad();

#pragma region Mesh Creations
	pMeshGrey = GenerateSquareMesh(0xFFa9a9a9);
	pMeshRed = GenerateSquareMesh(0xFFFF0000);
	pMeshRedBar = GenerateSquareMesh(0xFFFF0000);
	pMeshYellow = GenerateSquareMesh(0xFFFFFF00);
	pWhiteSquareMesh = GenerateSquareMesh(0xFFFFFFFF);
	pGreenSquareMesh = GenerateSquareMesh(0xFF00FF00);

	//Drawing line
	pLineMesh = GenerateLineMesh(0xFF000000);
#pragma endregion

	HealthBorder = AEGfxTextureLoad("Assets/UI_Sprite/Border/panel-border-015.png");

	menu = new PauseMenu_Manager();
}

void Lobby_Initialize()
{
	//Initializing grid data

	SetGridTypes(grids2D, gameMap, MAP_ROW_LOBBY_SIZE, MAP_COLUMN_LOBBY_SIZE);

	//For Initializing the grids and positions
	for (s16 rows = 0; rows < MAP_ROW_LOBBY_SIZE; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_LOBBY_SIZE; cols++)
		{
			grids2D[rows][cols].rowIndex = rows;
			grids2D[rows][cols].colIndex = cols;

			InitializeGrid(grids2D[rows][cols]);

			switch (gameMap[rows][cols].symbol)
			{
			//These are all NPCS
			case 10:
			case 11:
			case 12:
				NPCPositions.push_back(grids2D[rows][cols].position);
				break;
			case PLAYER_POS_GRID_1: //Only 1 transition player initial position here
				player->GetPlayerCurrentPosition() = {grids2D[rows][cols].position}; //Set position based on grid
				break;
			default:
				break;
			}
		}
	}

	player->GetPlayerScale() = {grids2D[0][0].size.x * 1.25f, grids2D[0][0].size.y * 1.25f};

	//Check if player previously just died
	if (player->GetPlayerJustDied())
	{
		player->GetPlayerJustDied() = false; //Set it to false
		player->GetCurrentHealth() = player->GetMaxHealth();
	}

	cam = new Camera(player->GetPlayerCurrentPosition());
	menu->Init(cam);
	cam->UpdatePos(player, grids2D[0][0].collisionBox.minimum.x,
	               grids2D[0][MAP_COLUMN_LOBBY_SIZE - 1].collisionBox.maximum.x,
	               grids2D[MAP_ROW_LOBBY_SIZE - 1][0].collisionBox.minimum.y, grids2D[0][0].collisionBox.maximum.y);
	//Initialize NPCs
	InitializeNPC(NPCPositions);

	//looping thru to init all enemy variables
	Inventory::InitInventory();
	ParticleInitialize();
	MapTransitionInit();
}

void Lobby_Update()
{
	MapTransitionUpdate();

	if (!player->GetIsTalkingToNpc())
	{
		menu->Update(cam);

		if (currScene == CurrentScene::PAUSE_SCENE || currScene == CurrentScene::CONTROL_SCENE || currScene ==
			CurrentScene::QUIT_SCENE)
			return;
		if (currScene == CurrentScene::MAIN_SCENE)
			player->Update(Inventory::inventoryOpen);
		cam->UpdatePos(player, grids2D[0][0].collisionBox.minimum.x,
		               grids2D[0][MAP_COLUMN_LOBBY_SIZE - 1].collisionBox.maximum.x,
		               grids2D[MAP_ROW_LOBBY_SIZE - 1][0].collisionBox.minimum.y, grids2D[0][0].collisionBox.maximum.y);

		player->CheckPlayerGridCollision(grids2D, MAP_ROW_LOBBY_SIZE, MAP_COLUMN_LOBBY_SIZE);

#pragma region InventorySystem
		if (AEInputCheckTriggered(AEVK_TAB))
		{
			Inventory::inventoryOpen = !Inventory::inventoryOpen;
			Inventory::itemHover = false;
		}

		if (Inventory::inventoryOpen)
		{
			Inventory::OpenInventory();
		}
#pragma endregion
	}

	UpdateNPC(player);
	ParticleUpdate();
}

void Lobby_Draw()
{
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	//For Grid Drawing
	RenderGrids(grids2D, MAP_ROW_LOBBY_SIZE, MAP_COLUMN_LOBBY_SIZE, *pWhiteSquareMesh);

	DrawNPC(*pWhiteSquareMesh);

	player->RenderPlayer();

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	player->RenderPlayerStatUI();

	f32 x, y;
	AEGfxGetCamPosition(&x, &y);

#pragma region Inventory_UI_Render
	Inventory::DrawInventory(pWhiteSquareMesh);
#pragma endregion

	menu->Render();
	ParticlesDraw(*pWhiteSquareMesh);
	
	//Print Mission Name
	missionSystem.PrintMissionText();

	DrawConvBox(player->GetIsTalkingToNpc(), *pWhiteSquareMesh);

	MapTransitionDraw();

}

void Lobby_Free()
{
	gameMap.clear();
	gameMap.resize(0);
	NPCPositions.clear();
	FreeNPC(); //Free both conv box and npc sprites here

	AEGfxSetCamPosition(0.f, 0.f);
	ParticlesFree();
}

void Lobby_Unload()
{
	Inventory::SaveInventory();
	Inventory::FreeInventory();

	AEGfxTextureUnload(HealthBorder);

	AEGfxMeshFree(pMeshGrey);
	AEGfxMeshFree(pMeshYellow);
	AEGfxMeshFree(pMeshRed);
	AEGfxMeshFree(pLineMesh);
	AEGfxMeshFree(pMeshRedBar);
	AEGfxMeshFree(pWhiteSquareMesh);
	AEGfxMeshFree(pGreenSquareMesh);

	delete cam;
	delete menu;

	for (int i = 0; i < MAP_ROW_LOBBY_SIZE; ++i)
	{
		delete[] grids2D[i];
	}
	delete[] grids2D;
}
