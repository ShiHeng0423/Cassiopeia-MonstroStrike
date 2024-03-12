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
	PauseMenu_Manager* menu;
#pragma region UserInterface
	//User Health
	AEGfxVertexList* pMeshRedBar;
	AEGfxTexture* HealthBorder;

#pragma endregion UserInterface

	bool inventory_open = false;
	int snapBack = -1;

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

void Lobby_Load()
{
	player = PlayerInitialize("Assets/Border.png", { 70.f,70.f }, { 0.f,-400.f }, { 40.f,0.f }, true);
	background = AEGfxTextureLoad("Assets/Background2.jpg");
	const char* fileName = "Assets/GameMap_Lobby.csv"; //Change name as per level

	//Load map
	if (MapLoader(fileName, gameMap, MAP_ROW_LOBBY_SIZE, MAP_COLUMN_LOBBY_SIZE))
	{
		PrintMap(gameMap, MAP_ROW_LOBBY_SIZE, MAP_COLUMN_LOBBY_SIZE); //Just for checking if the map data is stored properly
	}

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

	menu = new PauseMenu_Manager();
}

void Lobby_Initialize()
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
	menu->Init(cam);
	//Initialize NPCs
	InitializeNPC(NPCPositions);

	//looping thru to init all enemy variables

	ParticleInitialize();

}

void Lobby_Update()
{

	if (AEInputCheckTriggered(AEVK_9))
	{
		next = GameStates::AREA1;
	}

	menu->Update(cam);
	if (currScene == CurrentScene::PAUSE_SCENE || currScene == CurrentScene::CONTROL_SCENE || currScene == CurrentScene::QUIT_SCENE)
		return;
	if (currScene == CurrentScene::MAIN_SCENE)
		PlayerUpdate(*player, inventory_open);
	cam->UpdatePos(*player, grids2D[0][0].collisionBox.minimum.x, grids2D[0][MAP_COLUMN_LOBBY_SIZE - 1].collisionBox.maximum.x, grids2D[MAP_ROW_LOBBY_SIZE - 1][0].collisionBox.minimum.y, grids2D[0][0].collisionBox.maximum.y);


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
		}
	}

	if (AEInputCheckTriggered(AEVK_I))
	{
		inventory_open = !inventory_open;
	}

	if (inventory_open)
	{
		//update item position
		Inventory::UpdateInventory(playerInventory, inventoryButton);
		int index = 0;

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
			s32 testx = 0;
			s32 testy = 0;

			int index = 0;

			AEInputGetCursorPosition(&testx, &testy);
			AEVec2 mousePos;
			mousePos.x = testx - AEGfxGetWindowWidth() * 0.5f;
			mousePos.y = AEGfxGetWindowHeight() * 0.5f - testy;

			inventoryButton[snapBack].pos = mousePos;
		}

		if (AEInputCheckReleased(AEVK_LBUTTON))
		{
			int index = 0;
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

							std::cout << "swap\n";
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
			int index = 0;
			s32 testx = 0;
			s32 testy = 0;


			index = 0;
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

	UpdateNPC();
	ParticleUpdate();
	MapTransitionUpdate(player->obj.pos);
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

	//Inventory images
	if (inventory_open)
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
	}

	menu->Render();
	ParticlesDraw(*pWhiteSquareMesh);

	if (AEInputCheckTriggered(AEVK_G))
	{
		transitionalImageOBJ.PlayMapTransition(TRANSITION_UP, GameStates::AREA1); //Play the newly set animation here
	}

	MapTransitionDraw();
}

void Lobby_Free()
{

	gameMap.clear();
	gameMap.resize(0);
	NPCPositions.clear();
	FreeNPC();

	AEGfxSetCamPosition(0.f, 0.f);
	ParticlesFree();
}

void Lobby_Unload()
{
	Inventory::SaveInventory();

	AEGfxTextureUnload(background);
	AEGfxTextureUnload(HealthBorder);

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

	delete player->equippedWeapon;

	delete player;
	delete cam;
	delete menu;
}