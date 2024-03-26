#pragma once

#include "LevelHeaders.h"
#include "MapTransition.h"
#include "GameStateHeaders.h"

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

	std::vector<struct Platforms> platformVectors;

	std::vector<Enemy> vecEnemy; //enemy container
	std::vector<EnemyDrops> vecCollect;

	Player* player;

	AEGfxTexture* background;

	Camera* cam;
	PauseMenu_Manager* menu;


#pragma region UserInterface
	//User Health
	AEGfxVertexList* pMeshRedBar;
	AEGfxTexture* HealthBorder;

#pragma endregion UserInterface

	s16 hp = 100;
	s16 gear_equipped = 0;

	f32 burningEffectDuration = 1.5f;
	f64 timer = 0.f;


	AEVec2 playerBoundaryMin;
	AEVec2 playerBoundaryMax;


	void CheckPlayerGridCollision(Grids2D** gridMap, Player* player);

	void CheckEnemyGridCollision(Grids2D** gridMap, std::vector<Enemy>& enemy);
}




void Level1_E_Load()
{
	grids2D = new Grids2D * [MAP_ROW_SIZE];
	for (int i = 0; i < MAP_ROW_SIZE; ++i) {
		grids2D[i] = new Grids2D[MAP_COLUMN_SIZE];
	}

	bulletTex = AEGfxTextureLoad("Assets/RedCircle.png");

	player = new Player("Assets/Border.png", { AEGfxGetWindowWidth() * 0.05f, AEGfxGetWindowWidth() * 0.05f },
		{ 0, -100 }, { 40.f, 0.f }, true);
	playerReference = player;
	background = AEGfxTextureLoad("Assets/Background2.jpg");
	const char* fileName = "Assets/GameMaps/GameMap_Level1_E.csv"; //Change name as per level
	//Load map
	if (MapLoader(fileName, gameMap, MAP_ROW_SIZE, MAP_COLUMN_SIZE))
	{
		PrintMap(gameMap, MAP_ROW_SIZE, MAP_COLUMN_SIZE); //Just for checking if the map data is stored properly
	}

	//Inventory assets
	Inventory::LoadInventory();


	HealthBorder = AEGfxTextureLoad("Assets/UI_Sprite/Border/panel-border-015.png");

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

	ParticleLoad();

	menu = new PauseMenu_Manager();
}

void Level1_E_Initialize()
{
#pragma region Grid_Loading
	//Initializing grid data
	SetGridTypes(grids2D, gameMap, MAP_ROW_SIZE, MAP_COLUMN_SIZE);

	//For Initializing the grids and positions
	for (s16 rows = 0; rows < MAP_ROW_SIZE; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_SIZE; cols++)
		{
			grids2D[rows][cols].rowIndex = rows;
			grids2D[rows][cols].colIndex = cols;

			InitializeGrid(grids2D[rows][cols]);

			//Check if previous zone is the next zone
			if (grids2D[rows][cols].typeOfGrid == VERTICAL_PLATFORM_POS)
			{
				CreatePlatform(grids2D[rows][cols].position.x, grids2D[rows][cols].position.y,
					GRID_SIZE * 3.f, GRID_SIZE, 2.f, VERTICAL_MOVING_PLATFORM, platformVectors);
			}
			else if (grids2D[rows][cols].typeOfGrid == DIAGONAL_PLATFORM_POS)
			{
				CreatePlatform(grids2D[rows][cols].position.x, grids2D[rows][cols].position.y,
					GRID_SIZE * 3.f, GRID_SIZE, 2.f, DIAGONAL_PLATFORM, platformVectors);
			}
			else if (grids2D[rows][cols].typeOfGrid == HORIZONTAL_PLATFORM_POS)
			{
				CreatePlatform(grids2D[rows][cols].position.x, grids2D[rows][cols].position.y,
					GRID_SIZE * 3.f, GRID_SIZE, 2.f, HORIZONTAL_MOVING_PLATFORM, platformVectors);
			}
			//Previous zone is level 1 part B
			if (grids2D[rows][cols].typeOfGrid == PLAYER_POS_GRID_1 && previous == AREA1_D)
			{
				player->obj.pos = { grids2D[rows][cols].position }; //Set position based on grid
			}
			//Previous zone is level 1 part D
			else if (grids2D[rows][cols].typeOfGrid == PLAYER_POS_GRID_2 && previous == AREA1_F)
			{
				//Set initial player position at pos grid 2
				player->obj.pos = { grids2D[rows][cols].position }; //Set position based on grid
			}
		}
	}
	player->obj.scale = { grids2D[0][0].size.x * 1.25f, grids2D[0][0].size.y * 1.25f };

#pragma endregion

	cam = new Camera(player->obj.pos);

#pragma region Inventory_UI

	Inventory::InitInventory();

#pragma endregion

	menu->Init(cam);
	ParticleInitialize();
	MapTransitionInit();
}

void Level1_E_Update()
{
	//std::cout << AEFrameRateControllerGetFrameRate() << "\n";
	MapTransitionUpdate();

#pragma region PauseMenuTrigger

	menu->Update(cam);

#pragma endregion

	if (currScene == CurrentScene::PAUSE_SCENE || currScene == CurrentScene::CONTROL_SCENE || currScene ==
		CurrentScene::QUIT_SCENE)
		return;

#pragma region PlayerUpdate
	if (currScene == MAIN_SCENE)
		player->Update(Inventory::inventoryOpen);
	if (AEInputCheckTriggered(AEVK_I))
	{
		Inventory::inventoryOpen = !Inventory::inventoryOpen;
	}

	if (AEInputCheckTriggered(AEVK_0))
	{
		//next = GameStates::Quit;
		AEVec2 test{ 100.f, 100.f };
		cam->LookAhead(test);
	}
	if (AEInputCheckCurr(AEVK_1))
	{
		cam->CameraShake();
	}

	//This is set here temporary so that thing actually work, need to move
	if (player->isAttacking)
	{
		for (Enemy& enemy : vecEnemy)
		{
			if (enemy.isAlive)
			{
				CheckWeaponCollision(&player->equippedWeapon, enemy, *player);
			}
		}
	}

#pragma endregion

#pragma region EnemyUpdate
	AllEnemyUpdate(vecEnemy, *player, vecCollect);
#pragma endregion

#pragma region GridSystem
	CheckPlayerGridCollision(grids2D, player);
	//CheckEnemyGridCollision(grids2D, vecEnemy);

	for (s16 rows = 0; rows < MAP_ROW_SIZE; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_SIZE; cols++)
		{
			switch (grids2D[rows][cols].typeOfGrid)
			{
			case NORMAL_GROUND:
				AllEnemyNBulletCollisionCheck(vecEnemy, grids2D[rows][cols].collisionBox);
				break;
			}
		}
	}
#pragma endregion

#pragma region InventorySystem

	if (Inventory::inventoryOpen)
	{
		Inventory::OpenInventory();

		if (Inventory::itemHover)
		{
			//Display item info
		}
	}

	if (player->burningEffect)
	{
		if (timer >= burningEffectDuration)
		{
			AEVec2 hpLerp{ 0, 0 };
			hpLerp.x = (f32)hp;
			hpLerp.y = 0.f;
			AEVec2 end{ 0, 0 };
			end.x = 0;
			end.y = 0;
			AEVec2Lerp(&hpLerp, &hpLerp, &end, 0.01f);
			hp = (int)hpLerp.x;
			timer = 0.f;
		}
		else
			timer += AEFrameRateControllerGetFrameTime();
	}

#pragma endregion

#pragma region PlatformSystem
	//Testing moving platform logic
	UpdatePlatforms(*player, vecEnemy, platformVectors); //Numbers based on how many moving platforms

#pragma endregion

#pragma region ParticleSystem

	ParticleUpdate();

#pragma endregion

#pragma region CameraUpdate

	cam->UpdatePos(player, grids2D[0][0].collisionBox.minimum.x,
		grids2D[0][MAP_COLUMN_SIZE - 1].collisionBox.maximum.x,
		grids2D[MAP_ROW_SIZE - 1][0].collisionBox.minimum.y, grids2D[0][0].collisionBox.maximum.y);

#pragma endregion
}

void Level1_E_Draw()
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

#pragma region Background_Render

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxTextureSet(background, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, 0.f, 0.f, 4200, 1080.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

#pragma endregion


#pragma region Grid_Render

	RenderGrids(grids2D, MAP_ROW_SIZE, MAP_COLUMN_SIZE, *pWhiteSquareMesh);

	for (s16 i = 0; i < platformVectors.size(); i++)
	{
		AEGfxSetTransform(platformVectors[i].transformation.m);
		AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);
	}

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

#pragma region Player_Render

	AEGfxTextureSet(player->obj.pTex, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(player->obj.pos.x, player->obj.pos.y, 0.f, player->obj.scale.x,
		player->obj.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	//drawing enemy
	AllEnemyDraw(vecEnemy, pWhiteSquareMesh, vecCollect);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	if (player->isAttacking)
	{
		AEGfxSetTransform(ObjectTransformationMatrixSet(player->equippedWeapon.position.x,
			player->equippedWeapon.position.y, 0.f,
			player->equippedWeapon.scale.x,
			player->equippedWeapon.scale.y).m);
		AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);
		player->isAttacking = false;
	}

#pragma endregion

#pragma region Game_UI_Render

	f32 x, y;
	AEGfxGetCamPosition(&x, &y);

	AEGfxSetTransform(ObjectTransformationMatrixSet(-800.f + hp + x, 450.f + y, 0, hp * 2.f, 80.f).m);
	AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(HealthBorder, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(-800.f + hp + x, 450.f + y, 0, hp * 2.f, 80.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	std::string str = std::to_string(hp);
	const char* pTextHP = str.c_str();
	f32 width, height;
	AEGfxGetPrintSize(fontID, pTextHP, 0.5f, &width, &height);
	AEGfxPrint(fontID, pTextHP, -width / 2 - 0.9f, -width / 2 + 0.97f, 0.5f, 1, 1, 1, 1);

#pragma endregion

#pragma region Inventory_UI_Render

	//Inventory images
	if (Inventory::inventoryOpen)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

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

		if (Inventory::itemHover)
		{
			AEGfxTextureSet(itemDisplayBackground.img.pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(
				itemDisplayBackground.pos.x + x,
				itemDisplayBackground.pos.y + y, 0.f,
				itemDisplayBackground.img.scale.x,
				itemDisplayBackground.img.scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}


		for (ButtonGearUI button : inventoryButton)
		{
			//Filled slots
			if (button.Item.ID >= 0)
			{
				AEGfxTextureSet(button.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + x,
					button.pos.y + y, 0.f,
					button.img.scale.x, button.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}

			//Empty slots
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
			//Filled slots
			if (button.Item.ID >= 0)
			{
				AEGfxTextureSet(button.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + x,
					button.pos.y + y, 0.f,
					button.img.scale.x, button.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}

			//empty slots
			if (button.Item.ID < 0)
			{
				AEGfxTextureSet(button.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + x,
					button.pos.y + y, 0.f,
					button.img.scale.x, button.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}
		}
	}
#pragma endregion

	menu->Render();

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

#pragma region Particle_Render

	ParticlesDraw(*pWhiteSquareMesh);

#pragma endregion


#pragma region Center_Line_Render

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	AEGfxSetTransform(ObjectTransformationMatrixSet(cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y, 0.f,
		(f32)AEGfxGetWindowWidth(), 1.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(ObjectTransformationMatrixSet(cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y,
		0.5f * PI, (f32)AEGfxGetWindowWidth(), 1.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

#pragma endregion
	missionSystem.PrintMissionText();
	MapTransitionDraw();
}

void Level1_E_Free()
{
	FreeEnemy(vecEnemy); //loops thru all eney tex and free them.
	//Free Enemy Vector
	vecEnemy.clear();
	vecEnemy.resize(0);
	vecCollect.clear();
	vecCollect.resize(0);
	//Free vectors
	gameMap.clear();
	gameMap.resize(0);
	platformVectors.clear();
	platformVectors.resize(0);

	AEGfxSetCamPosition(0.f, 0.f);
	ParticlesFree();
}

void Level1_E_Unload()
{
	Inventory::SaveInventory();
	Inventory::FreeInventory();

	AEGfxTextureUnload(background);
	AEGfxTextureUnload(HealthBorder);
	AEGfxTextureUnload(bulletTex);
	AEGfxTextureUnload(enemyJumperDropTex);
	AEGfxTextureUnload(enemyChargerDropTex);
	AEGfxTextureUnload(enemyFlyDropTex);
	AEGfxTextureUnload(enemyBoss1DropTex);

	AEGfxTextureUnload(player->obj.pTex);

	AEGfxMeshFree(pMeshGrey);
	AEGfxMeshFree(pMeshYellow);
	AEGfxMeshFree(pMeshRed);
	AEGfxMeshFree(pLineMesh);
	AEGfxMeshFree(pMeshRedBar);
	AEGfxMeshFree(pWhiteSquareMesh);
	AEGfxMeshFree(pGreenSquareMesh);
	for (int i = 0; i < MAP_ROW_SIZE; ++i) {
		delete[] grids2D[i];
	}

	delete[] grids2D;
	delete player;
	delete cam;
	delete menu;
}

namespace {
	void CheckPlayerGridCollision(Grids2D** gridMap, Player* player)
	{
		int playerIndexY = (int)((AEGfxGetWindowHeight() * 0.5f - player->obj.pos.y) / (gridMap[0][0].size.x));

		for (int i = 0; i <= (int)(player->obj.scale.x * 2 / gridMap[0][0].size.x); i++)
		{
			int playerIndexX = (int)((player->obj.pos.x + AEGfxGetWindowWidth() * 0.5f) / (gridMap[0][0].size.x));
			for (int j = 0; j <= (int)(player->obj.scale.x * 2 / gridMap[0][0].size.x); j++)
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
				case LAVA_GRID:
					if (AABBvsAABB(player->collisionBox, gridMap[playerIndexY][playerIndexX].collisionBox))
					{
						OnPlayerDeath();
					}
					break;
				case MAP_TRANSITION_GRID_1:
					if (AABBvsAABB(player->collisionBox, gridMap[playerIndexY][playerIndexX].collisionBox))
					{
						//std::cout << "Collided\n";MainMenu_Song
						if (!transitionalImageOBJ.active)
						{
							transitionalImageOBJ.PlayMapTransition(TRANSITION_RIGHT, AREA1_D);
						}
					}
					break;
				case MAP_TRANSITION_GRID_2:
					if (AABBvsAABB(player->collisionBox, gridMap[playerIndexY][playerIndexX].collisionBox))
					{
						//std::cout << "Collided\n";MainMenu_Song
						if (!transitionalImageOBJ.active)
						{
							transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1_F);
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

	void CheckEnemyGridCollision(Grids2D** gridMap, std::vector<Enemy>& enemy)
	{
		for (Enemy& tmpEnemy : enemy)
		{

			int enemyIndexY = (int)((AEGfxGetWindowHeight() * 0.5f - tmpEnemy.obj.pos.y) / (gridMap[0][0].size.x));

			for (int i = 0; i <= (int)(tmpEnemy.obj.scale.x * 2 / gridMap[0][0].size.x); i++)
			{
				int enemyIndexX = (int)((tmpEnemy.obj.pos.x + AEGfxGetWindowWidth() * 0.5f) / (gridMap[0][0].size.x));
				for (int j = 0; j <= (int)(tmpEnemy.obj.scale.x * 2 / gridMap[0][0].size.x); j++)
				{

					//Check vertical box (Head + Feet) 
					if (AABBvsAABB(tmpEnemy.boxHeadFeet, gridMap[enemyIndexY][enemyIndexX].collisionBox))
					{
						tmpEnemy.collisionNormal = AABBNormalize(tmpEnemy.boxHeadFeet, gridMap[enemyIndexY][enemyIndexX].collisionBox);

						ResolveVerticalCollision(tmpEnemy.boxHeadFeet, gridMap[enemyIndexY][enemyIndexX].collisionBox,
							&tmpEnemy.collisionNormal, &tmpEnemy.obj.pos,
							&tmpEnemy.velocity, &tmpEnemy.onFloor, &tmpEnemy.gravityForce,
							&tmpEnemy.isFalling);
					}
					//Check horizontal box (Left arm -> Right arm)
					if (AABBvsAABB(tmpEnemy.boxArms, gridMap[enemyIndexY][enemyIndexX].collisionBox))
					{
						tmpEnemy.isCollision = true;
						tmpEnemy.collisionNormal = AABBNormalize(tmpEnemy.boxArms, gridMap[enemyIndexY][enemyIndexX].collisionBox);

						ResolveHorizontalCollision(tmpEnemy.boxArms, gridMap[enemyIndexY][enemyIndexX].collisionBox, &tmpEnemy.collisionNormal, &tmpEnemy.obj.pos,
							&tmpEnemy.velocity);
						tmpEnemy.loopIdle = false;
					}
				}
			}
		}
	}

}

