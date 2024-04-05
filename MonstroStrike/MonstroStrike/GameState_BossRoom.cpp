/*!************************************************************************
  \file					GameState_BossRoom.cpp
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (32.5%)
  \secondary authors	Teng Shi Heng (32.5%), Goh Jun Jie (15%), Keith Ng Hon Yew (15%), Choo Jian Wei (5%)
  \brief				File containing definitions of functions related to the boss room state. Such as loading,
						initializing, update, drawing etc... of the level.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

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

	std::vector<struct Platforms> platformVectors;

	std::vector<Enemy> vecEnemy; //enemy container
	std::vector<EnemyDrops> vecCollect;

	Player* player;

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

	void CheckEnemyGridCollision(Grids2D** gridMap, std::vector<Enemy>& enemy);

	bool isShaking;
	bool isLookingToBoss;
}

void Level1_BOSS_Load()
{
	grids2D = new Grids2D*[MAP_COLUMN_BOSS_SIZE];
	for (int i = 0; i < MAP_ROW_BOSS_SIZE; ++i)
	{
		grids2D[i] = new Grids2D[MAP_COLUMN_BOSS_SIZE];
	}

	Enemy_Load(ENEMY_BOSS1, vecEnemy);
	bulletTex = AEGfxTextureLoad("Assets/RedCircle.png");
	enemyBoss1DropTex = AEGfxTextureLoad("Assets/ENEMY_BOSS1_DROP.png");
	player = gameManager->GetPlayer();
	playerReference = player;
	const char* fileName = "Assets/GameMaps/GameMap_BossStage.csv"; //Change name as per level
	//Load map
	if (MapLoader(fileName, gameMap, MAP_ROW_BOSS_SIZE, MAP_COLUMN_BOSS_SIZE))
	{
		//PrintMap(gameMap, MAP_ROW_BOSS_SIZE, MAP_COLUMN_BOSS_SIZE);
		//Just for checking if the map data is stored properly
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
	PreLoadTrapsTexture();

	menu = new PauseMenu_Manager();
}

void Level1_BOSS_Initialize()
{
#pragma region Grid_Loading
	//Initializing grid data
	SetGridTypes(grids2D, gameMap, MAP_ROW_BOSS_SIZE, MAP_COLUMN_BOSS_SIZE);

	//For Initializing the grids and positions
	for (s16 rows = 0; rows < MAP_ROW_BOSS_SIZE; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_BOSS_SIZE; cols++)
		{
			grids2D[rows][cols].rowIndex = rows;
			grids2D[rows][cols].colIndex = cols;

			InitializeGrid(grids2D[rows][cols]);
			StoreTrapDetails(grids2D[rows][cols]); //Add any traps if any

			//Previous zone is area a
			if (grids2D[rows][cols].typeOfGrid == PLAYER_POS_GRID_1)
			{
				player->GetPlayerCurrentPosition() = { grids2D[rows][cols].position }; //Set position based on grid
			}
		}
	}
	player->GetPlayerScale() = { grids2D[0][0].size.x * 1.25f, grids2D[0][0].size.y * 1.25f };

#pragma endregion

	cam = new Camera(player->GetPlayerCurrentPosition());

#pragma region Inventory_UI

	Inventory::InitInventory();

#pragma endregion

	Enemy_Init({ 70.f, 70.f }, { 225.f, -500.f }, ENEMY_IDLE, vecEnemy[0]);

	menu->Init(cam);
	ParticleInitialize();
	MapTransitionInit();

	cam->CameraShake(3.f);
	isShaking = true;
	isLookingToBoss = false;
}

void Level1_BOSS_Update()
{
	MapTransitionUpdate();

#pragma region PauseMenuTrigger

	menu->Update();

#pragma endregion

	if (currScene == CurrentScene::PAUSE_SCENE || currScene == CurrentScene::CONTROL_SCENE || currScene ==
		CurrentScene::QUIT_SCENE)
		return;

#pragma region PlayerUpdate
	if (currScene == MAIN_SCENE)
		player->Update(Inventory::inventoryOpen);
	if (AEInputCheckTriggered(AEVK_TAB))
	{
		Inventory::inventoryOpen = !Inventory::inventoryOpen;
		Inventory::itemHover = false;
		audioManager->PlayAudio(false, Audio_List::INVENTORY_OPEN);
	}

	//This is set here temporary so that thing actually work, need to move
	if (player->GetIsPlayerAttacking())
	{
		for (Enemy& enemy : vecEnemy)
		{
			if (enemy.isAlive)
			{
				Weapon_System::CheckWeaponCollision(&player->GetWeaponSet(), enemy, *player);
			}
		}
	}

#pragma endregion

#pragma region EnemyUpdate
	AllEnemyUpdate(vecEnemy, *player, vecCollect);
#pragma endregion

#pragma region GridSystem
	player->CheckPlayerGridCollision(grids2D, MAP_ROW_BOSS_SIZE, MAP_COLUMN_BOSS_SIZE);
	//CheckEnemyGridCollision(grids2D, vecEnemy);

	for (s16 rows = 0; rows < MAP_ROW_BOSS_SIZE; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_BOSS_SIZE; cols++)
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

#pragma endregion

#pragma region PlatformSystem
	//Testing moving platform logic
	UpdatePlatforms(*player, vecEnemy, platformVectors); //Numbers based on how many moving platforms

#pragma endregion

#pragma region ParticleSystem

	ParticleUpdate();

#pragma endregion

#pragma region CameraUpdate

	if (isShaking)
	{
		if (cam->IsCameraShakeFinish())
		{
			isShaking = false;
			isLookingToBoss = true;
			cam->LookAhead(vecEnemy[0].startingPosition);
		}
	}

	if (isLookingToBoss)
	{
		if (cam->IsCameraLookAheadFinish())
		{
			isLookingToBoss = false;
		}
	}
	cam->UpdatePos(player, grids2D[0][0].collisionBox.minimum.x,
	               grids2D[0][MAP_COLUMN_BOSS_SIZE - 1].collisionBox.maximum.x,
	               grids2D[MAP_ROW_BOSS_SIZE - 1][0].collisionBox.minimum.y, grids2D[0][0].collisionBox.maximum.y);

	

#pragma endregion

	UpdateTraps();
}

void Level1_BOSS_Draw()
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

#pragma region Grid_Render

	DrawTraps(pWhiteSquareMesh);
	RenderGrids(grids2D, MAP_ROW_BOSS_SIZE, MAP_COLUMN_BOSS_SIZE, *pWhiteSquareMesh);


	for (s16 i = 0; i < platformVectors.size(); i++)
	{
		AEGfxSetTransform(platformVectors[i].transformation.m);
		AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);
	}

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

#pragma region Player_Render

	player->RenderPlayer();

	//drawing enemy
	AllEnemyDraw(vecEnemy, pWhiteSquareMesh, vecCollect, pMeshRed);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	if (player->GetIsPlayerAttacking())
	{
		AEGfxSetTransform(ObjectTransformationMatrixSet(player->GetWeaponSet().position.x,
		                                                player->GetWeaponSet().position.y, 0.f,
		                                                player->GetWeaponSet().scale.x,
		                                                player->GetWeaponSet().scale.y).m);
		AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);
		player->GetIsPlayerAttacking() = false;
	}

#pragma endregion

#pragma region Game_UI_Render

	/*f32 x, y;
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
	AEGfxPrint(fontID, pTextHP, -width / 2 - 0.9f, -width / 2 + 0.97f, 0.5f, 1, 1, 1, 1);*/



	player->RenderPlayerStatUI();

#pragma endregion

#pragma region Inventory_UI_Render
	Inventory::DrawInventory(pWhiteSquareMesh);
#pragma endregion

	menu->Render();

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

#pragma region Particle_Render

	ParticlesDraw(*pWhiteSquareMesh);

#pragma endregion


#pragma region Center_Line_Render

	//AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	//AEGfxSetTransform(ObjectTransformationMatrixSet(cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y, 0.f,
	//                                                (f32)AEGfxGetWindowWidth(), 1.f).m);
	//AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	//AEGfxSetTransform(ObjectTransformationMatrixSet(cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y,
	//                                                0.5f * PI, (f32)AEGfxGetWindowWidth(), 1.f).m);
	//AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

#pragma endregion
	missionSystem.PrintMissionText();
	MapTransitionDraw();
}

void Level1_BOSS_Free()
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

void Level1_BOSS_Unload()
{
	Inventory::SaveInventory();
	Inventory::FreeInventory();

	AEGfxTextureUnload(HealthBorder);
	AEGfxTextureUnload(bulletTex);
	AEGfxTextureUnload(enemyBoss1DropTex);

	UnloadTrapsTexture();

	AEGfxMeshFree(pMeshGrey);
	AEGfxMeshFree(pMeshYellow);
	AEGfxMeshFree(pMeshRed);
	AEGfxMeshFree(pLineMesh);
	AEGfxMeshFree(pMeshRedBar);
	AEGfxMeshFree(pWhiteSquareMesh);
	AEGfxMeshFree(pGreenSquareMesh);

	for (int i = 0; i < MAP_ROW_BOSS_SIZE; ++i)
	{
		delete[] grids2D[i];
	}

	delete[] grids2D;

	delete cam;
	delete menu;
}

namespace
{
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
						tmpEnemy.collisionNormal = AABBNormalize(tmpEnemy.boxHeadFeet,
						                                         gridMap[enemyIndexY][enemyIndexX].collisionBox);

						ResolveVerticalCollision(tmpEnemy.boxHeadFeet, gridMap[enemyIndexY][enemyIndexX].collisionBox,
						                         &tmpEnemy.collisionNormal, &tmpEnemy.obj.pos,
						                         &tmpEnemy.velocity);
					}
					//Check horizontal box (Left arm -> Right arm)
					if (AABBvsAABB(tmpEnemy.boxArms, gridMap[enemyIndexY][enemyIndexX].collisionBox))
					{
						tmpEnemy.isCollision = true;
						tmpEnemy.collisionNormal = AABBNormalize(tmpEnemy.boxArms,
						                                         gridMap[enemyIndexY][enemyIndexX].collisionBox);

						ResolveHorizontalCollision(tmpEnemy.boxArms, gridMap[enemyIndexY][enemyIndexX].collisionBox,
						                           &tmpEnemy.collisionNormal, &tmpEnemy.obj.pos,
						                           &tmpEnemy.velocity);
						tmpEnemy.loopIdle = false;
					}
				}
			}
		}
	}
}
