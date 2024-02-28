#include "GameState_Level1.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "TransformMatrix.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyUtils.h"
#include <iostream>

#include "CSVMapLoader.h"
#include "GridTypesList.h"
#include "Physics.h"
#include "CollisionShape.h" //For Vertical + Horizontal collision
#include <string>
#include "Camera.h"
#include <vector>

#include "MapPlatformGenerate.h"
#include "NonPlayableCharacters.h"

namespace {

	AEGfxVertexList* pLineMesh;
	AEGfxVertexList* pMeshYellow;
	AEGfxVertexList* pMeshRed;
	AEGfxVertexList* pMeshGrey;
	AEGfxVertexList* pWhiteSquareMesh;

	Grids2D grids2D[MAP_ROW_SIZE][MAP_COLUMN_SIZE]; //Initializing map
	std::vector<std::vector<MapCell>> gameMap(MAP_ROW_SIZE, std::vector<MapCell>(MAP_COLUMN_SIZE)); //Map for this level

	Platforms movingObject[3]; //Indicate total number of movingObjects
	//const int size = 5;	//size so that loops work
	//Enemy* enemy[size];
	std::vector<Enemy> vecEnemy; //enemy container

	Player* player;

	AEGfxTexture* background;

	Camera* cam;

	s8 pFont;

#pragma region UserInterface
	//User Health
	AEGfxVertexList* pMeshRedBar;
	AEGfxTexture* HealthBorder;
	
#pragma endregion UserInterface

#pragma region PauseMenu

	enum CurrentScene {
		MainScene,
		ControlScene,
		QuitScene
	};

	enum Interactable {
		Resume,
		Return,
		Controls,
		Quit
	};

	AEGfxTexture* PauseMenuBackground;
	AEGfxTexture* ButtonFrame;

	//Return to Main Menu Option - "Return to Main Menu" or "Exit to Main Menu"
	// //Restart Game Option - "Restart Game", "Return to Overworld", "New Game", "Exit Level"
	//How To Play - "How to Play" or "Controls"
	//Resume Game Option - "Resume Game"

	Button PauseMenuButtons[4];
	Button QuitToMainmenu[2];
	Button BackButton;

	int currScene;

#pragma endregion PauseMenu

	bool inventory_open = false;

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
}

AEGfxTexture* bulletTex = nullptr;


//temporary code section
void Equip(int index, ButtonGearUI tmp)
{
	equipmentDisplay[index].img.pTex = tmp.img.pTex;
	if (!tmp.isWeapon)
	{
		gear_equipped++;
		if (gear_equipped == 4)
			hp *= 2;
	}
	else
	{
		player->burningEffect = true;
	}
}

AEGfxVertexList* GenerateSquareMesh(u32 MeshColor)
{
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, MeshColor, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, MeshColor, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, MeshColor, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, MeshColor, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, MeshColor, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, MeshColor, 0.0f, 0.0f);  // top-left: blue

	return AEGfxMeshEnd();
}

AEGfxVertexList* GenerateLineMesh(u32 MeshColor)
{
	AEGfxMeshStart();

	AEGfxVertexAdd(-0.5f, 0.f, MeshColor, 1.0f, 1.0f);
	AEGfxVertexAdd(0.5f, 0.f, MeshColor, 1.0f, 1.0f);

	return AEGfxMeshEnd();
}

#pragma region PauseMenu

void ResumeGame() {}
void ReturnLobby() {}
void OpenControls() {}
void QuitMainmenu() {}
void QuitConfirmation() {}
void BackPauseMenu() {}
#pragma endregion PauseMenu

void Level1_Load()
{

	//loading texture only, push back into the vector
	Enemy_Load(ENEMY_CHARGER, vecEnemy);
	Enemy_Load(ENEMY_BOSS1, vecEnemy);
	Enemy_Load(ENEMY_JUMPER, vecEnemy);
	Enemy_Load(ENEMY_FLY, vecEnemy);

	bulletTex = AEGfxTextureLoad("Assets/RedCircle.png");


	player = PlayerInitialize("Assets/Kronii_Pixel.png", { 70.f,70.f }, { -750.f,-155.f }, { 40.f,0.f }, true);
	background = AEGfxTextureLoad("Assets/Background2.jpg");
	const char* fileName = "Assets/GameMap.csv"; //Change name as per level
	//Load map
	if (MapLoader(fileName, gameMap, MAP_ROW_SIZE, MAP_COLUMN_SIZE))
	{
		PrintMap(gameMap, MAP_ROW_SIZE, MAP_COLUMN_SIZE); //Just for checking if the map data is stored properly
	}

	pFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 72);

	inventoryBackground.img.pTex = AEGfxTextureLoad("Assets/panel_brown.png");

	Gear1 = AEGfxTextureLoad("Assets/item_.png");
	Gear2 = AEGfxTextureLoad("Assets/item_helmet.png");
	weapon3 = AEGfxTextureLoad("Assets/item_sword.png");
	Gear4 = AEGfxTextureLoad("Assets/tile_archColumns.png");
	Gear5 = AEGfxTextureLoad("Assets/tile_archHalf.png");
	blank = AEGfxTextureLoad("Assets/panelInset_beige.png");

	equipmentBackground.img.pTex = AEGfxTextureLoad("Assets/panel_brown.png");

	LoadNPC();

#pragma region Mesh Creations
	pMeshGrey = GenerateSquareMesh(0xFFa9a9a9);
	pMeshRed = GenerateSquareMesh(0xFFFF0000);
	pMeshRedBar = GenerateSquareMesh(0xFFFF0000);
	pMeshYellow = GenerateSquareMesh(0xFFFFFF00);
	pWhiteSquareMesh = GenerateSquareMesh(0xFFFFFFFF);

	//Drawing line
	pLineMesh = GenerateLineMesh(0xFF000000);
#pragma endregion

	HealthBorder = AEGfxTextureLoad("Assets/UI_Sprite/Border/panel-border-015.png");

#pragma region PauseMenu
	PauseMenuBackground = AEGfxTextureLoad("Assets/UI_Sprite/Panel/panel-001.png");
	ButtonFrame = AEGfxTextureLoad("Assets/UI_Sprite/Border/panel-border-015.png");
#pragma endregion PauseMenu
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

	AEVec2Set(&inventoryBackground.img.scale, 500.f, 500.f);

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

	cam = new Camera(player->obj.pos);
	//Need to place the objects one by one 
	CreatePlatform(1200.f, -300.f, 140.f, 30.f, 3.f, HORIZONTAL_MOVING_PLATFORM, movingObject[0]);
	CreatePlatform(1200.f, 0.f, 140.f, 30.f, 2.f, VERTICAL_MOVING_PLATFORM, movingObject[1]);
	CreatePlatform(1400.f, 0.f, 140.f, 30.f, 2.f, DIAGONAL_PLATFORM, movingObject[2]);

	//Initialize NPCs
	InitializeNPC();

	//looping thru to init all enemy variables
	Enemy_Init({ 70.f,70.f }, { 1200.f,-320.f }, ENEMY_IDLE, vecEnemy[0]);
	Enemy_Init({ 70.f,70.f }, { -500.f,-100.f }, ENEMY_IDLE, vecEnemy[1]);
	Enemy_Init({ 70.f,70.f }, { -500.f,250.f }, ENEMY_IDLE, vecEnemy[2]);
	Enemy_Init({ 70.f,70.f }, { 300.f,250.f }, ENEMY_IDLE, vecEnemy[3]);

#pragma region PauseMenu

	for (size_t i = 0; i < sizeof(PauseMenuButtons) / sizeof(PauseMenuButtons[0]); i++)
	{
		PauseMenuButtons[i].pTex = ButtonFrame;
		AEVec2Set(&PauseMenuButtons[i].scale, 250.f, 80.f);
		AEVec2Set(&PauseMenuButtons[i].pos, 0.f, 100.f - 100.f * i);

		switch (i)
		{
		case Interactable::Resume:
			PauseMenuButtons[i].Ptr = ResumeGame;
			break;
		case Interactable::Return:
			PauseMenuButtons[i].Ptr = ReturnLobby;
			break;
		case Interactable::Controls:
			PauseMenuButtons[i].Ptr = OpenControls;
			break;
		case Interactable::Quit:
			PauseMenuButtons[i].Ptr = QuitConfirmation;
			break;
		default:
			break;
		}
	}

	QuitToMainmenu[0].pTex = ButtonFrame;
	AEVec2Set(&QuitToMainmenu[0].scale, 250.f, 80.f);
	AEVec2Set(&QuitToMainmenu[0].pos, 250.f, 0.f);
	QuitToMainmenu[0].Ptr = BackPauseMenu;

	QuitToMainmenu[1].pTex = ButtonFrame;
	AEVec2Set(&QuitToMainmenu[1].scale, 250.f, 80.f);
	AEVec2Set(&QuitToMainmenu[1].pos, -250.f, 0.f);
	QuitToMainmenu[1].Ptr = QuitMainmenu;

	BackButton.pTex = ButtonFrame;
	AEVec2Set(&BackButton.scale, 250.f, 80.f);
	AEVec2Set(&BackButton.pos, 250.f, 0.f);
	BackButton.Ptr = BackPauseMenu;

	currScene = CurrentScene::MainScene;
#pragma endregion PauseMenu
}

void Level1_Update()
{
	PlayerUpdate(*player);
	cam->UpdatePos(*player);

#pragma region PlayerButton

	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		player->isAttacking = true;
	}

	if (AEInputCheckTriggered(AEVK_ESCAPE))
	{

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

#pragma endregion PlayerButton


	if (0==AESysDoesWindowExist())
	{
		next = GameStates::Quit;
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

	for (Enemy& enemy : vecEnemy) {
		if (enemy.isAlive) {
			Enemy_Update_Choose(enemy, *player);
		}
	}

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
				if (AABBvsAABB(player->boxHeadFeet, grids2D[rows][cols].collisionBox)) {
					 player->collisionNormal = AABBNormalize(player->boxHeadFeet, grids2D[rows][cols].collisionBox);

					 ResolveVerticalCollision(player->boxHeadFeet, grids2D[rows][cols].collisionBox, &player->collisionNormal, &player->obj.pos,
						 &player->velocity, &player->onFloor);
				}

				//Check horizontal box (Left arm -> Right arm)
				if (AABBvsAABB(player->boxArms, grids2D[rows][cols].collisionBox)) {
					player->collisionNormal = AABBNormalize(player->boxArms, grids2D[rows][cols].collisionBox);
					ResolveHorizontalCollision(player->boxArms, grids2D[rows][cols].collisionBox, &player->collisionNormal, &player->obj.pos,
						&player->velocity, &player->onFloor);
				}
//(ENEMY AND BULLETS COLLISION CHECKING)
//is this efficient? 
				for (Enemy& enemy : vecEnemy) {

					//Check vertical box (Head + Feet) 
					if (AABBvsAABB(enemy.boxHeadFeet, grids2D[rows][cols].collisionBox)) {
						enemy.collisionNormal = AABBNormalize(enemy.boxHeadFeet, grids2D[rows][cols].collisionBox);

						ResolveVerticalCollision(enemy.boxHeadFeet, grids2D[rows][cols].collisionBox, &enemy.collisionNormal, &enemy.obj.pos,
							&enemy.velocity, &enemy.onFloor);

					}
					//Check horizontal box (Left arm -> Right arm)
					if (AABBvsAABB(enemy.boxArms, grids2D[rows][cols].collisionBox)) {
						enemy.isCollision = true;
						enemy.collisionNormal = AABBNormalize(enemy.boxArms, grids2D[rows][cols].collisionBox);

						ResolveHorizontalCollision(enemy.boxArms, grids2D[rows][cols].collisionBox, &enemy.collisionNormal, &enemy.obj.pos,
							&enemy.velocity, &enemy.onFloor);
						enemy.loop_idle = false;
					}


					for ( Bullet& bullet : enemy.bullets) {
						if (AABBvsAABB(bullet.collisionBox, grids2D[rows][cols].collisionBox)) {
							bullet.lifetime = 0; //makes bullet erase

						}
					}
				}

				break;
			case EMPTY:
				break;
			}
		}
	}

	if (AEInputCheckTriggered(AEVK_I))
	{
		inventory_open = !inventory_open;
	}

	if (inventory_open)
	{
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			int index = 0;
			AEVec2 mousePos{ cam->GetCameraWorldPoint().x,cam->GetCameraWorldPoint().y };
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

	//Testing moving platform logic

	UpdatePlatforms(movingObject, 3, *player); //Numbers based on how many moving platforms
	UpdateNPC();
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
	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, 0.f, 0.f, 4200, 1080.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	//For Grid Drawing
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
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

	AEGfxSetTransform(movingObject[0].transformation.m);
	AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(movingObject[1].transformation.m);
	AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(movingObject[2].transformation.m);
	AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);

	DrawNPC(*pWhiteSquareMesh);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(player->obj.img.pTex, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(player->obj.pos.x, player->obj.pos.y, 0.f, player->obj.img.scale.x, player->obj.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);



	for (Enemy& enemy : vecEnemy) {
		if (enemy.isAlive) {
			if (enemy.isShooting) {
				//AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 0.0f);//this line makes enemy go red when shooting
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

				if (enemy.enemyType == ENEMY_BOSS1 && enemy.wing1.isAlive) {
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

					AEGfxTextureSet(enemy.wing1.obj.img.pTex, 0, 0);
					AEGfxSetTransform(ObjectTransformationMatrixSet(enemy.wing1.obj.pos.x, enemy.wing1.obj.pos.y, 0.f, enemy.wing1.obj.img.scale.x, enemy.wing1.obj.img.scale.y).m);
					AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
				}
				if (enemy.enemyType == ENEMY_BOSS1 && enemy.wing2.isAlive) {
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

					AEGfxTextureSet(enemy.wing2.obj.img.pTex, 0, 0);
					AEGfxSetTransform(ObjectTransformationMatrixSet(enemy.wing2.obj.pos.x, enemy.wing2.obj.pos.y, 0.f, enemy.wing2.obj.img.scale.x, enemy.wing2.obj.img.scale.y).m);
					AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
				}
			}
		}
	}

	if (player->isAttacking)
	{
		AEGfxSetTransform(ObjectTransformationMatrixSet(player->equippedWeapon.position.x, player->equippedWeapon.position.y, 0.f, player->equippedWeapon.Scale.x, player->equippedWeapon.Scale.y).m);
		AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);
	}

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(ObjectTransformationMatrixSet(cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y, 0.f, (f32)AEGfxGetWindowWidth(), 1.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(ObjectTransformationMatrixSet(cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y, 0.5f * PI, (f32)AEGfxGetWindowWidth(), 1.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);


	AEGfxSetTransform(ObjectTransformationMatrixSet(-800.f + hp + cam->GetCameraWorldPoint().x, 450.f + cam->GetCameraWorldPoint().y, 0, hp * 2.f, 80.f).m);
	AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(HealthBorder, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(-800.f + hp + cam->GetCameraWorldPoint().x, 450.f + cam->GetCameraWorldPoint().y, 0, hp * 2.f, 80.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	std::string str = std::to_string(hp);
	const char* pText = str.c_str();
	f32 width, height;
	AEGfxGetPrintSize(pFont, pText, 0.5f, &width, &height);
	AEGfxPrint(pFont, pText, -width / 2 - 0.9f, -width / 2 + 0.97f, 0.5f, 1, 1, 1, 1);

	//Inventory images
	if (inventory_open)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

		AEGfxTextureSet(inventoryBackground.img.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y, 0.f, inventoryBackground.img.scale.x, inventoryBackground.img.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		for (ButtonGearUI button : inventoryButton)
		{
			AEGfxTextureSet(button.img.pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + cam->GetCameraWorldPoint().x, button.pos.y + cam->GetCameraWorldPoint().y, 0.f, button.img.scale.x, button.img.scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}

		AEGfxTextureSet(equipmentBackground.img.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(equipmentBackground.pos.x + cam->GetCameraWorldPoint().x, equipmentBackground.pos.y + cam->GetCameraWorldPoint().y, 0.f, equipmentBackground.img.scale.x, equipmentBackground.img.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		for (ButtonGearUI button : equipmentDisplay)
		{
			AEGfxTextureSet(button.img.pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + cam->GetCameraWorldPoint().x, button.pos.y + cam->GetCameraWorldPoint().y, 0.f, button.img.scale.x, button.img.scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}
	}

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	switch (currScene)
	{
	case CurrentScene::MainScene:
	{
		for (size_t i = 0; i < sizeof(PauseMenuButtons) / sizeof(PauseMenuButtons[0]); i++)
		{
			AEGfxTextureSet(PauseMenuButtons[i].pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(PauseMenuButtons[i].pos.x, PauseMenuButtons[i].pos.y, 0.f, PauseMenuButtons[i].scale.x, PauseMenuButtons[i].scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}

		f32 width, height;

		const char* pText = "Start";
		AEGfxGetPrintSize(pFont, pText, 0.5f, &width, &height);
		AEGfxPrint(pFont, pText, -width / 2, -height / 2 + 0.22f, 0.5f, 1, 1, 1, 1);

		const char* pText1 = "Load";
		AEGfxGetPrintSize(pFont, pText1, 0.5f, &width, &height);
		AEGfxPrint(pFont, pText1, -width / 2, -height / 2, 0.5f, 1, 1, 1, 1);

		const char* pText2 = "Credit";
		AEGfxGetPrintSize(pFont, pText2, 0.5f, &width, &height);
		AEGfxPrint(pFont, pText2, -width / 2, -height / 2 - 0.22f, 0.5f, 1, 1, 1, 1);

		const char* pText3 = "Controls";
		AEGfxGetPrintSize(pFont, pText3, 0.5f, &width, &height);
		AEGfxPrint(pFont, pText3, -width / 2, -height / 2 - 0.44f, 0.5f, 1, 1, 1, 1);
		break;
	}
	case CurrentScene::ControlScene:
	{
		AEGfxTextureSet(BackButton.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(BackButton.pos.x, BackButton.pos.y, 0.f, BackButton.scale.x, BackButton.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		f32 width, height;

		const char* pText = "Back";
		AEGfxGetPrintSize(pFont, pText, 0.5f, &width, &height);
		AEGfxPrint(pFont, pText, -width / 2 - 0.85f, -height / 2 - 0.9f, 0.5f, 1, 1, 1, 1);
		break;
	}
	case CurrentScene::QuitScene:
	{
		AEGfxTextureSet(QuitToMainmenu[0].pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(QuitToMainmenu[0].pos.x, QuitToMainmenu[0].pos.y, 0.f, QuitToMainmenu[0].scale.x, QuitToMainmenu[0].scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(QuitToMainmenu[1].pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(QuitToMainmenu[1].pos.x, QuitToMainmenu[1].pos.y, 0.f, QuitToMainmenu[1].scale.x, QuitToMainmenu[1].scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		f32 width, height;

		const char* pText = "Yes";
		AEGfxGetPrintSize(pFont, pText, 0.5f, &width, &height);
		AEGfxPrint(pFont, pText, -width / 2 - 0.31f, -height / 2, 0.5f, 1, 1, 1, 1);

		const char* pText1 = "No";
		AEGfxGetPrintSize(pFont, pText1, 0.5f, &width, &height);
		AEGfxPrint(pFont, pText1, -width / 2 + 0.31f, -height / 2, 0.5f, 1, 1, 1, 1);
		break;
	}
	default:
		break;
	}
}

void Level1_Free()
{
	//Free the bullet tex
	AEGfxTextureUnload(bulletTex);
	//Free Enemy Vector
	vecEnemy.clear();


	//Free vectors
	gameMap.clear();
	gameMap.resize(0);

	FreeNPC();
}

void Level1_Unload()
{
	//Free meshes
	AEGfxMeshFree(pMeshGrey);
	AEGfxMeshFree(pMeshYellow);
	AEGfxMeshFree(pMeshRed);
	AEGfxMeshFree(pLineMesh);
	AEGfxMeshFree(pWhiteSquareMesh);

	delete cam;
}
