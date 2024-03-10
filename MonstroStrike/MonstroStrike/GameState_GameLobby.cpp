#include "LevelHeaders.h"
#include "MapTransition.h"
namespace {
	AEGfxVertexList* pLineMesh;
	AEGfxVertexList* pMeshYellow;
	AEGfxVertexList* pMeshRed;
	AEGfxVertexList* pMeshGrey;
	AEGfxVertexList* pWhiteSquareMesh;

	Grids2D grids2D[MAP_ROW_LOBBY_SIZE][MAP_COLUMN_LOBBY_SIZE]; //Initializing map
	std::vector<std::vector<MapCell>> gameMap; //Map for this level
	std::vector<AEVec2> NPCPositions;


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
		PauseScene,
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

	Sprite_V2 pauseMenuBackground;

	int currScene;

#pragma endregion PauseMenu

	bool inventory_open = false;
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
}

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

void ResumeGame() { currScene = CurrentScene::MainScene; }
void ReturnLobby() { /* go back to village aka safe spot */ }
void OpenControls() { currScene = CurrentScene::ControlScene; }
void QuitMainmenu() { next = GameStates::SplashScreen; }
void QuitConfirmation() { currScene = CurrentScene::QuitScene; }
void BackPauseMenu() { currScene = CurrentScene::PauseScene; }

#pragma endregion PauseMenu

void GameLobby_Load()
{
	player = PlayerInitialize("Assets/Border.png", { 70.f,70.f }, { 0.f,-400.f }, { 40.f,0.f }, true);
	background = AEGfxTextureLoad("Assets/Background2.jpg");
	const char* fileName = "Assets/GameMap_Lobby.csv"; //Change name as per level
	//Load map
	if (MapLoader(fileName, gameMap, MAP_ROW_LOBBY_SIZE, MAP_COLUMN_LOBBY_SIZE))
	{
		PrintMap(gameMap, MAP_ROW_LOBBY_SIZE, MAP_COLUMN_LOBBY_SIZE); //Just for checking if the map data is stored properly
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
	ParticleLoad();

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
	PauseMenuBackground = AEGfxTextureLoad("Assets/UI_Sprite/Transparent center/panel-transparent-center-015.png");
	ButtonFrame = AEGfxTextureLoad("Assets/UI_Sprite/Border/panel-border-015.png");
#pragma endregion PauseMenu
}

void GameLobby_Initialize()
{
	MapTransitionInit(player->obj.pos);

	//Initializing grid data
	for (s16 rows = 0; rows < MAP_ROW_LOBBY_SIZE; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_LOBBY_SIZE; cols++)
		{
			switch (gameMap[rows][cols].symbol) //For checking the map symbol from the csv file
			{
			case 0:
				grids2D[rows][cols].typeOfGrid = EMPTY;
				break;
			case 1:
				grids2D[rows][cols].typeOfGrid = NORMAL_GROUND;
				break;
			case 9:
				grids2D[rows][cols].typeOfGrid = NONE;
				break;
			case 10:
				grids2D[rows][cols].typeOfGrid = NPC_BLACKSMITH_A_POS;
				break;
			case 11:
				grids2D[rows][cols].typeOfGrid = NPC_BLACKSMITH_B_POS;
				break;
			case 12:
				grids2D[rows][cols].typeOfGrid = NPC_QUEST_GIVER_POS;
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

	//For Initializing the grids
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
			default:
				break;
			}
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

	//Initialize NPCs
	InitializeNPC(NPCPositions);
#pragma region PauseMenu

	for (size_t i = 0; i < sizeof(PauseMenuButtons) / sizeof(PauseMenuButtons[0]); i++)
	{
		PauseMenuButtons[i].pTex = ButtonFrame;
		AEVec2Set(&PauseMenuButtons[i].scale, 250.f, 80.f);
		AEVec2Set(&PauseMenuButtons[i].pos, cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y - 100.f * i + 100.f);

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
	AEVec2Set(&QuitToMainmenu[0].pos, 250.f + cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y);
	QuitToMainmenu[0].Ptr = BackPauseMenu;

	QuitToMainmenu[1].pTex = ButtonFrame;
	AEVec2Set(&QuitToMainmenu[1].scale, 250.f, 80.f);
	AEVec2Set(&QuitToMainmenu[1].pos, -250.f + cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y);
	QuitToMainmenu[1].Ptr = QuitMainmenu;

	BackButton.pTex = ButtonFrame;
	AEVec2Set(&BackButton.scale, 250.f, 80.f);
	AEVec2Set(&BackButton.pos, 250.f + cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y);
	BackButton.Ptr = BackPauseMenu;

	currScene = CurrentScene::MainScene;

	pauseMenuBackground.pTex = PauseMenuBackground;
	pauseMenuBackground.scale.x = 1000.f;
	pauseMenuBackground.scale.y = 500.f;
	pauseMenuBackground.pos = cam->GetCameraWorldPoint();
#pragma endregion PauseMenu

	ParticleInitialize();

}

void GameLobby_Update()
{

	PlayerUpdate(*player);
	cam->UpdatePos(*player);

	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		//player->isAttacking = true;
		s32 x, y;
		AEInputGetCursorPosition(&x, &y);
		AEVec2 mousePos{ 0,0 };
		mousePos.x = x - AEGfxGetWindowWidth() * 0.5f;
		mousePos.y = AEGfxGetWindowHeight() * 0.5f - y;

		switch (currScene)
		{
		case CurrentScene::PauseScene:
		{
			for (size_t i = 0; i < sizeof(PauseMenuButtons) / sizeof(PauseMenuButtons[0]); i++)
			{
				AEVec2 translateOrigin = PauseMenuButtons[i].pos;
				translateOrigin.x -= cam->GetCameraWorldPoint().x;
				translateOrigin.y -= cam->GetCameraWorldPoint().y;
				if (AETestPointToRect(&mousePos, &translateOrigin, PauseMenuButtons[i].scale.x, PauseMenuButtons[i].scale.y))
					PauseMenuButtons[i].Ptr();
			}
			break;
		}
		case CurrentScene::ControlScene:
		{
			AEVec2 translateOrigin = BackButton.pos;
			translateOrigin.x -= cam->GetCameraWorldPoint().x;
			translateOrigin.y -= cam->GetCameraWorldPoint().y;
			if (AETestPointToRect(&mousePos, &translateOrigin, BackButton.scale.x, BackButton.scale.y))
				BackButton.Ptr();
			break;
		}
		case CurrentScene::QuitScene:
		{
			AEVec2 translateOrigin = QuitToMainmenu[0].pos;
			translateOrigin.x -= cam->GetCameraWorldPoint().x;
			translateOrigin.y -= cam->GetCameraWorldPoint().y;
			if (AETestPointToRect(&mousePos, &translateOrigin, QuitToMainmenu[0].scale.x, QuitToMainmenu[0].scale.y))
				QuitToMainmenu[0].Ptr();

			translateOrigin = QuitToMainmenu[1].pos;
			translateOrigin.x -= cam->GetCameraWorldPoint().x;
			translateOrigin.y -= cam->GetCameraWorldPoint().y;
			if (AETestPointToRect(&mousePos, &translateOrigin, QuitToMainmenu[1].scale.x, QuitToMainmenu[1].scale.y))
				QuitToMainmenu[1].Ptr();
			break;
		}
		default:
			break;
		}
	}

	if (AEInputCheckTriggered(AEVK_ESCAPE))
	{
		if (currScene == CurrentScene::MainScene)
		{
			//rmb to freeze game update
			currScene = CurrentScene::PauseScene;

			for (size_t i = 0; i < sizeof(PauseMenuButtons) / sizeof(PauseMenuButtons[0]); i++)
				AEVec2Set(&PauseMenuButtons[i].pos, cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y - 100.f * i + 100.f);

			AEVec2Set(&QuitToMainmenu[0].pos, 250.f + cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y);
			AEVec2Set(&QuitToMainmenu[1].pos, -250.f + cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y);

			AEVec2Set(&BackButton.pos, 250.f + cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y);
		}
		else
		{
			//unfreeze the game
			currScene = CurrentScene::MainScene;
		}
	}

	if (0 == AESysDoesWindowExist())
	{
		next = GameStates::Quit;
	}

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
				if (AABBvsAABB(player->collisionBox, grids2D[rows][cols].collisionBox))
				{
					//std::cout << "Collided\n";
					if (!transitionalImageOBJ.active)
					{
						transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, Area1);
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
		//update item position
		Inventory::UpdateInventory(Player_Inventory, inventoryButton);

		//Hover collision with button && hold left mouse button


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

	UpdateNPC();
	ParticleUpdate();
	MapTransitionUpdate(player->obj.pos);
}

void GameLobby_Draw()
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
	for (s16 rows = 0; rows < MAP_ROW_LOBBY_SIZE; rows++)
	{
		for (s16 cols = 0; cols < MAP_COLUMN_LOBBY_SIZE; cols++)
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

	DrawNPC(*pWhiteSquareMesh);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(player->obj.img.pTex, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(player->obj.pos.x, player->obj.pos.y, 0.f, player->obj.img.scale.x, player->obj.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

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
	const char* pTextHP = str.c_str();
	f32 width, height;
	AEGfxGetPrintSize(pFont, pTextHP, 0.5f, &width, &height);
	AEGfxPrint(pFont, pTextHP, -width / 2 - 0.9f, -width / 2 + 0.97f, 0.5f, 1, 1, 1, 1);

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

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	switch (currScene)
	{
	case CurrentScene::PauseScene:
	{
		AEGfxTextureSet(PauseMenuBackground, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(pauseMenuBackground.pos.x, pauseMenuBackground.pos.x, 0.f, pauseMenuBackground.scale.x, pauseMenuBackground.pos.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		for (size_t i = 0; i < sizeof(PauseMenuButtons) / sizeof(PauseMenuButtons[0]); i++)
		{
			AEGfxTextureSet(PauseMenuButtons[i].pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(PauseMenuButtons[i].pos.x, PauseMenuButtons[i].pos.y, 0.f, PauseMenuButtons[i].scale.x, PauseMenuButtons[i].scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}

		const char* pText = "Resume";
		AEGfxGetPrintSize(pFont, pText, 0.5f, &width, &height);
		AEGfxPrint(pFont, pText, -width / 2, -height / 2 + 0.22f, 0.5f, 1, 1, 1, 1);

		const char* pText1 = "Back To Village";
		AEGfxGetPrintSize(pFont, pText1, 0.5f, &width, &height);
		AEGfxPrint(pFont, pText1, -width / 2, -height / 2, 0.5f, 1, 1, 1, 1);

		const char* pText2 = "Controls";
		AEGfxGetPrintSize(pFont, pText2, 0.5f, &width, &height);
		AEGfxPrint(pFont, pText2, -width / 2, -height / 2 - 0.22f, 0.5f, 1, 1, 1, 1);

		const char* pText3 = "Quit to Main menu";
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
		AEGfxPrint(pFont, pText, -width / 2 + 0.31f, -height / 2, 0.5f, 1, 1, 1, 1);
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
	ParticlesDraw(*pWhiteSquareMesh);

	if (AEInputCheckTriggered(AEVK_G))
	{
		transitionalImageOBJ.PlayMapTransition(TRANSITION_UP, GameStates::Area1); //Play the newly set animation here
	}

	MapTransitionDraw();
}

void GameLobby_Free()
{

	gameMap.clear();
	gameMap.resize(0);
	NPCPositions.clear();
	FreeNPC();

	AEGfxSetCamPosition(0.f, 0.f);
	ParticlesFree();
}

void GameLobby_Unload()
{
	Inventory::SaveInventory();

	AEGfxTextureUnload(background);
	AEGfxTextureUnload(HealthBorder);
	AEGfxTextureUnload(PauseMenuBackground);
	AEGfxTextureUnload(ButtonFrame);

	AEGfxTextureUnload(blank);
	AEGfxTextureUnload(Gear1);
	AEGfxTextureUnload(Gear2);
	AEGfxTextureUnload(weapon3);
	AEGfxTextureUnload(Gear4);
	AEGfxTextureUnload(Gear5);

	AEGfxTextureUnload(inventoryBackground.img.pTex);
	AEGfxTextureUnload(equipmentBackground.img.pTex);
	AEGfxTextureUnload(player->obj.img.pTex);


	AEGfxDestroyFont(pFont);

	AEGfxMeshFree(pMeshGrey);
	AEGfxMeshFree(pMeshYellow);
	AEGfxMeshFree(pMeshRed);
	AEGfxMeshFree(pLineMesh);
	AEGfxMeshFree(pMeshRedBar);
	AEGfxMeshFree(pWhiteSquareMesh);

	delete player;
	delete cam;
}