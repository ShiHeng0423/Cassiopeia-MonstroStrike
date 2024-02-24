#include "Level1.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "TransformMatrix.h"
#include "Player.h"
#include "Enemy.h"
#include <iostream>

#include "CSVMapLoader.h"
#include "GridTypesList.h"
#include "Physics.h"
#include "CollisionShape.h" //For Vertical + Horizontal collision
#include <string>

//Temporary moving platform object
struct MovingPlatform
{
	AEVec2 velocity; //Added for movement - Johny
	AEVec2 collisionNormal;
	AEVec2 size;
	AEVec2 position;

	AEVec2 endPoint;
	AEVec2 startPoint;

	AABB collisionBox;

	AEMtx33 scale;
	AEMtx33 rotation;
	AEMtx33 translation;
	AEMtx33 transformation;

	GRID_TYPES gridType;
};

namespace {
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

	s8 pFont;

	AEGfxVertexList* pWhiteSquareMesh;
	AEGfxVertexList* pLineMesh;
	AEGfxVertexList* pMeshYellow;
	AEGfxVertexList* pMeshRed;
	AEGfxVertexList* pMeshGrey;

	Grids2D grids2D[MAP_ROW_SIZE][MAP_COLUMN_SIZE]; //Initializing map
	std::vector<std::vector<MapCell>> gameMap(MAP_ROW_SIZE, std::vector<MapCell>(MAP_COLUMN_SIZE)); //Map for this level

	MovingPlatform movingObject;

	const int size = 5;
	Enemy* enemy[size];
	Player* player;

	AEGfxTexture* background;

	bool inventory_open = false;

	int hp = 100;
	int gear_equipped = 0;

	float burningEffectDuration = 1.5f;
	float timer = 0.f;
}


//temporary code section
void Equip(int index, ButtonGearUI tmp)
{
	equipmentDisplay[index].img.pTex = tmp.img.pTex;
	if (!tmp.isWeapon)
	{
		gear_equipped++;
		if (gear_equipped == 4)
			hp *= 1.5;
	}
	else
	{
		player->burningEffect = true;
	}
}

void Level1_Load()
{

	enemy[0] = ENEMY_Init({80.f,80.f}, {500.f,-150.f}, ENEMY_JUMPER, ENEMY_IDLE);
	enemy[1] = ENEMY_Init({80.f,80.f }, {-500.f,-150.f}, ENEMY_FLY, ENEMY_IDLE);
	enemy[2] = ENEMY_Init({80.f,80.f }, { -500.f,150.f }, ENEMY_BOSS1_WING1, ENEMY_IDLE);
	enemy[3] = ENEMY_Init({80.f,80.f }, { -500.f,150.f }, ENEMY_BOSS1_WING2, ENEMY_IDLE);
	enemy[4] = ENEMY_Init({ 80.f,80.f }, { -500.f,150.f }, ENEMY_BOSS1, ENEMY_IDLE);

	player = PlayerInitialize("Assets/Kronii_Pixel.png", { 70.f,70.f }, { -750.f,-155.f }, { 10.f,0.f }, true);
	background = AEGfxTextureLoad("Assets/Background2.jpg");
	const char* fileName = "Assets/GameMap.csv"; //Change name as per level
	//Load map
	if (MapLoader(fileName, gameMap, MAP_ROW_SIZE, MAP_COLUMN_SIZE))
	{
		PrintMap(gameMap, MAP_ROW_SIZE, MAP_COLUMN_SIZE); //Just for checking if the map data is stored properly
	}
	AEGfxMeshStart();

	pFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 72);

	inventoryBackground.img.pTex = AEGfxTextureLoad("Assets/panel_brown.png");
	AEVec2Set(&inventoryBackground.img.scale, 500.f, 500.f);

	int index = 0;
	Gear1 = AEGfxTextureLoad("Assets/item_.png");
	Gear2 = AEGfxTextureLoad("Assets/item_helmet.png");
	weapon3 = AEGfxTextureLoad("Assets/item_sword.png");
	Gear4 = AEGfxTextureLoad("Assets/tile_archColumns.png");
	Gear5 = AEGfxTextureLoad("Assets/tile_archHalf.png");
	blank = AEGfxTextureLoad("Assets/panelInset_beige.png");
	for (ButtonGearUI&button : inventoryButton)
	{
		button.img.pTex = blank;
		AEVec2Set(&button.img.scale, 60.f, 60.f);
		AEVec2Set(&button.pos, (index % 5) * 90 - 180, -(index / 5) * 90 +180);
		button.isWeapon = false;
		index++;
	}
	inventoryButton[0].img.pTex = Gear1;
	inventoryButton[1].img.pTex = Gear2;
	inventoryButton[2].img.pTex = weapon3;
	inventoryButton[2].isWeapon = true;
	inventoryButton[3].img.pTex = Gear4;
	inventoryButton[4].img.pTex = Gear5;

	equipmentBackground.img.pTex = AEGfxTextureLoad("Assets/panel_brown.png");
	AEVec2Set(&equipmentBackground.img.scale, 250.f, 500.f);
	AEVec2Set(&equipmentBackground.pos, -375.f, 0.f);

	index = 0;
	for (ButtonGearUI& button : equipmentDisplay)
	{
		button.img.pTex = blank;
		AEVec2Set(&button.img.scale, 60.f, 60.f);
		AEVec2Set(&button.pos, -375.f, -index * 90 + 180);
		index++;
	}
#pragma region Mesh Creations
	// *Color follows ARGB format (Alpha, Red, Green, Blue)
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFa9a9a9, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, 0xFFa9a9a9, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, 0xFFa9a9a9, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFa9a9a9, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, 0xFFa9a9a9, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, 0xFFa9a9a9, 0.0f, 0.0f);  // top-left: blue

	pMeshGrey = AEGfxMeshEnd();

	// *Color follows ARGB format (Alpha, Red, Green, Blue)
	//Red Mesh
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, 0xFFFF0000, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFF0000, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, 0xFFFF0000, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);  // top-left: blue

	 pMeshRed = AEGfxMeshEnd();

	// *Color follows ARGB format (Alpha, Red, Green, Blue)
//Yellow Mesh
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, 0xFFFFFF00, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f);  // top-left: blue

	pMeshYellow = AEGfxMeshEnd();

	//Drawing square
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh
	pWhiteSquareMesh = AEGfxMeshEnd();

	//Drawing line
	AEGfxMeshStart();
	AEGfxVertexAdd(-0.5f, 0.f, 0xFF000000, 1.0f, 1.0f);
	AEGfxVertexAdd(0.5f, 0.f, 0xFF000000, 1.0f, 1.0f);
	pLineMesh = AEGfxMeshEnd();
#pragma endregion

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


	//Moving platform test, success
	AEVec2Zero(&movingObject.collisionNormal);
	
	movingObject.rotation = { 0 };
	AEMtx33Rot(&movingObject.rotation, 0.f);

	AEVec2Set(&movingObject.size, 140.f, 50.f);
	AEMtx33Scale(&movingObject.scale, movingObject.size.x, movingObject.size.y);
	
	AEVec2Set(&movingObject.position, 1200, -340.f);
	AEMtx33Trans(&movingObject.translation, movingObject.position.x, movingObject.position.y);

	movingObject.transformation = { 0 };
	AEMtx33Concat(&movingObject.transformation, &movingObject.rotation, &movingObject.scale);
	AEMtx33Concat(&movingObject.transformation, &movingObject.translation, &movingObject.transformation);

	movingObject.gridType = NORMAL_GROUND;

	movingObject.startPoint = movingObject.position;
	movingObject.endPoint.x = movingObject.startPoint.x + 300.f;
	movingObject.endPoint.y = movingObject.position.y;

	AEVec2Set(&movingObject.velocity, movingObject.endPoint.x - movingObject.startPoint.x, movingObject.endPoint.y - movingObject.startPoint.y);
	AEVec2Normalize(&movingObject.velocity, &movingObject.velocity);
	float speed = 5.0f; // Adjust as needed
	AEVec2Scale(&movingObject.velocity, &movingObject.velocity, speed);
}

void Level1_Update()
{

	PlayerUpdate(*player);
	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		player->isAttacking = true;
	}
	//std::cout << player->obj.pos.x << std::endl;
	//std::cout << player->obj.pos.y << std::endl;

	//This is set here temporary so that thing actually work, need to move
	if (player->isAttacking)
	{
		for (int i = 0; i < size; i++)
		{	
			if (enemy[i]->isAlive) {
				CheckWeaponCollision(&player->equippedWeapon, *enemy[i], *player);
			}
		}
	}

	for (int i = 0; i < size; ++i) {
		if (enemy[i]->isAlive) {
			Enemy_Update_Choose(*enemy[i], *player);
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
			s32 x, y;
			AEInputGetCursorPosition(&x, &y);
			AEVec2 mousePos;
			mousePos.x = x - AEGfxGetWindowWidth() * 0.5f;
			mousePos.y = AEGfxGetWindowHeight() * 0.5f - y;

			int index = 0;
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

	if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
	{
		next = GameStates::Quit;
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
					 std::cout << "Normal X at Horizontal:" << player->collisionNormal.x << std::endl;
					 std::cout << "Normal Y at Horizontal:" << player->collisionNormal.y << std::endl;

					 ResolveVerticalCollision(player->boxHeadFeet, grids2D[rows][cols].collisionBox, &player->collisionNormal, &player->obj.pos,
						 &player->velocity, &player->onFloor);
				}

				//Check horizontal box (Left arm -> Right arm)
				if (AABBvsAABB(player->boxArms, grids2D[rows][cols].collisionBox)) {
					player->collisionNormal = AABBNormalize(player->boxArms, grids2D[rows][cols].collisionBox);
					 std::cout << "Normal X at Horizontal:" << player->collisionNormal.x << std::endl;
					 std::cout << "Normal Y at Horizontal:" << player->collisionNormal.y << std::endl;
					ResolveHorizontalCollision(player->boxArms, grids2D[rows][cols].collisionBox, &player->collisionNormal, &player->obj.pos,
						&player->velocity, &player->onFloor);
				}
				break;
			case EMPTY:
				break;
			}
		}
	}

	//Burning effect
	if (player->burningEffect)
	{
		if (timer >= burningEffectDuration)
		{
			AEVec2 hpLerp;
			hpLerp.x = hp;
			hpLerp.y = 0.f;
			AEVec2 end;
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

	// Update platform position based on velocity
	//movingObject.velocity.x += 50.f * static_cast<float>(AEFrameRateControllerGetFrameTime());
	AEVec2Add(&movingObject.position, &movingObject.position, &movingObject.velocity);


	AEMtx33Trans(&movingObject.translation, movingObject.position.x, movingObject.position.y);
	AEMtx33Concat(&movingObject.transformation, &movingObject.rotation, &movingObject.scale);
	AEMtx33Concat(&movingObject.transformation, &movingObject.translation, &movingObject.transformation);

	if (AEVec2Distance(&movingObject.position, &movingObject.endPoint) < 1.0f) {
		// Swap start and end points
		AEVec2 temp = movingObject.startPoint;
		movingObject.startPoint = movingObject.endPoint;
		movingObject.endPoint = temp;
		// Recalculate velocity direction
		AEVec2Set(&movingObject.velocity, movingObject.endPoint.x - movingObject.startPoint.x, movingObject.endPoint.y - movingObject.startPoint.y);
		AEVec2Normalize(&movingObject.velocity, &movingObject.velocity);
		AEVec2Scale(&movingObject.velocity, &movingObject.velocity, 5.f);
	}

	//Check horizontal box (Left arm -> Right arm)
	if (AABBvsAABB(player->boxArms, movingObject.collisionBox)) {
		player->collisionNormal = AABBNormalize(player->boxArms, movingObject.collisionBox);



		ResolveHorizontalCollision(player->boxArms, movingObject.collisionBox, &player->collisionNormal, &player->obj.pos,
			&player->velocity, &player->onFloor);
	}

	//Vertical
	if (AABBvsAABB(player->boxHeadFeet, movingObject.collisionBox)) {
		player->collisionNormal = AABBNormalize(player->boxHeadFeet, movingObject.collisionBox);

		ResolveVerticalCollision(player->boxHeadFeet, movingObject.collisionBox, &player->collisionNormal, &player->obj.pos,
			&player->velocity, &player->onFloor);

		player->obj.pos.x += movingObject.velocity.x;
		player->obj.pos.y += movingObject.velocity.y;
	}


	movingObject.collisionBox.minimum.x = movingObject.position.x - movingObject.size.x * 0.5f;
	movingObject.collisionBox.minimum.y = movingObject.position.y - movingObject.size.y * 0.5f;
	movingObject.collisionBox.maximum.x = movingObject.position.x + movingObject.size.x * 0.5f;
	movingObject.collisionBox.maximum.y = movingObject.position.y + movingObject.size.y * 0.5f;

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

	AEGfxSetTransform(movingObject.transformation.m);
	AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(player->obj.img.pTex, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(player->obj.pos.x, player->obj.pos.y, 0.f, player->obj.img.scale.x, player->obj.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	for (int i = 0; i < size; ++i) {
		if (enemy[i]->isAlive) {
			if (enemy[i]->isShooting) {
				AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 0.0f);//this line makes enemy go red when shooting
				AEGfxTextureSet(enemy[i]->obj.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(enemy[i]->obj.pos.x, enemy[i]->obj.pos.y, 0.f, enemy[i]->obj.img.scale.x, enemy[i]->obj.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}
			else {
				AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
				AEGfxTextureSet(enemy[i]->obj.img.pTex, 0, 0);
				AEGfxSetTransform(ObjectTransformationMatrixSet(enemy[i]->obj.pos.x, enemy[i]->obj.pos.y, 0.f, enemy[i]->obj.img.scale.x, enemy[i]->obj.img.scale.y).m);
				AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
			}
		}
		
	}

	AEVec2 cam;
	AEGfxGetCamPosition(&cam.x, &cam.y);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransform(ObjectTransformationMatrixSet(cam.x, cam.y, 0.f, AEGfxGetWindowWidth(), 1.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(ObjectTransformationMatrixSet(cam.x, cam.y, 0.5f * PI, AEGfxGetWindowWidth(), 1.f).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(ObjectTransformationMatrixSet(-800 + hp + cam.x, 450 + cam.y, 0, hp * 2, 80.f).m);
	AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);

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
		AEGfxSetTransform(ObjectTransformationMatrixSet(cam.x, cam.y, 0.f, inventoryBackground.img.scale.x, inventoryBackground.img.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		for (ButtonGearUI button : inventoryButton)
		{
			AEGfxTextureSet(button.img.pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + cam.x, button.pos.y + cam.y, 0.f, button.img.scale.x, button.img.scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}

		AEGfxTextureSet(equipmentBackground.img.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(equipmentBackground.pos.x + cam.x, equipmentBackground.pos.y + cam.y, 0.f, equipmentBackground.img.scale.x, equipmentBackground.img.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		for (ButtonGearUI button : equipmentDisplay)
		{
			AEGfxTextureSet(button.img.pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(button.pos.x + cam.x, button.pos.y + cam.y, 0.f, button.img.scale.x, button.img.scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}
	}

	//Player attack
	if (player->isAttacking)
	{
		AEGfxSetTransform(ObjectTransformationMatrixSet(player->equippedWeapon.position.x, player->equippedWeapon.position.y, 0.f, player->equippedWeapon.Scale.x, player->equippedWeapon.Scale.y).m);
		AEGfxMeshDraw(pMeshRed, AE_GFX_MDM_TRIANGLES);
	}
}

void Level1_Free()
{
	for (int i = 0; i < size; ++i) {
		Enemy_Free(enemy[i]);
	}

	//Free vectors
	gameMap.clear();
	gameMap.resize(0);

	//Free meshes
	AEGfxMeshFree(pMeshGrey);
	AEGfxMeshFree(pMeshYellow);
	AEGfxMeshFree(pMeshRed);
	AEGfxMeshFree(pLineMesh);
	AEGfxMeshFree(pWhiteSquareMesh);

}

void Level1_Unload()
{

}
