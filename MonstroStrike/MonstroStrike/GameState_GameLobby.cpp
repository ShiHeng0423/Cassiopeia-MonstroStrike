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
}


void Lobby_Load()
{
	//Memory allocation
	grids2D = new Grids2D * [MAP_ROW_LOBBY_SIZE];
	for (int i = 0; i < MAP_ROW_LOBBY_SIZE; ++i) {
		grids2D[i] = new Grids2D[MAP_COLUMN_LOBBY_SIZE];
	}

	player = gameManager->GetPlayer();
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
				player->GetPlayerCurrentPosition() = {grids2D[rows][cols].position}; //Set position based on grid
				break;
			default:
				break;
			}
		}
	}

	player->GetPlayerScale() = {grids2D[0][0].size.x * 1.25f, grids2D[0][0].size.y * 1.25f};

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

	player->RenderPlayer();

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//AEGfxSetTransform(ObjectTransformationMatrixSet(cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y, 0.f,
	//                                                (f32)AEGfxGetWindowWidth(), 1.f).m);
	//AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	//AEGfxSetTransform(ObjectTransformationMatrixSet(cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y,
	//                                                0.5f * PI, (f32)AEGfxGetWindowWidth(), 1.f).m);
	//AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	player->RenderPlayerStatUI();

	f32 x, y;
	AEGfxGetCamPosition(&x, &y);

	//Print Mission Name
	//Print 
	//Inventory images
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	if (Inventory::inventoryOpen)
	{

		f32 x, y;
		AEGfxGetCamPosition(&x, &y);

		AEGfxTextureSet(inventoryBackground.img.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(x, y, 0.f,
			inventoryBackground.img.scale.x,
			inventoryBackground.img.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);


		AEGfxTextureSet(equipmentBackground.img.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(
			equipmentBackground.pos.x + x,
			equipmentBackground.pos.y + y, 0.f,
			equipmentBackground.img.scale.x,
			equipmentBackground.img.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);


		for (ButtonGearUI button : inventoryButton)
		{
			AEGfxTextureSet(button.img.pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + x,
			                                                button.pos.y + y, 0.f,
			                                                button.img.scale.x, button.img.scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}

		f32 width, height;
		std::string playerStatsHeader = "Player Stats: ";
		auto pStats = playerStatsHeader.c_str();
		AEGfxGetPrintSize(fontID, pStats, 0.5f, &width, &height);
		AEGfxPrint(fontID, pStats, -0.75f,
		           0.45f - height * 0.5f,
		           0.35f, 1, 1, 1, 1);


		auto playerHealth = "Health: " + std::to_string(PlayerMaxBasehealth) + " + (" + std::to_string(
			playerReference->GetMaxHealth()) + ")";
		const char* pHealthText = playerHealth.c_str();
		AEGfxGetPrintSize(fontID, pHealthText, 0.5f, &width, &height);
		AEGfxPrint(fontID, pHealthText, -0.75f,
		           0.3f - height * 0.5f,
		           0.35f, 1, 1, 1, 1);

		auto playerAttack = "Attack: " + std::to_string(playerReference->GetWeaponSet().damage);
		const char* pAttackText = playerAttack.c_str();
		AEGfxGetPrintSize(fontID, pAttackText, 0.5f, &width, &height);
		AEGfxPrint(fontID, pAttackText, -0.75f,
		           0.1f - height * 0.5f,
		           0.35f, 1, 1, 1, 1);


		for (ButtonGearUI button : Inventory::equipmentDisplay)
		{
			AEGfxTextureSet(button.img.pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + x,
			                                                button.pos.y + y, 0.f,
			                                                button.img.scale.x, button.img.scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}


		//ItemInfoDisplay
		if (Inventory::itemHover)
		{
			//Inventory::DisplayItemInfo(Inventory::displayItem);

			AEGfxTextureSet(itemDisplayBackground.img.pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(
				itemDisplayBackground.pos.x + x,
				itemDisplayBackground.pos.y + y, 0.f,
				itemDisplayBackground.img.scale.x,
				itemDisplayBackground.img.scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

			f32 width, height;

			auto pText = Inventory::displayItem.Item.name.c_str();
			AEGfxGetPrintSize(fontID, pText, 0.5f, &width, &height);
			AEGfxPrint(fontID, pText, 0.35f,
			           0.45f - height * 0.5f,
			           0.4f, 1, 1, 1, 1);

			auto quantityString = "Qty: " + std::to_string(Inventory::displayItem.Item.quantity);
			const char* pText1 = quantityString.c_str();
			AEGfxGetPrintSize(fontID, pText1, 0.5f, &width, &height);
			AEGfxPrint(fontID, pText1, 0.35f,
			           0.35f - height * 0.5f,
			           0.35f, 1, 1, 1, 1);

			AEGfxTextureSet(blank, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(480.f + x,
			                                                90.f + y, 0.f,
			                                                Inventory::displayItem.img.scale.x * 2,
			                                                Inventory::displayItem.img.scale.y * 2).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);


			AEGfxTextureSet(Inventory::displayItem.img.pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(480.f + x,
			                                                90.f + y, 0.f,
			                                                Inventory::displayItem.img.scale.x * 2,
			                                                Inventory::displayItem.img.scale.y * 2).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

			std::string descriptionHeader = "Description: ";
			auto pDescription = descriptionHeader.c_str();
			AEGfxGetPrintSize(fontID, pDescription, 0.5f, &width, &height);
			AEGfxPrint(fontID, pDescription, 0.35f,
			           0.f - height * 0.5f,
			           0.35f, 1, 1, 1, 1);

			auto pText2 = Inventory::displayItem.Item.description.c_str();
			AEGfxGetPrintSize(fontID, pText2, 0.5f, &width, &height);
			AEGfxPrint(fontID, pText2, 0.35f,
			           -0.1f - height * 0.5f,
			           0.35f, 1, 1, 1, 1);

			if (Inventory::displayItem.Item.item_type != MATERIAL)
			{
				if ((Inventory::displayItem.Item.item_type == FOOD) || (Inventory::displayItem.Item.item_type ==
					POTION))
				{
					auto healthString = "Healing Amount: " + std::to_string(Inventory::displayItem.Item.health);
					const char* pText3 = healthString.c_str();
					AEGfxGetPrintSize(fontID, pText3, 0.5f, &width, &height);
					AEGfxPrint(fontID, pText3, 0.35f,
					           -0.25f - height * 0.5f,
					           0.35f, 1, 1, 1, 1);

					auto attackString = "Atk Buff: " + std::to_string(Inventory::displayItem.Item.attack);
					const char* pText4 = attackString.c_str();
					AEGfxGetPrintSize(fontID, pText4, 0.5f, &width, &height);
					AEGfxPrint(fontID, pText4, 0.35f,
					           -0.35f - height * 0.5f,
					           0.35f, 1, 1, 1, 1);
				}
				else
				{
					auto healthString = "Bonus HP: " + std::to_string(Inventory::displayItem.Item.health);
					const char* pText3 = healthString.c_str();
					AEGfxGetPrintSize(fontID, pText3, 0.5f, &width, &height);
					AEGfxPrint(fontID, pText3, 0.35f,
					           -0.25f - height * 0.5f,
					           0.35f, 1, 1, 1, 1);

					auto attackString = "Bonus Atk: " + std::to_string(Inventory::displayItem.Item.attack);
					const char* pText4 = attackString.c_str();
					AEGfxGetPrintSize(fontID, pText4, 0.5f, &width, &height);
					AEGfxPrint(fontID, pText4, 0.35f,
					           -0.35f - height * 0.5f,
					           0.35f, 1, 1, 1, 1);
				}
			}
		}
	}
#pragma endregion

	menu->Render();
	ParticlesDraw(*pWhiteSquareMesh);


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

	AEGfxTextureUnload(background);
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

	for (int i = 0; i < MAP_ROW_LOBBY_SIZE; ++i) {
		delete[] grids2D[i];
	}
	delete[] grids2D;
}
