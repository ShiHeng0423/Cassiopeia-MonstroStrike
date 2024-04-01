#include "LevelHeaders.h"
#include "MapTransition.h"
#include "GameStateHeaders.h"
#include "MapTraps.h"

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

void Level1_A_Load()
{
	//Memory allocation
	grids2D = new Grids2D*[MAP_ROW_SIZE];
	for (int i = 0; i < MAP_ROW_SIZE; ++i)
	{
		grids2D[i] = new Grids2D[MAP_COLUMN_SIZE];
	}

	//loading texture only, push back into the vector
	Enemy_Load(ENEMY_JUMPER, vecEnemy);
	Enemy_Load(ENEMY_CHARGER, vecEnemy);

	bulletTex = AEGfxTextureLoad("Assets/RedCircle.png");
	enemyJumperDropTex = AEGfxTextureLoad("Assets/ENEMY_JUMPER_DROP.png");
	enemyChargerDropTex = AEGfxTextureLoad("Assets/ENEMY_CHARGER_DROP.png");
	enemyFlyDropTex = AEGfxTextureLoad("Assets/ENEMY_FLY_DROP.png");
	enemyBoss1DropTex = AEGfxTextureLoad("Assets/ENEMY_BOSS1_DROP.png");

	player = gameManager->GetPlayer();
	playerReference = player;
	background = AEGfxTextureLoad("Assets/Background2.jpg");
	auto fileName = "Assets/GameMaps/GameMap_Level1_A.csv"; //Change name as per level
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
	PreLoadTrapsTexture();

	menu = new PauseMenu_Manager();
}

void Level1_A_Initialize()
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
			
			StoreTrapDetails(grids2D[rows][cols]); //Add any traps if any

			//Previous zone is lobby
			if (grids2D[rows][cols].typeOfGrid == PLAYER_POS_GRID_1 && previous == GAME_LOBBY)
			{
				player->GetPlayerCurrentPosition() = {grids2D[rows][cols].position}; //Set position based on grid
			}
			//Previous zone is level 1 part B
			else if (grids2D[rows][cols].typeOfGrid == PLAYER_POS_GRID_2 && previous == AREA1_B)
			{
				//Set initial player position at pos grid 2
				player->GetPlayerCurrentPosition() = {grids2D[rows][cols].position}; //Set position based on grid
			}
		}
	}
	player->GetPlayerScale() = {grids2D[0][0].size.x * 1.25f, grids2D[0][0].size.y * 1.25f};
#pragma endregion

	cam = new Camera(player->GetPlayerCurrentPosition());

#pragma region Inventory_UI

	Inventory::InitInventory();

#pragma endregion

#pragma region Enemy
	Enemy_Init({70.f, 70.f}, {180.f, -50.f}, ENEMY_IDLE, vecEnemy[0]);
	Enemy_Init({70.f, 70.f}, {1090.f, -200.f}, ENEMY_IDLE, vecEnemy[1]);
#pragma endregion Enemy

	menu->Init(cam);
	ParticleInitialize();
	MapTransitionInit();
}

void Level1_A_Update()
{
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
	player->CheckPlayerGridCollision(grids2D, MAP_ROW_SIZE, MAP_COLUMN_SIZE);
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

	UpdateTraps();
}

void Level1_A_Draw()
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
	RenderGrids(grids2D, MAP_ROW_SIZE, MAP_COLUMN_SIZE, *pWhiteSquareMesh);

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


#pragma region INVENTORY_UI_RENDER
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

void Level1_A_Free()
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

void Level1_A_Unload()
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

	for (int i = 0; i < MAP_ROW_SIZE; ++i)
	{
		delete[] grids2D[i];
	}

	delete[] grids2D;

	delete cam;
	delete menu;
}

//Function definition for private functions
namespace {

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
				enemyIndexX += 1;
			}
			enemyIndexY += 1;
		}
	}
}
