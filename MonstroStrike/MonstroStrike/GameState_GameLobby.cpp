#include "LevelHeaders.h"
#include "MapTransition.h"

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

	AEGfxTexture* background;

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

	void CheckPlayerGridCollision(Grids2D** gridMap, Player* player);
}


void Lobby_Load()
{
	//Memory allocation
	grids2D = new Grids2D * [MAP_ROW_LOBBY_SIZE];
	for (int i = 0; i < MAP_ROW_LOBBY_SIZE; ++i) {
		grids2D[i] = new Grids2D[MAP_COLUMN_LOBBY_SIZE];
	}

	player = PlayerInitialize("Assets/Border.png", {0.f, 0.f}, {0.f, 0.f}, {40.f, 0.f}, true);
	background = AEGfxTextureLoad("Assets/Background2.jpg");
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
				player->obj.pos = {grids2D[rows][cols].position}; //Set position based on grid
				break;
			default:
				break;
			}
		}
	}

	player->obj.scale = {grids2D[0][0].size.x * 1.25f, grids2D[0][0].size.y * 1.25f};

	cam = new Camera(player->obj.pos);
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

	if (!player->isConversation)
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

		CheckPlayerGridCollision(grids2D, player);

		if (AEInputCheckTriggered(AEVK_TAB))
		{
			Inventory::inventoryOpen = !Inventory::inventoryOpen;
		}

		if (Inventory::inventoryOpen)
		{
			Inventory::OpenInventory();
		}
	}

	UpdateNPC(player);
	ParticleUpdate();
	MapTransitionUpdate();
}

void Lobby_Draw()
{
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(background, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, 0.f, 0.f, 4200, 1080.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	//For Grid Drawing
	RenderGrids(grids2D, MAP_ROW_LOBBY_SIZE, MAP_COLUMN_LOBBY_SIZE, *pWhiteSquareMesh);

	DrawNPC(*pWhiteSquareMesh);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(player->obj.pTex, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(player->obj.pos.x, player->obj.pos.y, 0.f, player->obj.scale.x,
	                                                player->obj.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(ObjectTransformationMatrixSet(cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y, 0.f,
	                                                (f32)AEGfxGetWindowWidth(), 1.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(ObjectTransformationMatrixSet(cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y,
	                                                0.5f * PI, (f32)AEGfxGetWindowWidth(), 1.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	f32 x, y;
	AEGfxGetCamPosition(&x, &y);

	AEGfxSetTransform(ObjectTransformationMatrixSet(-800.f + (int)player->currHealth + x, 450.f + y, 0,
	                                                (int)player->currHealth * 2.f, 80.f).m);
	AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(HealthBorder, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(-800.f + (int)player->maxHealth + x, 450.f + y, 0,
	                                                (int)player->maxHealth * 2.f, 80.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	std::string str = std::to_string((int)player->currHealth);
	const char* pTextHP = str.c_str();
	f32 width, height;
	AEGfxGetPrintSize(fontID, pTextHP, 0.5f, &width, &height);
	AEGfxPrint(fontID, pTextHP, -width / 2 - 0.9f, -width / 2 + 0.97f, 0.5f, 1, 1, 1, 1);

	//Print Mission Name
	//Print 
	//Inventory images
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	
	if (Inventory::inventoryOpen)
	{
		f32 x, y;
		AEGfxGetCamPosition(&x, &y);

		AEGfxTextureSet(inventoryBackground.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(x, y, 0.f,
		                                                inventoryBackground.scale.x,
		                                                inventoryBackground.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);


		AEGfxTextureSet(equipmentBackground.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(
			equipmentBackground.pos.x + x,
			equipmentBackground.pos.y + y, 0.f,
			equipmentBackground.scale.x,
			equipmentBackground.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);


		for (ButtonGearUI button : inventoryButton)
		{
			if (button.Item.ID < 0)
			{
				AEGfxTextureSet(button.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + x,
				                                                button.pos.y + y, 0.f,
				                                                button.img.scale.x, button.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}
		}


		for (ButtonGearUI button : inventoryButton)
		{
			if (button.Item.ID >= 0)
			{
				AEGfxTextureSet(button.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + x,
				                                                button.pos.y + y, 0.f,
				                                                button.img.scale.x, button.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}
		}

		for (ButtonGearUI button : Inventory::equipmentDisplay)
		{
			if (button.Item.ID < 0)
			{
				AEGfxTextureSet(button.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + x,
				                                                button.pos.y + y, 0.f,
				                                                button.img.scale.x, button.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}
		}

		for (ButtonGearUI button : Inventory::equipmentDisplay)
		{
			if (button.Item.ID >= 0)
			{
				AEGfxTextureSet(button.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + x,
				                                                button.pos.y + y, 0.f,
				                                                button.img.scale.x, button.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}
		}


		//ItemInfoDisplay
		if (Inventory::itemHover)
		{
			AEGfxTextureSet(itemDisplayBackground.pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(
				itemDisplayBackground.pos.x + x,
				itemDisplayBackground.pos.y + y, 0.f,
				itemDisplayBackground.scale.x,
				itemDisplayBackground.scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

			f32 width, height;

			auto pText = Inventory::displayItem.name.c_str();
			AEGfxGetPrintSize(fontID, pText, 0.5f, &width, &height);
			AEGfxPrint(fontID, pText, 0.75f - width * 0.5f,
			           0.5f - height * 0.5f,
			           0.25f, 1, 1, 1, 1);

			auto pText1 = "Load";
			AEGfxGetPrintSize(fontID, pText1, 0.5f, &width, &height);
			AEGfxPrint(fontID, pText1, -width / 2,
			           -height / 2, 0.5f, 1, 1, 1, 1);

			auto pText2 = "Credit";
			AEGfxGetPrintSize(fontID, pText2, 0.5f, &width, &height);
			AEGfxPrint(fontID, pText2, -width / 2,
			           -height / 2, 0.5f, 1, 1, 1, 1);

			auto pText3 = "Controls";
			AEGfxGetPrintSize(fontID, pText3, 0.5f, &width, &height);
			AEGfxPrint(fontID, pText3, -itemDisplayBackground.pos.x + x,
			           -itemDisplayBackground.pos.y + y - 0.44f, 0.5f, 1, 1, 1, 1);
		}


		for (ButtonGearUI button : inventoryButton)
		{
			if (button.Item.ID < 0)
			{
				AEGfxTextureSet(button.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + x,
				                                                button.pos.y + y, 0.f,
				                                                button.scale.x, button.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}
		}


		for (ButtonGearUI button : inventoryButton)
		{
			if (button.Item.ID >= 0)
			{
				AEGfxTextureSet(button.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + x,
				                                                button.pos.y + y, 0.f,
				                                                button.scale.x, button.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}
		}

		for (ButtonGearUI button : Inventory::equipmentDisplay)
		{
			if (button.Item.ID < 0)
			{
				AEGfxTextureSet(button.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + x,
				                                                button.pos.y + y, 0.f,
				                                                button.scale.x, button.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}
		}

		for (ButtonGearUI button : Inventory::equipmentDisplay)
		{
			if (button.Item.ID >= 0)
			{
				AEGfxTextureSet(button.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + x,
				                                                button.pos.y + y, 0.f,
				                                                button.scale.x, button.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}
		}
	}

	int index = 1;
	for (ButtonGearUI button : Inventory::equipmentDisplay)
	{
		AEGfxTextureSet(button.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(AEGfxGetWinMinX() + 80.f * index++,
			AEGfxGetWinMinY() +50.f, 0.f,
			button.scale.x, button.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	}

	menu->Render();
	ParticlesDraw(*pWhiteSquareMesh);


	missionSystem.PrintMissionText();

	DrawConvBox(player->isConversation, *pWhiteSquareMesh);

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

	AEGfxTextureUnload(background);
	AEGfxTextureUnload(HealthBorder);

	AEGfxMeshFree(pMeshGrey);
	AEGfxMeshFree(pMeshYellow);
	AEGfxMeshFree(pMeshRed);
	AEGfxMeshFree(pLineMesh);
	AEGfxMeshFree(pMeshRedBar);
	AEGfxMeshFree(pWhiteSquareMesh);
	AEGfxMeshFree(pGreenSquareMesh);

	delete player;
	delete cam;
	delete menu;

	for (int i = 0; i < MAP_ROW_LOBBY_SIZE; ++i) {
		delete[] grids2D[i];
	}
	delete[] grids2D;
}

//Private function definition
namespace {
	void CheckPlayerGridCollision(Grids2D** gridMap, Player* player)
	{
		int playerIndexY = (int)((AEGfxGetWindowHeight() * 0.5f - player->obj.pos.y) / (gridMap[0][0].size.x));

		for (int i = 0; i <= (int)(player->obj.img.scale.x * 2 / gridMap[0][0].size.x); i++)
		{
			int playerIndexX = (int)((player->obj.pos.x + AEGfxGetWindowWidth() * 0.5f) / (gridMap[0][0].size.x));
			for (int j = 0; j <= (int)(player->obj.img.scale.x * 2 / gridMap[0][0].size.x); j++)
			{
				switch (gridMap[playerIndexY][playerIndexX].typeOfGrid)
				{
				case NORMAL_GROUND:
					//Collision check
					//Resolve + Vertical Collision only for entity x (wall or ground)
					//Check vertical box (Head + Feet) 
					if (AABBvsAABB(player->boxHeadFeet, gridMap[playerIndexY][playerIndexX].collisionBox))
					{
						player->collisionNormal = AABBNormalize(player->boxHeadFeet,
							gridMap[playerIndexY][playerIndexX].collisionBox);
						ResolveVerticalCollision(player->boxHeadFeet, gridMap[playerIndexY][playerIndexX].collisionBox,
							&player->collisionNormal, &player->obj.pos,
							&player->velocity, &player->onFloor, &player->gravityForce,
							&player->isFalling);
					}

					//Check horizontal box (Left arm -> Right arm)
					if (AABBvsAABB(player->boxArms, gridMap[playerIndexY][playerIndexX].collisionBox))
					{
						player->collisionNormal = AABBNormalize(player->boxArms,
							gridMap[playerIndexY][playerIndexX].collisionBox);
						ResolveHorizontalCollision(player->boxArms, gridMap[playerIndexY][playerIndexX].collisionBox,
							&player->collisionNormal, &player->obj.pos,
							&player->velocity);
					}
					break;
				case MAP_TRANSITION_GRID_1:
					if (AABBvsAABB(player->collisionBox, gridMap[playerIndexY][playerIndexX].collisionBox))
					{
						//std::cout << "Collided\n";MainMenu_Song
						if (!transitionalImageOBJ.active)
						{
							transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1_A);
						}
					}
					break;
				case EMPTY:
					break;
				}
				playerIndexX += 1;
			}
			playerIndexY += 1;
		}
	}
}
