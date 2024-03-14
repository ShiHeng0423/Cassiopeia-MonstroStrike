#include "LevelHeaders.h"
#include "MapTransition.h"

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

	bool inventoryOpen;
	s16 snapBack = -1;

	s16 hp = 100;
	s16 gear_equipped = 0;

	f32 burningEffectDuration = 1.5f;
	f64 timer = 0.f;

	ButtonGearUI inventoryBackground;
	ButtonGearUI inventoryButton[25];

	ButtonGearUI equipmentBackground;

	AEGfxTexture* blank;

	AEVec2 playerBoundaryMin;
	AEVec2 playerBoundaryMax;
}


AEGfxTexture* bulletTex = nullptr;

s16 playerGridMinX;
s16 playerGridMinY;
s16 playerGridMaxX;
s16 playerGridMaxY;

void CheckPlayerGridCollision(Grids2D gridMap[][MAP_COLUMN_SIZE], Player* player);

void CheckEnemyGridCollision(Grids2D gridMap[][MAP_COLUMN_SIZE], std::vector<Enemy>& enemy);

void Level1_Load()
{
	//loading texture only, push back into the vector
	Enemy_Load(ENEMY_JUMPER, vecEnemy);
	Enemy_Load(ENEMY_JUMPER, vecEnemy);
	Enemy_Load(ENEMY_BOSS1, vecEnemy);
	Enemy_Load(ENEMY_CHARGER, vecEnemy);
	Enemy_Load(ENEMY_CHARGER, vecEnemy);
	Enemy_Load(ENEMY_FLY, vecEnemy);

	bulletTex = AEGfxTextureLoad("Assets/RedCircle.png");

	player = PlayerInitialize("Assets/Border.png", { 0.f, 0.f }, { 0.f,0.f },{ 40.f,0.f }, true);
	playerReference = player;
	background = AEGfxTextureLoad("Assets/Background2.jpg");
	const char* fileName = "Assets/GameMap.csv"; //Change name as per level
	//Load map
	if (MapLoader(fileName, gameMap, MAP_ROW_SIZE, MAP_COLUMN_SIZE))
	{
		PrintMap(gameMap, MAP_ROW_SIZE, MAP_COLUMN_SIZE); //Just for checking if the map data is stored properly
	}

	//Inventory assets
	Inventory::LoadInventory();

	inventoryBackground.img.pTex = AEGfxTextureLoad("Assets/panel_brown.png");

	equipmentBackground.img.pTex = AEGfxTextureLoad("Assets/panel_brown.png");

	Gear[0] = AEGfxTextureLoad("Assets/items/item_helmet.png");
	Gear[1] = AEGfxTextureLoad("Assets/items/chest.png");
	Gear[2] = AEGfxTextureLoad("Assets/items/tile_archHalf.png");
	Gear[3] = AEGfxTextureLoad("Assets/items/tile_archColumns.png");
	Gear[4] = AEGfxTextureLoad("Assets/items/item_sword.png");
	Gear[5] = AEGfxTextureLoad("Assets/items/item_helmet.png");
	Gear[6] = AEGfxTextureLoad("Assets/items/item_angel_sword.png");
	Gear[7] = AEGfxTextureLoad("Assets/items/item_.png");

	blank = AEGfxTextureLoad("Assets/panelInset_beige.png");

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
			case 97:
				grids2D[rows][cols].typeOfGrid = PLAYER_POS_GRID;
				break;
			case 98:
				grids2D[rows][cols].typeOfGrid = MAP_TRANSITION_GRID;
				break;
			default:
				grids2D[rows][cols].typeOfGrid = NONE;
				break;
			}
		}
	}

	//For Initializing the grids and positions
	for (s16 rows = 0; rows < MAP_ROW_SIZE; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_SIZE; cols++)
		{
			grids2D[rows][cols].rowIndex = rows;
			grids2D[rows][cols].colIndex = cols;

			InitializeGrid(grids2D[rows][cols]);

			if (grids2D[rows][cols].typeOfGrid == PLAYER_POS_GRID)
			{
				player->obj.pos = { grids2D[rows][cols].position }; //Set position based on grid
			}
		}
	}
	player->obj.img.scale = { grids2D[0][0].size };

	//Need to place the objects one by one 
	CreatePlatform(1200.f, -300.f, 140.f, 30.f, 3.f, HORIZONTAL_MOVING_PLATFORM, platformVectors);
	CreatePlatform(1200.f, 0.f, 140.f, 30.f, 2.f, VERTICAL_MOVING_PLATFORM, platformVectors);
	CreatePlatform(1400.f, 0.f, 140.f, 30.f, 2.f, DIAGONAL_PLATFORM, platformVectors);
#pragma endregion

	cam = new Camera(player->obj.pos);

#pragma region Inventory_UI

	AEVec2Set(&inventoryBackground.img.scale, 500.f, 500.f);

	inventoryOpen = false;
	s16 index = 0;
	//Display inventory


	AEVec2Set(&equipmentBackground.img.scale, 250.f, 500.f);
	AEVec2Set(&equipmentBackground.pos, -375.f, 0.f);

	index = 0;
	for (ButtonGearUI& button : Inventory::equipmentDisplay)
	{
		button.img.pTex = blank;
		AEVec2Set(&button.img.scale, 60.f, 60.f);
		AEVec2Set(&button.pos, -375.f, -index * 90.f + 180.f);
		index++;
	}

#pragma endregion

#pragma region Enemy
	Enemy_Init({70.f,70.f}, { 1800.f,-80.f }, ENEMY_IDLE, vecEnemy[0]);
	Enemy_Init({ 70.f,70.f }, { 1850.f,-80.f }, ENEMY_IDLE, vecEnemy[1]);
	Enemy_Init({70.f,70.f}, {1800.f, 250.f}, ENEMY_IDLE, vecEnemy[2]);
	Enemy_Init({70.f,70.f}, { -500.f, -80.f }, ENEMY_IDLE, vecEnemy[3]);
	Enemy_Init({ 70.f,70.f }, { -300.f, -80.f }, ENEMY_IDLE, vecEnemy[4]);
	Enemy_Init({70.f,70.f }, { 800.f,150.f }, ENEMY_IDLE, vecEnemy[5]);
#pragma endregion Enemy
	
	menu->Init(cam);
	ParticleInitialize();
	MapTransitionInit();
}

void Level1_Update()
{
	//std::cout << AEFrameRateControllerGetFrameRate() << "\n";
	MapTransitionUpdate();

#pragma region PauseMenuTrigger

	menu->Update(cam);

#pragma endregion

	if (currScene == CurrentScene::PAUSE_SCENE || currScene == CurrentScene::CONTROL_SCENE || currScene == CurrentScene::QUIT_SCENE)
		return;

#pragma region PlayerUpdate
	if (currScene == MAIN_SCENE)
		PlayerUpdate(*player, inventoryOpen);
	if (AEInputCheckTriggered(AEVK_I))
	{
		inventoryOpen = !inventoryOpen;
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
	if (player->isAttacking)
	{
		for (Enemy& enemy : vecEnemy)
		{
			if (enemy.isAlive)
			{
				CheckWeaponCollision(player->equippedWeapon, enemy, *player);
			}
		}
	}

#pragma endregion

#pragma region EnemyUpdate
	for (Enemy& enemy : vecEnemy)
	{
		if (enemy.isAlive)
		{
			EnemyUpdateChoose(enemy, *player);
		}
	}
#pragma endregion

#pragma region GridSystem
	CheckPlayerGridCollision(grids2D, player);
	CheckEnemyGridCollision(grids2D, vecEnemy);
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
					if (AABBvsAABB(player->boxHeadFeet, grids2D[rows][cols].collisionBox))
					{
						player->collisionNormal = AABBNormalize(player->boxHeadFeet, grids2D[rows][cols].collisionBox);
						ResolveVerticalCollision(player->boxHeadFeet, grids2D[rows][cols].collisionBox,
							&player->collisionNormal, &player->obj.pos,
							&player->velocity, &player->onFloor, &player->gravityForce,
							&player->isFalling);
					}

					//Check horizontal box (Left arm -> Right arm)
					if (AABBvsAABB(player->boxArms, grids2D[rows][cols].collisionBox))
					{
						player->collisionNormal = AABBNormalize(player->boxArms, grids2D[rows][cols].collisionBox);
						ResolveHorizontalCollision(player->boxArms, grids2D[rows][cols].collisionBox,
							&player->collisionNormal, &player->obj.pos,
							&player->velocity);
					}
//(ENEMY AND BULLETS COLLISION CHECKING)
				for (Enemy& enemy : vecEnemy) {

					//Check vertical box (Head + Feet) 
					if (AABBvsAABB(enemy.boxHeadFeet, grids2D[rows][cols].collisionBox)) {
						enemy.collisionNormal = AABBNormalize(enemy.boxHeadFeet, grids2D[rows][cols].collisionBox);

						ResolveVerticalCollision(enemy.boxHeadFeet, grids2D[rows][cols].collisionBox,
						                         &enemy.collisionNormal, &enemy.obj.pos,
						                         &enemy.velocity, &enemy.onFloor, &enemy.gravityForce,
						                         &enemy.isFalling);
					}
					//Check horizontal box (Left arm -> Right arm)
					if (AABBvsAABB(enemy.boxArms, grids2D[rows][cols].collisionBox))
					{
						enemy.isCollision = true;
						enemy.collisionNormal = AABBNormalize(enemy.boxArms, grids2D[rows][cols].collisionBox);

						ResolveHorizontalCollision(enemy.boxArms, grids2D[rows][cols].collisionBox, &enemy.collisionNormal, &enemy.obj.pos,
							&enemy.velocity);
						enemy.loopIdle = false;
					}

					if (enemy.enemyType == ENEMY_FLY || enemy.enemyType == ENEMY_BOSS1)
					{
						for (Bullet& bullet : enemy.bullets)
						{
							if (AABBvsAABB(bullet.collisionBox, grids2D[rows][cols].collisionBox))
							{
								bullet.lifeTime = 0; //makes bullet erase
							}
						}
					}
				}
				break;
			case MAP_TRANSITION_GRID:
				if (AABBvsAABB(player->collisionBox, grids2D[rows][cols].collisionBox))
				{
					if (!transitionalImageOBJ.active)
					{
						transitionalImageOBJ.PlayMapTransition(TRANSITION_RIGHT, GAME_LOBBY);
					}
				}
				break;
			case EMPTY:
				break;
			}
		}
	}
#pragma endregion

#pragma region InventorySystem

	if (inventoryOpen)
	{
		//update item position
		Inventory::UpdateInventory(playerInventory, inventoryButton);
		s16 index = 0;


		for (ButtonGearUI& button : inventoryButton)
		{
			button.img.pTex = blank;
			AEVec2Set(&button.img.scale, 60.f, 60.f);
			AEVec2Set(&button.pos, (index % 5) * 90.f - 180.f, -(index / 5) * 90.f + 180.f);

			if (button.Item.ID < 0)
			{
				button.img.pTex = blank;
			}
			else
			{
				if (button.Item.quantity <= 0)
				{
					Item blank;
					blank.ID = -9999;
					button.Item = blank;
				}
				else
				{
					button.img.pTex = Gear[button.Item.ID];
				}
			}

			//button.isWeapon = false;
			index++;
		}
		//Hover collision with button && hold left mouse button

		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			s32 textX = 0;
			s32 textY = 0;

			s16 index = 0;

			AEInputGetCursorPosition(&textX, &textY);
			AEVec2 mousePos;
			mousePos.x = textX - AEGfxGetWindowWidth() * 0.5f;
			mousePos.y = AEGfxGetWindowHeight() * 0.5f - textY;

			for (ButtonGearUI& button : inventoryButton)
			{
				if (AETestPointToRect(&mousePos, &button.pos, button.img.scale.x, button.img.scale.y))
				{
					if (button.img.pTex != blank)
					{
						std::cout << button.Item.name << std::endl;
						//snap origin of img to mouse pos
						snapBack = index;
						break;
					}
					//button.Ptr();
				}
				index++;
			}
		}

		if (snapBack >= 0)
		{
			s32 textX = 0;
			s32 textY = 0;

			s16 index = 0;

			AEInputGetCursorPosition(&textX, &textY);
			AEVec2 mousePos;
			mousePos.x = textX - AEGfxGetWindowWidth() * 0.5f;
			mousePos.y = AEGfxGetWindowHeight() * 0.5f - textY;

			inventoryButton[snapBack].pos = mousePos;
		}

		if (AEInputCheckReleased(AEVK_LBUTTON))
		{
			s16 index = 0;
			if (snapBack >= 0)
			{
				for (ButtonGearUI& button : inventoryButton)
				{
					if (AETestRectToRect(&inventoryButton[snapBack].pos,
					                     inventoryButton[snapBack].img.scale.x,
					                     inventoryButton[snapBack].img.scale.y, &button.pos,
					                     button.img.scale.x,
					                     button.img.scale.y))
					{
						//Different items overlapping
						if (index != snapBack)
						{
							AEVec2Set(&inventoryButton[snapBack].pos, (snapBack % 5) * 90.f - 180.f,
								-(snapBack / 5.f) * 90.f + 180.f);

							//std::cout << "swap\n";
							ButtonGearUI tmp = button;
							button = inventoryButton[snapBack];
							inventoryButton[snapBack] = tmp;

							if (playerInventory.size() <= index)
							{
								size_t oldsize = playerInventory.size();
								playerInventory.resize(index + 1);
								for (size_t x = oldsize; x < playerInventory.size(); x++)
								{
									playerInventory[x].ID = -9999;
								}
							}
							Inventory::SwapInventory(playerInventory[index], playerInventory[snapBack]);
							AEVec2Set(&inventoryButton[snapBack].pos, (snapBack % 5) * 90.f - 180.f,
								-(snapBack / 5) * 90.f + 180.f);

							AEVec2Set(&button.pos, (index % 5) * 90.f - 180.f,
								-(index / 5.f) * 90.f + 180.f);

							snapBack = -1;
							break;
						}
					}
					index++;
				}

				if (snapBack >= 0)
				{
					AEVec2Set(&inventoryButton[snapBack].pos, (snapBack % 5) * 90.f - 180.f,
						-(snapBack / 5.f) * 90.f + 180.f);
					snapBack = -1;
				}
			}
		}


		if (AEInputCheckTriggered(AEVK_RBUTTON))
		{
			s16 index = 0;
			s32 textX = 0;
			s32 textY = 0;


			index = 0;
			AEInputGetCursorPosition(&textX, &textY);
			AEVec2 mousePos;
			mousePos.x = textX - AEGfxGetWindowWidth() * 0.5f;
			mousePos.y = AEGfxGetWindowHeight() * 0.5f - textY;

			for (ButtonGearUI& button : inventoryButton)
			{
				if (AETestPointToRect(&mousePos, &button.pos, button.img.scale.x, button.img.scale.y))
				{
					if (button.img.pTex != blank)
					{
						Inventory::UseItem(index, button, *player);
						if (button.Item.quantity == 0)
						{
							button.img.pTex = blank;
						}
						break;
					}
				}

				index++;
			}

			index = 0;
			for (ButtonGearUI& button : Inventory::equipmentDisplay)
			{
				button.img.pTex = Gear[button.Item.ID];
				AEVec2Set(&button.img.scale, 60.f, 60.f);
				AEVec2Set(&button.pos, -375.f, -index * 90.f + 180.f);
				index++;
			}
		}
	}

	if (player->burningEffect)
	{
		if (timer >= burningEffectDuration)
		{
			AEVec2 hpLerp{0, 0};
			hpLerp.x = (f32)hp;
			hpLerp.y = 0.f;
			AEVec2 end{0, 0};
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
		ParticleEmit(5, player->obj.pos.x, player->obj.pos.y, 5.f, 5.f, PI / 3, ParticleType::TEST);
	}
	if (AEInputCheckTriggered(AEVK_H))
	{
		ParticleEmit(5, player->obj.pos.x, player->obj.pos.y, 20.f, 20.f, PI / 4, ParticleType::TEST);
	}
	ParticleUpdate();

#pragma endregion

#pragma region CameraUpdate

	cam->UpdatePos(*player, grids2D[0][0].collisionBox.minimum.x, grids2D[0][MAP_COLUMN_SIZE - 1].collisionBox.maximum.x, grids2D[MAP_ROW_SIZE - 1][0].collisionBox.minimum.y, grids2D[0][0].collisionBox.maximum.y);

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

	for (s16 i = 0; i < platformVectors.size(); i++)
	{
		AEGfxSetTransform(platformVectors[i].transformation.m);
		AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);
	}

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

#pragma region Player_Render

	AEGfxTextureSet(player->obj.img.pTex, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(player->obj.pos.x, player->obj.pos.y, 0.f, player->obj.img.scale.x,
	                                                player->obj.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	for (Enemy& enemy : vecEnemy) {
		if (enemy.isAlive) {


			if (enemy.isShooting) {
				AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

				AEGfxTextureSet(enemy.angryTex, 0, 0); 
				AEGfxSetTransform(ObjectTransformationMatrixSet(enemy.obj.pos.x, enemy.obj.pos.y, 0.f, enemy.obj.img.scale.x, enemy.obj.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

				DrawBullets(enemy, pWhiteSquareMesh); //drawing bullets
			}
			else
			{
				AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

				AEGfxTextureSet(enemy.obj.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(enemy.obj.pos.x, enemy.obj.pos.y, 0.f,
				                                                enemy.obj.img.scale.x, enemy.obj.img.scale.y).m);
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
		AEGfxSetTransform(ObjectTransformationMatrixSet(player->equippedWeapon->position.x,
		                                                player->equippedWeapon->position.y, 0.f,
		                                                player->equippedWeapon->scale.x,
		                                                player->equippedWeapon->scale.y).m);
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
	if (inventoryOpen)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

		f32 x, y;
		AEGfxGetCamPosition(&x, &y);

		AEGfxTextureSet(inventoryBackground.img.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(x,y, 0.f,
			inventoryBackground.img.scale.x,
			inventoryBackground.img.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);


		AEGfxTextureSet(equipmentBackground.img.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(
			equipmentBackground.pos.x + x,
			equipmentBackground.pos.y +y, 0.f,
			equipmentBackground.img.scale.x,
			equipmentBackground.img.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		for (ButtonGearUI button : inventoryButton)
		{
			if (button.Item.ID < 0)
			{
				AEGfxTextureSet(button.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x +x,
				                                                button.pos.y +y, 0.f,
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

MapTransitionDraw();
}

void Level1_Free()
{
	FreeEnemy(vecEnemy); //loops thru all eney tex and free them.
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


	for (s16 i = 0; i < 8; ++i)
	{
		AEGfxTextureUnload(Gear[i]);
	}


	AEGfxTextureUnload(inventoryBackground.img.pTex);
	AEGfxTextureUnload(equipmentBackground.img.pTex);
	AEGfxTextureUnload(player->obj.img.pTex);

	AEGfxMeshFree(pMeshGrey);
	AEGfxMeshFree(pMeshYellow);
	AEGfxMeshFree(pMeshRed);
	AEGfxMeshFree(pLineMesh);
	AEGfxMeshFree(pMeshRedBar);
	AEGfxMeshFree(pWhiteSquareMesh);

	delete player->equippedWeapon;
	delete player;
	delete cam;
	delete menu;
}

void CheckPlayerGridCollision(Grids2D gridMap[][MAP_COLUMN_SIZE], Player* player)
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
				if (AABBvsAABB(player->boxHeadFeet, gridMap[playerIndexY][playerIndexX].collisionBox)) {
					player->collisionNormal = AABBNormalize(player->boxHeadFeet, gridMap[playerIndexY][playerIndexX].collisionBox);
					ResolveVerticalCollision(player->boxHeadFeet, gridMap[playerIndexY][playerIndexX].collisionBox, &player->collisionNormal, &player->obj.pos,
						&player->velocity, &player->onFloor, &player->gravityForce, &player->isFalling);
				}

				//Check horizontal box (Left arm -> Right arm)
				if (AABBvsAABB(player->boxArms, gridMap[playerIndexY][playerIndexX].collisionBox))
				{
					player->collisionNormal = AABBNormalize(player->boxArms, gridMap[playerIndexY][playerIndexX].collisionBox);
					ResolveHorizontalCollision(player->boxArms, gridMap[playerIndexY][playerIndexX].collisionBox, &player->collisionNormal, &player->obj.pos,
						&player->velocity);
				}
				break;
			case MAP_TRANSITION_GRID:
				if (AABBvsAABB(player->collisionBox, gridMap[playerIndexY][playerIndexX].collisionBox))
				{
					//std::cout << "Collided\n";MainMenu_Song
					if (!transitionalImageOBJ.active)
					{
						transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1);
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

void CheckEnemyGridCollision(Grids2D gridMap[][MAP_COLUMN_SIZE], std::vector<Enemy>& enemy)
{
	for (Enemy& tmp : enemy) {

		int enemyIndexY = (int)((AEGfxGetWindowHeight() * 0.5f - tmp.obj.pos.y) / (gridMap[0][0].size.x));

		for (int i = 0; i <= (int)(tmp.obj.img.scale.x * 2 / gridMap[0][0].size.x); i++)
		{
			int enemyIndexX = (int)((tmp.obj.pos.x + AEGfxGetWindowWidth() * 0.5f) / (gridMap[0][0].size.x));
			for (int j = 0; j <= (int)(tmp.obj.img.scale.x * 2 / gridMap[0][0].size.x); j++)
			{
				switch (gridMap[enemyIndexY][enemyIndexX].typeOfGrid)
				{
				case NORMAL_GROUND:
					if (AABBvsAABB(tmp.boxHeadFeet, gridMap[enemyIndexY][enemyIndexX].collisionBox)) {
						tmp.collisionNormal = AABBNormalize(tmp.boxHeadFeet, gridMap[enemyIndexY][enemyIndexX].collisionBox);
						ResolveVerticalCollision(tmp.boxHeadFeet, gridMap[enemyIndexY][enemyIndexX].collisionBox, &tmp.collisionNormal, &tmp.obj.pos,
							&tmp.velocity, &tmp.onFloor, &tmp.gravityForce, &tmp.isFalling);
					}

					if (AABBvsAABB(tmp.boxArms, gridMap[enemyIndexY][enemyIndexX].collisionBox))
					{
						tmp.collisionNormal = AABBNormalize(tmp.boxArms, gridMap[enemyIndexY][enemyIndexX].collisionBox);
						ResolveHorizontalCollision(tmp.boxArms, gridMap[enemyIndexY][enemyIndexX].collisionBox, &tmp.collisionNormal, &tmp.obj.pos,
							&tmp.velocity);
					}
					break;
				}
				enemyIndexX += 1;
			}
			enemyIndexY += 1;
		}
	}
}