#include "LevelHeaders.h"

namespace
{
	AEGfxVertexList* pLineMesh;
	AEGfxVertexList* pMeshYellow;
	AEGfxVertexList* pMeshRed;
	AEGfxVertexList* pMeshGrey;
	AEGfxVertexList* pWhiteSquareMesh;

	Grids2D grids2D[MAP_ROW_SIZE][MAP_COLUMN_SIZE]; //Initializing map
	std::vector<std::vector<MapCell>> gameMap; //Map for this level

	std::vector<struct Platforms> platformVectors;

	std::vector<Enemy> vecEnemy; //enemy container

	Player* player;

	AEGfxTexture* background;

	Camera* cam;
	PauseMenu_Manager* menu;

#pragma region UserInterface
	//User Health
	AEGfxVertexList* pMeshRedBar;
	AEGfxTexture* HealthBorder;
	
#pragma endregion UserInterface

	bool inventory_open;
	int snap_back = -1;

	int hp = 100;
	int gear_equipped = 0;

	float burningEffectDuration = 1.5f;
	f64 timer = 0.f;

	ButtonGearUI inventoryBackground;
	ButtonGearUI inventoryButton[25];

	ButtonGearUI equipmentBackground;
	ButtonGearUI equipmentDisplay[5];

	AEGfxTexture* blank;
	AEGfxTexture* Gear1;
	AEGfxTexture* Gear2;
	AEGfxTexture* weapon3;
	AEGfxTexture* Gear4;
	AEGfxTexture* Gear5;

	//Button inventoryBackground;

	AEVec2 playerBoundaryMin;
	AEVec2 playerBoundaryMax;
}

AEGfxTexture* bulletTex = nullptr;

int playerGridMinX;
int playerGridMinY;
int playerGridMaxX;
int playerGridMaxY;

void Level1_Load()
{

	//loading texture only, push back into the vector
	Enemy_Load(ENEMY_JUMPER, vecEnemy);
	Enemy_Load(ENEMY_BOSS1, vecEnemy);
	Enemy_Load(ENEMY_CHARGER, vecEnemy);
	Enemy_Load(ENEMY_FLY, vecEnemy);

	bulletTex = AEGfxTextureLoad("Assets/RedCircle.png");


	player = PlayerInitialize("Assets/Border.png", { AEGfxGetWindowWidth() * 0.05f, AEGfxGetWindowWidth() * 0.05f }, { 0,-100 }, { 40.f,0.f }, true);
	background = AEGfxTextureLoad("Assets/Background2.jpg");
	const char* fileName = "Assets/GameMap.csv"; //Change name as per level
	//Load map
	if (MapLoader(fileName, gameMap, MAP_ROW_SIZE, MAP_COLUMN_SIZE))
	{
		PrintMap(gameMap, MAP_ROW_SIZE, MAP_COLUMN_SIZE); //Just for checking if the map data is stored properly
	}

	inventoryBackground.img.pTex = AEGfxTextureLoad("Assets/panel_brown.png");

	Gear1 = AEGfxTextureLoad("Assets/item_.png");
	Gear2 = AEGfxTextureLoad("Assets/item_helmet.png");
	weapon3 = AEGfxTextureLoad("Assets/item_sword.png");
	Gear4 = AEGfxTextureLoad("Assets/tile_archColumns.png");
	Gear5 = AEGfxTextureLoad("Assets/tile_archHalf.png");
	blank = AEGfxTextureLoad("Assets/panelInset_beige.png");

	equipmentBackground.img.pTex = AEGfxTextureLoad("Assets/panel_brown.png");
	

	HealthBorder = AEGfxTextureLoad("Assets/UI_Sprite/Border/panel-border-015.png");

#pragma region Mesh Creations
	pMeshGrey = GenerateSquareMesh(0xFFa9a9a9);
	pMeshRed = GenerateSquareMesh(0xFFFF0000);
	pMeshRedBar = GenerateSquareMesh(0xFFFF0000);
	pMeshYellow = GenerateSquareMesh(0xFFFFFF00);
	pWhiteSquareMesh = GenerateSquareMesh(0xFFFFFFFF);

	//Drawing line
	pLineMesh = GenerateLineMesh(0xFF000000);
#pragma endregion
	ParticleLoad();
	
	menu = new PauseMenu_Manager();
}

void Level1_Initialize()
{
#pragma region Grid_Loading
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
				grids2D[rows][cols].typeOfGrid = NONE;
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

			InitializeGrid(grids2D[rows][cols]);
		}
	}

	//Need to place the objects one by one 
	CreatePlatform(1200.f, -300.f, 140.f, 30.f, 3.f, HORIZONTAL_MOVING_PLATFORM, platformVectors);
	CreatePlatform(1200.f, 0.f, 140.f, 30.f, 2.f, VERTICAL_MOVING_PLATFORM, platformVectors);
	CreatePlatform(1400.f, 0.f, 140.f, 30.f, 2.f, DIAGONAL_PLATFORM, platformVectors);
#pragma endregion

	cam = new Camera(player->obj.pos);

#pragma region Inventory_UI

	AEVec2Set(&inventoryBackground.img.scale, 500.f, 500.f);

	inventory_open = false;
	int index = 0;
	for (ButtonGearUI& button : inventoryButton)
	{
		button.img.pTex = blank;
		AEVec2Set(&button.img.scale, 60.f, 60.f);
		AEVec2Set(&button.pos, (index % 5) * 90.f - 180.f, -(index / 5.f) * 90.f + 180.f);
		button.isWeapon = false;
		index++;
	}
	inventoryButton[0].img.pTex = Gear1;
	inventoryButton[1].img.pTex = Gear2;
	inventoryButton[2].img.pTex = weapon3;
	inventoryButton[2].isWeapon = true;
	inventoryButton[3].img.pTex = Gear4;
	inventoryButton[4].img.pTex = Gear5;


	AEVec2Set(&equipmentBackground.img.scale, 250.f, 500.f);
	AEVec2Set(&equipmentBackground.pos, -375.f, 0.f);

	index = 0;
	for (ButtonGearUI& button : equipmentDisplay)
	{
		button.img.pTex = blank;
		AEVec2Set(&button.img.scale, 60.f, 60.f);
		AEVec2Set(&button.pos, -375.f, -index * 90.f + 180.f);
		index++;
	}

#pragma endregion

	menu->Init(cam);

#pragma region Enemy
	Enemy_Init({70.f,70.f}, {1200.f,-320.f}, ENEMY_IDLE, vecEnemy[0]);
	Enemy_Init({70.f,70.f}, {1500.f, 250.f}, ENEMY_IDLE, vecEnemy[1]);
	Enemy_Init({70.f,70.f}, { -500.f,player->obj.pos.y }, ENEMY_IDLE, vecEnemy[2]);
	Enemy_Init({70.f,70.f }, { 800.f,150.f }, ENEMY_IDLE, vecEnemy[3]);
#pragma endregion Enemy
	ParticleInitialize();
}

void Level1_Update()
{
	//std::cout << AEFrameRateControllerGetFrameRate() << "\n";

#pragma region PauseMenuTrigger

	menu->Update(cam);

#pragma endregion
	if (currScene == CurrentScene::PauseScene || currScene == CurrentScene::ControlScene || currScene == CurrentScene::QuitScene)
		return;
#pragma region PlayerUpdate
	if (currScene == MainScene && !inventory_open)
		PlayerUpdate(*player);
	if (AEInputCheckTriggered(AEVK_I))
	{
		inventory_open = !inventory_open;
	}

	//This is set here temporary so that thing actually work, need to move
	if (player->isAttacking)
	{
		for (Enemy& enemy : vecEnemy) {
			if (enemy.isAlive) {
				CheckWeaponCollision(&player->equippedWeapon, enemy, *player);
			}
		}

	}

#pragma endregion

#pragma region EnemySystem

	for (Enemy& enemy : vecEnemy) {
		if (enemy.isAlive) {
			Enemy_Update_Choose(enemy, *player);
		}
	}

#pragma endregion

	

#pragma region GridSystem
	//For printing the grids every frame
	for (s16 rows = 0; rows < MAP_ROW_LOBBY_SIZE; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_LOBBY_SIZE; cols++)
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
						&player->velocity, &player->onFloor, &player->gravityForce, &player->isFalling);
				}

				//Check horizontal box (Left arm -> Right arm)
				if (AABBvsAABB(player->boxArms, grids2D[rows][cols].collisionBox))
				{
					player->collisionNormal = AABBNormalize(player->boxArms, grids2D[rows][cols].collisionBox);
					ResolveHorizontalCollision(player->boxArms, grids2D[rows][cols].collisionBox, &player->collisionNormal, &player->obj.pos,
						&player->velocity);
				}
				break;
			case MAP_TRANSITION_GRID:
				//if (AABBvsAABB(player->collisionBox, grids2D[rows][cols].collisionBox))
				//{
				//	if (!transitionalImageOBJ.active)
				//	{
				//		transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, Area1);
				//	}
				//}
				break;
			case EMPTY:
				break;
			}
		}
	}
#pragma endregion

#pragma region InventorySystem

	if (inventory_open)
	{
		//update item position
		Inventory::UpdateInventory(Inventory::Player_Inventory, inventoryButton);

		//Hover collision with button && hold left mouse button

		int snap_back = 0;
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			s32 testx = 0;
			s32 testy = 0;

			int index = 0;

			AEInputGetCursorPosition(&testx, &testy);
			AEVec2 mousePos;
			mousePos.x = testx - AEGfxGetWindowWidth() * 0.5f;
			mousePos.y = AEGfxGetWindowHeight() * 0.5f - testy;

			for (ButtonGearUI& button : inventoryButton)
			{
				if (AETestPointToRect(&mousePos, &button.pos, button.img.scale.x, button.img.scale.y))
				{
					if (button.img.pTex != blank)
					{
						std::cout << "left triggered\n";
						//snap origin of img to mouse pos
						snap_back = index;
						break;
					}
					//button.Ptr();
				}
				index++;
			}
		}

		if (snap_back >= 0)
		{
			s32 testx = 0;
			s32 testy = 0;

			int index = 0;

			AEInputGetCursorPosition(&testx, &testy);
			AEVec2 mousePos;
			mousePos.x = testx - AEGfxGetWindowWidth() * 0.5f;
			mousePos.y = AEGfxGetWindowHeight() * 0.5f - testy;

			inventoryButton[snap_back].pos = mousePos;
		}

		if (AEInputCheckReleased(AEVK_LBUTTON))
		{
			int index = 0;
			if (snap_back >= 0)
			{
				for (ButtonGearUI& button : inventoryButton)
				{
					if (AETestRectToRect(&inventoryButton[snap_back].pos, inventoryButton[snap_back].img.scale.x,
						inventoryButton[snap_back].img.scale.y, &button.pos, button.img.scale.x,
						button.img.scale.y))
					{
						//Different items overlapping
						if (index != snap_back)
						{
							AEVec2Set(&inventoryButton[snap_back].pos, (snap_back % 5) * 90.f - 180.f,
								-(snap_back / 5.f) * 90.f + 180.f);

							std::cout << "swap\n";
							ButtonGearUI tmp = button;
							button = inventoryButton[snap_back];
							inventoryButton[snap_back] = tmp;

							AEVec2Set(&inventoryButton[snap_back].pos, (snap_back % 5) * 90.f - 180.f,
								-(snap_back / 5) * 90.f + 180.f);

							AEVec2Set(&button.pos, (index % 5) * 90.f - 180.f,
								-(index / 5.f) * 90.f + 180.f);

							snap_back = -1;
							break;
						}
					}
					index++;
				}

				if (snap_back >= 0)
				{
					AEVec2Set(&inventoryButton[snap_back].pos, (snap_back % 5) * 90.f - 180.f,
						-(snap_back / 5.f) * 90.f + 180.f);
					snap_back = -1;
				}
			}
		}


		if (AEInputCheckTriggered(AEVK_RBUTTON))
		{
			int index = 0;
			s32 testx = 0;
			s32 testy = 0;


			AEInputGetCursorPosition(&testx, &testy);
			AEVec2 mousePos;
			mousePos.x = testx - AEGfxGetWindowWidth() * 0.5f;
			mousePos.y = AEGfxGetWindowHeight() * 0.5f - testy;

			for (ButtonGearUI& button : inventoryButton)
			{
				if (AETestPointToRect(&mousePos, &button.pos, button.img.scale.x, button.img.scale.y))
				{
					if (button.img.pTex != blank)
					{
						Equip(index, button);
						button.img.pTex = blank;
						break;
					}
					//button.Ptr();
				}
				index++;
			}
		}
	}

	if (player->burningEffect)
	{
		if (timer >= burningEffectDuration)
		{
			AEVec2 hpLerp{ 0,0 };
			hpLerp.x = (f32)hp;
			hpLerp.y = 0.f;
			AEVec2 end{ 0,0 };
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

	if (AEInputCheckTriggered(AEVK_U))
	{
		ParticleEmit(5, player->obj.pos.x, player->obj.pos.y, 5.f, 5.f, PI / 3, TEST);
	}
	if (AEInputCheckTriggered(AEVK_H))
	{
		ParticleEmit(5, player->obj.pos.x, player->obj.pos.y, 20.f, 20.f, PI / 4, TEST);
	}
	ParticleUpdate();

#pragma endregion

#pragma region CameraUpdate

	cam->UpdatePos(*player, grids2D[0][0].collisionBox.minimum.x, grids2D[0][97].collisionBox.maximum.x, grids2D[49][0].collisionBox.minimum.y, grids2D[0][0].collisionBox.maximum.y);

#pragma endregion

}

void Level1_Draw()
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

#pragma region Background_Render

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxTextureSet(background, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, 0.f, 0.f, 4200, 1080.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

#pragma endregion

	//For Grid Drawing
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

#pragma region Grid_Render

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
				AEGfxSetTransform(grids2D[rows][cols].transformation.m);
				AEGfxMeshDraw(pMeshGrey, AE_GFX_MDM_TRIANGLES);
				break;
			}
		}
	}

	for (int i = 0; i < platformVectors.size(); i++)
	{
		AEGfxSetTransform(platformVectors[i].transformation.m);
		AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);

	}

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

#pragma region Player_Render

	AEGfxTextureSet(player->obj.img.pTex, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(player->obj.pos.x, player->obj.pos.y, 0.f, player->obj.img.scale.x, player->obj.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	for (Enemy& enemy : vecEnemy) {
		if (enemy.isAlive) {


			if (enemy.isShooting) {
				AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

				AEGfxTextureSet(enemy.angrytex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(enemy.obj.pos.x, enemy.obj.pos.y, 0.f, enemy.obj.img.scale.x, enemy.obj.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

				DrawBullets(enemy, pWhiteSquareMesh); //drawing bullets
			}
			else {
				AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

				AEGfxTextureSet(enemy.obj.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(enemy.obj.pos.x, enemy.obj.pos.y, 0.f, enemy.obj.img.scale.x, enemy.obj.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

				DrawBullets(enemy, pWhiteSquareMesh); //drawing bullets

			}


			if (enemy.enemyType == ENEMY_BOSS1 && enemy.wing1.isAlive) {
				if (enemy.isShooting) {
					AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 0.0f);
				}
				else {
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
				}


				AEGfxTextureSet(enemy.wing1.obj.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(enemy.wing1.obj.pos.x, enemy.wing1.obj.pos.y, 0.f, enemy.wing1.obj.img.scale.x, enemy.wing1.obj.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}
			if (enemy.enemyType == ENEMY_BOSS1 && enemy.wing2.isAlive) {
				if (enemy.isShooting) {
					AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 0.0f);
				}
				else {
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
				}

				AEGfxTextureSet(enemy.wing2.obj.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(enemy.wing2.obj.pos.x, enemy.wing2.obj.pos.y, 0.f, enemy.wing2.obj.img.scale.x, enemy.wing2.obj.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}
		}
	}

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	if (player->isAttacking)
	{
		AEGfxSetTransform(ObjectTransformationMatrixSet(player->equippedWeapon.position.x,
			player->equippedWeapon.position.y, 0.f,
			player->equippedWeapon.Scale.x,
			player->equippedWeapon.Scale.y).m);
		AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);
	}

#pragma endregion

#pragma region Game_UI_Render

	f32 x, y;
	AEGfxGetCamPosition(&x, &y);

	AEGfxSetTransform(ObjectTransformationMatrixSet(-800.f + hp + x, 450.f + y, 0, hp * 2.f, 80.f).m);
	AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(HealthBorder, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(-800.f + hp +x, 450.f + y, 0, hp * 2.f, 80.f).m);
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
	if (inventory_open)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

		AEGfxTextureSet(inventoryBackground.img.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y, 0.f,
			inventoryBackground.img.scale.x,
			inventoryBackground.img.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		for (ButtonGearUI button : inventoryButton)
		{
			AEGfxTextureSet(button.img.pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + cam->GetCameraWorldPoint().x,
				button.pos.y + cam->GetCameraWorldPoint().y, 0.f,
				button.img.scale.x, button.img.scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}

		AEGfxTextureSet(equipmentBackground.img.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(equipmentBackground.pos.x + cam->GetCameraWorldPoint().x,
			equipmentBackground.pos.y + cam->GetCameraWorldPoint().y, 0.f,
			equipmentBackground.img.scale.x,
			equipmentBackground.img.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		for (ButtonGearUI button : equipmentDisplay)
		{
			AEGfxTextureSet(button.img.pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + cam->GetCameraWorldPoint().x,
				button.pos.y + cam->GetCameraWorldPoint().y, 0.f,
				button.img.scale.x, button.img.scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
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

	AEGfxSetTransform(ObjectTransformationMatrixSet(cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y, 0.f, (f32)AEGfxGetWindowWidth(), 1.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(ObjectTransformationMatrixSet(cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y, 0.5f * PI, (f32)AEGfxGetWindowWidth(), 1.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

#pragma endregion
}

void Level1_Free()
{

	FreeEnemy(vecEnemy);	//loops thru all eney tex and free them.
	//Free Enemy Vector
	vecEnemy.clear();
	vecEnemy.resize(0);

	//Free vectors
	gameMap.clear();
	gameMap.resize(0);
	platformVectors.clear();
	platformVectors.resize(0);

	AEGfxSetCamPosition(0.f, 0.f);
	ParticlesFree();
}

void Level1_Unload()
{
	Inventory::SaveInventory();

	AEGfxTextureUnload(background);
	AEGfxTextureUnload(HealthBorder);
	AEGfxTextureUnload(bulletTex);

	AEGfxTextureUnload(blank);
	AEGfxTextureUnload(Gear1);
	AEGfxTextureUnload(Gear2);
	AEGfxTextureUnload(weapon3);
	AEGfxTextureUnload(Gear4);
	AEGfxTextureUnload(Gear5);

	AEGfxTextureUnload(inventoryBackground.img.pTex);
	AEGfxTextureUnload(equipmentBackground.img.pTex);
	AEGfxTextureUnload(player->obj.img.pTex);

	AEGfxMeshFree(pMeshGrey);
	AEGfxMeshFree(pMeshYellow);
	AEGfxMeshFree(pMeshRed);
	AEGfxMeshFree(pLineMesh);
	AEGfxMeshFree(pMeshRedBar);
	AEGfxMeshFree(pWhiteSquareMesh);

	delete player;
	delete cam;
	delete menu;
}
