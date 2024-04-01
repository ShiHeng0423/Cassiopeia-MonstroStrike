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


	void CheckEnemyGridCollision(Grids2D** gridMap, std::vector<Enemy>& enemy);
}

void Level1_D_Load()
{
	grids2D = new Grids2D*[MAP_ROW_SIZE_2];
	for (int i = 0; i < MAP_ROW_SIZE_2; ++i)
	{
		grids2D[i] = new Grids2D[MAP_COLUMN_SIZE_2];
	}

	Enemy_Load(ENEMY_FLY, vecEnemy);
	Enemy_Load(ENEMY_JUMPER, vecEnemy);
	Enemy_Load(ENEMY_JUMPER, vecEnemy);
	Enemy_Load(ENEMY_CHARGER, vecEnemy);
	Enemy_Load(ENEMY_FLY, vecEnemy);
	Enemy_Load(ENEMY_JUMPER, vecEnemy);
	Enemy_Load(ENEMY_JUMPER, vecEnemy);

	bulletTex = AEGfxTextureLoad("Assets/RedCircle.png");
	enemyJumperDropTex = AEGfxTextureLoad("Assets/ENEMY_JUMPER_DROP.png");
	enemyChargerDropTex = AEGfxTextureLoad("Assets/ENEMY_CHARGER_DROP.png");
	enemyFlyDropTex = AEGfxTextureLoad("Assets/ENEMY_FLY_DROP.png");
	enemyBoss1DropTex = AEGfxTextureLoad("Assets/ENEMY_BOSS1_DROP.png");

	player = gameManager->GetPlayer();
	playerReference = player;
	background = AEGfxTextureLoad("Assets/Background2.jpg");
	auto fileName = "Assets/GameMaps/GameMap_Level1_D.csv"; //Change name as per level
	//Load map
	if (MapLoader(fileName, gameMap, MAP_ROW_SIZE_2, MAP_COLUMN_SIZE_2))
	{
		PrintMap(gameMap, MAP_ROW_SIZE_2, MAP_COLUMN_SIZE_2); //Just for checking if the map data is stored properly
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

void Level1_D_Initialize()
{
#pragma region Grid_Loading
	//Initializing grid data
	SetGridTypes(grids2D, gameMap, MAP_ROW_SIZE_2, MAP_COLUMN_SIZE_2);

	//For Initializing the grids and positions
	for (s16 rows = 0; rows < MAP_ROW_SIZE_2; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_SIZE_2; cols++)
		{
			grids2D[rows][cols].rowIndex = rows;
			grids2D[rows][cols].colIndex = cols;

			InitializeGrid(grids2D[rows][cols]);
			StoreTrapDetails(grids2D[rows][cols]); //Add any traps if any

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

			//Previous zone is area a
			if (grids2D[rows][cols].typeOfGrid == PLAYER_POS_GRID_1 && previous == AREA1_C)
			{
				player->GetPlayerCurrentPosition() = {grids2D[rows][cols].position}; //Set position based on grid
			}
			else if (grids2D[rows][cols].typeOfGrid == PLAYER_POS_GRID_2 && previous == AREA1_E)
			{
				//Set initial player position at pos grid 2
				player->GetPlayerCurrentPosition() = {grids2D[rows][cols].position}; //Set position based on grid
			}
			//Previous zone is treasure zone
			//else if (grids2D[rows][cols].typeOfGrid == PLAYER_POS_GRID_3 && previous == AREA1_C)
			//{
			//	//Set initial player position at pos grid 2
			//	player->GetPlayerCurrentPosition() = { grids2D[rows][cols].position }; //Set position based on grid
			//}
			//Previous zone is area b
		}
	}
	player->GetPlayerScale() = {grids2D[0][0].size.x * 1.25f, grids2D[0][0].size.y * 1.25f};

#pragma endregion

	cam = new Camera(player->GetPlayerCurrentPosition());

#pragma region Inventory_UI

	Inventory::InitInventory();

#pragma endregion

	Enemy_Init({70.f, 70.f}, {-560.f, 0.f}, ENEMY_IDLE, vecEnemy[0]);
	Enemy_Init({70.f, 70.f}, {-664.f, 350.f}, ENEMY_IDLE, vecEnemy[1]);
	Enemy_Init({70.f, 70.f}, {-532.f, -700.f}, ENEMY_IDLE, vecEnemy[2]);
	Enemy_Init({70.f, 70.f}, {161.f, -1100.f}, ENEMY_IDLE, vecEnemy[3]);
	Enemy_Init({70.f, 70.f}, {-338.f, -1200.f}, ENEMY_IDLE, vecEnemy[4]);
	Enemy_Init({70.f, 70.f}, {-170.f, -1700.f}, ENEMY_IDLE, vecEnemy[5]);
	Enemy_Init({70.f, 70.f}, {-626.f, -1700.f}, ENEMY_IDLE, vecEnemy[6]);

	menu->Init(cam);
	ParticleInitialize();
	MapTransitionInit();
}

void Level1_D_Update()
{
	//std::cout << player->obj.pos.x << " " << player->obj.pos.y << "\n";
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
	if (AEInputCheckTriggered(AEVK_TAB))
	{
		Inventory::inventoryOpen = !Inventory::inventoryOpen;
		Inventory::itemHover = false;
	}

	if (AEInputCheckTriggered(AEVK_0))
	{
		//next = GameStates::Quit;
		AEVec2 test{100.f, 100.f};
		cam->LookAhead(test);
	}
	if (AEInputCheckCurr(AEVK_1))
	{
		cam->CameraShake();
	}

	//This is set here temporary so that thing actually work, need to move
	if (player->GetIsPlayerAttacking())
	{
		for (Enemy& enemy : vecEnemy)
		{
			if (enemy.isAlive)
			{
				CheckWeaponCollision(&player->GetWeaponSet(), enemy, *player);
			}
		}
	}

#pragma endregion

#pragma region EnemyUpdate
	AllEnemyUpdate(vecEnemy, *player, vecCollect);
#pragma endregion

#pragma region GridSystem
	player->CheckPlayerGridCollision(grids2D, MAP_ROW_SIZE_2, MAP_COLUMN_SIZE_2);
	//CheckEnemyGridCollision(grids2D, vecEnemy);

	for (s16 rows = 0; rows < MAP_ROW_SIZE_2; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_SIZE_2; cols++)
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
	               grids2D[0][MAP_COLUMN_SIZE_2 - 1].collisionBox.maximum.x,
	               grids2D[MAP_ROW_SIZE_2 - 1][0].collisionBox.minimum.y, grids2D[0][0].collisionBox.maximum.y);

#pragma endregion

	UpdateTraps();

}

void Level1_D_Draw()
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

#pragma region Background_Render

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxTextureSet(background, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, 0.f, 0.f, 4200, 1080.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

#pragma endregion

#pragma region Grid_Render
	DrawTraps(pWhiteSquareMesh);

	RenderGrids(grids2D, MAP_ROW_SIZE_2, MAP_COLUMN_SIZE_2, *pWhiteSquareMesh);

	for (s16 i = 0; i < platformVectors.size(); i++)
	{
		AEGfxSetTransform(platformVectors[i].transformation.m);
		AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);
	}

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

#pragma region Player_Render

	player->RenderPlayer();

	//drawing enemy
	AllEnemyDraw(vecEnemy, pWhiteSquareMesh, vecCollect);

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

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	//AEGfxSetTransform(ObjectTransformationMatrixSet(cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y, 0.f,
	//	(f32)AEGfxGetWindowWidth(), 1.f).m);
	//AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	//AEGfxSetTransform(ObjectTransformationMatrixSet(cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y,
	//	0.5f * PI, (f32)AEGfxGetWindowWidth(), 1.f).m);
	//AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

#pragma endregion
	missionSystem.PrintMissionText();
	MapTransitionDraw();
}

void Level1_D_Free()
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

void Level1_D_Unload()
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

	UnloadTrapsTexture();


	AEGfxMeshFree(pMeshGrey);
	AEGfxMeshFree(pMeshYellow);
	AEGfxMeshFree(pMeshRed);
	AEGfxMeshFree(pLineMesh);
	AEGfxMeshFree(pMeshRedBar);
	AEGfxMeshFree(pWhiteSquareMesh);
	AEGfxMeshFree(pGreenSquareMesh);
	for (int i = 0; i < MAP_ROW_SIZE_2; ++i)
	{
		delete[] grids2D[i];
	}

	delete[] grids2D;

	delete cam;
	delete menu;
}

namespace
{
	//void CheckPlayerGridCollision(Grids2D** gridMap, Player* player)
	//{
	//	int playerIndexY = (int)((AEGfxGetWindowHeight() * 0.5f - player->GetPlayerCurrentPosition().y) / (gridMap[0][0].size.x));

	//	for (int i = 0; i <= (int)(player->GetPlayerScale().x * 2 / gridMap[0][0].size.x); i++)
	//	{
	//		int playerIndexX = (int)((player->GetPlayerCurrentPosition().x + AEGfxGetWindowWidth() * 0.5f) / (gridMap[0][0].size.x));
	//		for (int j = 0; j <= (int)(player->GetPlayerScale().x * 2 / gridMap[0][0].size.x); j++)
	//		{
	//			switch (gridMap[playerIndexY][playerIndexX].typeOfGrid)
	//			{
	//			case NORMAL_GROUND:
	//				//Collision check
	//				//Resolve + Vertical Collision only for entity x (wall or ground)
	//				//Check vertical box (Head + Feet) 
	//				if (AABBvsAABB(player->boxHeadFeet, gridMap[playerIndexY][playerIndexX].collisionBox))
	//				{
	//					player->collisionNormal = AABBNormalize(player->boxHeadFeet,
	//						gridMap[playerIndexY][playerIndexX].collisionBox);
	//					ResolveVerticalCollision(player->boxHeadFeet, gridMap[playerIndexY][playerIndexX].collisionBox,
	//						&player->collisionNormal, &player->GetPlayerCurrentPosition(),
	//						&player->velocity, &player->onFloor, &player->gravityForce,
	//						&player->isFalling);
	//				}

	//				//Check horizontal box (Left arm -> Right arm)
	//				if (AABBvsAABB(player->boxArms, gridMap[playerIndexY][playerIndexX].collisionBox))
	//				{
	//					player->collisionNormal = AABBNormalize(player->boxArms,
	//						gridMap[playerIndexY][playerIndexX].collisionBox);
	//					ResolveHorizontalCollision(player->boxArms, gridMap[playerIndexY][playerIndexX].collisionBox,
	//						&player->collisionNormal, &player->GetPlayerCurrentPosition(),
	//						&player->velocity);
	//				}
	//				break;
	//			case LAVA_GRID:
	//				if (AABBvsAABB(player->collisionBox, gridMap[playerIndexY][playerIndexX].collisionBox))
	//				{
	//					OnPlayerDeath();
	//				}
	//				break;
	//			case MAP_TRANSITION_GRID_1:
	//				if (AABBvsAABB(player->collisionBox, gridMap[playerIndexY][playerIndexX].collisionBox))
	//				{
	//					//std::cout << "Collided\n";MainMenu_Song
	//					if (!transitionalImageOBJ.active)
	//					{
	//						transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1_C);
	//					}
	//				}
	//				break;
	//			case MAP_TRANSITION_GRID_2:
	//				if (AABBvsAABB(player->collisionBox, gridMap[playerIndexY][playerIndexX].collisionBox))
	//				{
	//					//std::cout << "Collided\n";MainMenu_Song
	//					if (!transitionalImageOBJ.active)
	//					{
	//						transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1_E);
	//					}
	//				}
	//				//Treasure room
	//			//case MAP_TRANSITION_GRID_3:
	//			//	if (AABBvsAABB(player->collisionBox, gridMap[playerIndexY][playerIndexX].collisionBox))
	//			//	{
	//			//		//std::cout << "Collided\n";MainMenu_Song
	//			//		if (!transitionalImageOBJ.active)
	//			//		{
	//			//			transitionalImageOBJ.PlayMapTransition(TRANSITION_RIGHT, AREA1_E);
	//			//		}
	//			//	}
	//				break;
	//			case EMPTY:
	//				break;
	//			}
	//			playerIndexX += 1;
	//		}
	//		playerIndexY += 1;
	//	}
	//}

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
						                         &tmpEnemy.velocity, &tmpEnemy.onFloor, &tmpEnemy.gravityForce,
						                         &tmpEnemy.isFalling);
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
