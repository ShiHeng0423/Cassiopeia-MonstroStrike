#include "Enemy.h"
#include "Player.h"
#include "AEEngine.h"
#include "Physics.h"
#include "EnemyUtils.h"
#include "Inventory.h"


void Enemy_Load(s8 enemyType, std::vector<Enemy>& vecEnemy)
{
	Enemy enemy;

	switch (enemyType)
	{
	case ENEMY_JUMPER:
		enemy.obj.pTex = AEGfxTextureLoad("Assets/Enemy_Assets/Enemy_Jumper_Normal.png");
		enemy.angryTex = AEGfxTextureLoad("Assets/Enemy_Assets/Enemy_Jumper_Angry.png");
		break;
	case ENEMY_CHARGER:
		enemy.obj.pTex = AEGfxTextureLoad("Assets/Enemy_Assets/Enemy_Charger_Normal.png");
		enemy.angryTex = AEGfxTextureLoad("Assets/Enemy_Assets/Enemy_Charger_Angry.png");
		break;
	case ENEMY_FLY:
		enemy.obj.pTex = AEGfxTextureLoad("Assets/Enemy_Assets/Enemy_FLY_Normal.png");
		enemy.angryTex = AEGfxTextureLoad("Assets/Enemy_Assets/Enemy_FLY_Angry.png");
		break;
	case ENEMY_BOSS1:

		enemy.obj.pTex = AEGfxTextureLoad("Assets/Enemy_Assets/Enemy_Boss1_Normal.png");
		enemy.angryTex = AEGfxTextureLoad("Assets/Enemy_Assets/Enemy_Boss1_Angry.png");
		enemy.wing1.obj.pTex = AEGfxTextureLoad("Assets/Enemy_Assets/Enemy_wing_right.png");
		enemy.wing2.obj.pTex = AEGfxTextureLoad("Assets/Enemy_Assets/Enemy_wing_left.png");

		break;

	default:
		std::cerr << "Unknown enemy type: " << enemyType << std::endl;
		return;
	}

	enemy.enemyType = enemyType; //initialize enemy_type
	vecEnemy.push_back(enemy);
}

void FreeEnemy(std::vector<Enemy>& vecEnemy)
{
	for (Enemy& enemy : vecEnemy)
	{
		AEGfxTextureUnload(enemy.obj.pTex); //default tex
		AEGfxTextureUnload(enemy.angryTex); //angry tex

		if (enemy.enemyType == ENEMY_BOSS1)
		{
			//check if need to free wings
			AEGfxTextureUnload(enemy.wing1.obj.pTex);
			AEGfxTextureUnload(enemy.wing2.obj.pTex);
		}
	}
}


void Enemy_Init(AEVec2 scale, AEVec2 location, s8 startingState, Enemy& enemy)
{
	UNREFERENCED_PARAMETER(startingState);

	//Initializing Enemy struct variables
	AEVec2Set(&enemy.obj.pos, location.x, location.y); //set starting location
	AEVec2Set(&enemy.obj.scale, scale.x, scale.y); //set scale of the image

	enemy.startingPosition = location;
	enemy.lastPosition = location;
	enemy.wayPoint.x = location.x + 200.f;
	enemy.wayPoint.y = 0;
	enemy.targetPosition = ENEMY_DEFAULT;
	enemy.loopIdle = true;

	enemy.enemyCurrent = ENEMY_IDLE;
	enemy.enemyNext = ENEMY_IDLE;

	enemy.attackState = ENEMY_ATTACK_DEFAULT;

	enemy.isAlive = true;
	enemy.isAttacking = false;
	enemy.isCollision = false;
	enemy.isFlying = false;
	enemy.isCollidedWithPlayer = false;
	enemy.isRecoil = false;
	enemy.hasDealtDmg = false;

	enemy.isVisible = true;

	enemy.timePassed = 0.f;

	enemy.stuckTimer = -1.f;
	//AABB Box init, Collision boxes
	enemy.onFloor = true;
	enemy.isFalling = false;

	AEVec2Set(&enemy.boxArms.maximum, 0.f, 0.f);
	AEVec2Set(&enemy.boxHeadFeet.maximum, 0.f, 0.f);
	AEVec2Set(&enemy.collisionNormal, 0.f, 0.f);

	AEVec2Set(&enemy.spawnPoint, 0.f, 0.f);

	enemy.gravityForce = 0.f;

	//(Offsets)
	f32 enemyWingScaleOffset = 30.f;
	f32 enemyWingPosOffset = 70.f;

	switch (enemy.enemyType)
	{
	case ENEMY_JUMPER:
		enemy.speed = 50.f;
		enemy.lineOfSight = 500.f;
		enemy.shootingRange = 250.f;
		enemy.fireRate = 1.0f;
		enemy.timeSinceLastFire = 0;
		enemy.maxHealth = 50;
		enemy.health = enemy.maxHealth;
		enemy.mass = 40.f;
		AEVec2Set(&enemy.velocity, 0.f, 0.f); //Begin with no velocity
		break;
	case ENEMY_CHARGER:
		enemy.speed = 80.f;
		enemy.lineOfSight = 300.f;
		enemy.shootingRange = 300.f;
		enemy.fireRate = 5.0f;
		enemy.timeSinceLastFire = 0;
		enemy.maxHealth = 100;
		enemy.health = enemy.maxHealth;
		enemy.mass = 100.f;
		AEVec2Set(&enemy.velocity, 0.f, 0.f); //Begin with no velocity
		break;
	case ENEMY_FLY:
		enemy.isFlying = true;
		enemy.speed = 80.f;
		enemy.lineOfSight = 400.f;
		enemy.shootingRange = 250.f;
		enemy.fireRate = 1.0f;
		enemy.timeSinceLastFire = 0;
		enemy.maxHealth = 30;
		enemy.health = enemy.maxHealth;
		enemy.mass = 100.f;
		AEVec2Set(&enemy.velocity, 0.f, 0.f); //Begin with no velocity
		break;
	case ENEMY_BOSS1:
		//main body
		enemy.attackState = ENEMY_ATTACK_CHOOSING;
		enemy.isFlying = true;
		enemy.speed = 80.f;
		enemy.lineOfSight = 1000.f;
		enemy.shootingRange = 500.f;
		enemy.fireRate = 1.0f;
		enemy.timeSinceLastFire = 0;
		enemy.maxHealth = 500;
		enemy.health = enemy.maxHealth;
		enemy.mass = 150.f;
		AEVec2Set(&enemy.velocity, 0.f, 0.f); //Begin with no velocity

	//wing1
		enemy.wing1.isAlive = false;
		enemy.wing1.Offset = enemyWingPosOffset;
		AEVec2Set(&enemy.wing1.obj.pos, location.x + enemy.wing1.Offset, location.y); //set starting location
		AEVec2Set(&enemy.wing1.obj.scale, scale.x + enemyWingScaleOffset, scale.y + enemyWingScaleOffset);
	//set scale of the image
		enemy.wing1.collisionBox.minimum.x = enemy.wing1.obj.pos.x - enemy.wing1.obj.scale.x * 0.5f;
		enemy.wing1.collisionBox.minimum.y = enemy.wing1.obj.pos.y - enemy.wing1.obj.scale.y * 0.5f;
		enemy.wing1.collisionBox.maximum.x = enemy.wing1.obj.pos.x + enemy.wing1.obj.scale.x * 0.5f;
		enemy.wing1.collisionBox.maximum.y = enemy.wing1.obj.pos.y + enemy.wing1.obj.scale.y * 0.5f;
		enemy.wing1.fireRate = 1.0f;
		enemy.wing1.timeSinceLastFire = 0;
		enemy.wing1.health = 200;
		enemy.wing1.maxHealth = enemy.wing1.health;
	//wing2
		enemy.wing2.isAlive = false;
		enemy.wing2.Offset = -enemyWingPosOffset;
		AEVec2Set(&enemy.wing2.obj.pos, location.x + enemy.wing2.Offset, location.y); //set starting location
		AEVec2Set(&enemy.wing2.obj.scale, scale.x + enemyWingScaleOffset, scale.y + enemyWingScaleOffset);
	//set scale of the image
		enemy.wing2.collisionBox.minimum.x = enemy.wing2.obj.pos.x - enemy.wing2.obj.scale.x * 0.5f;
		enemy.wing2.collisionBox.minimum.y = enemy.wing2.obj.pos.y - enemy.wing2.obj.scale.y * 0.5f;
		enemy.wing2.collisionBox.maximum.x = enemy.wing2.obj.pos.x + enemy.wing2.obj.scale.x * 0.5f;
		enemy.wing2.collisionBox.maximum.y = enemy.wing2.obj.pos.y + enemy.wing2.obj.scale.y * 0.5f;
		enemy.wing2.fireRate = 1.0f;
		enemy.wing2.timeSinceLastFire = 0;
		enemy.wing2.health = 200;
		enemy.wing2.maxHealth = enemy.wing2.health;
		break;
	default:
		return;
	}
}


void EnemyUpdateChoose(Enemy& enemy, class Player& player, std::vector<EnemyDrops>& vecCollectables)
{
	//(update bullet)---------------------------------------------------------------------------------------
	for (auto it = enemy.bullets.begin(); it != enemy.bullets.end();)
	{
		it->obj.pos.x += it->bulletVel.x * (f32)AEFrameRateControllerGetFrameTime() * 100.f;
		it->obj.pos.y += it->bulletVel.y * (f32)AEFrameRateControllerGetFrameTime() * 100.f;

		it->collisionBox.minimum.x = it->obj.pos.x - it->obj.scale.x * 0.5f;
		it->collisionBox.minimum.y = it->obj.pos.y - it->obj.scale.y * 0.5f;
		it->collisionBox.maximum.x = it->obj.pos.x + it->obj.scale.x * 0.5f;
		it->collisionBox.maximum.y = it->obj.pos.y + it->obj.scale.y * 0.5f;

		if (AABBvsAABB(it->collisionBox, player.GetPlayerCollisionBox()))
		{
			it = enemy.bullets.erase(it);
			player.GetCurrentHealth() -= 5;
			continue;
		}
		it->lifeTime--; //decrease lifetime
		if (it->lifeTime <= 0)
		{
			it = enemy.bullets.erase(it);
			continue;
		}
		else
		{
			++it;
		}
	}
	//(update bullet)---------------------------------------------------------------------------------------


	enemy.timeSinceLastFire += (f32)AEFrameRateControllerGetFrameTime();
	enemy.wing1.timeSinceLastFire += (f32)AEFrameRateControllerGetFrameTime();
	enemy.wing2.timeSinceLastFire += (f32)AEFrameRateControllerGetFrameTime();

	if (!enemy.isFlying)
	{
		ApplyGravity(&enemy.velocity, enemy.mass, &enemy.onFloor, &enemy.gravityForce, &enemy.isFalling);
	}

	switch (enemy.enemyType)
	{
	case ENEMY_JUMPER:
		ENEMY_JUMPER_Update(enemy, player, vecCollectables);
		break;
	case ENEMY_CHARGER:
		ENEMY_CHARGER_Update(enemy, player, vecCollectables);
		break;
	case ENEMY_FLY:
		ENEMY_FLY_Update(enemy, player, vecCollectables);
		break;
	case ENEMY_BOSS1:
		ENEMY_BOSS_Update(enemy, player, vecCollectables);
		break;
	}


	//main body collision box
	enemy.collisionBox.minimum.x = enemy.obj.pos.x - enemy.obj.scale.x * 0.5f;
	enemy.collisionBox.minimum.y = enemy.obj.pos.y - enemy.obj.scale.y * 0.5f;
	enemy.collisionBox.maximum.x = enemy.obj.pos.x + enemy.obj.scale.x * 0.5f;
	enemy.collisionBox.maximum.y = enemy.obj.pos.y + enemy.obj.scale.y * 0.5f;

	f32 verticalOffset = enemy.obj.scale.y * 0.05f;
	//Vertical
	enemy.boxHeadFeet = enemy.collisionBox; // Get original collision box size
	enemy.boxHeadFeet.minimum.y -= verticalOffset;
	enemy.boxHeadFeet.maximum.y += verticalOffset;

	f32 horizontalOffset = enemy.obj.scale.x * 0.02f;
	//Horizontal
	enemy.boxArms = enemy.collisionBox;
	enemy.boxArms.minimum.x -= horizontalOffset;
	enemy.boxArms.maximum.x += horizontalOffset;
}

void AllEnemyUpdate(std::vector<Enemy>& vecEnemyVar, class Player& player, std::vector<EnemyDrops>& vecCollectables)
{
	//(update drops)---------------------------------------------------------------------------------------
	auto it = vecCollectables.begin();

	// Iterate over the vector
	while (it != vecCollectables.end())
	{
		// Check collision
		if (AABBvsAABB(it->collisionBox, player.GetPlayerCollisionBox()))
		{
			//addit to inventory
			//here
			Item holder;
			switch (it->dropType)
			{
			case ENEMY_JUMPER_DROP:
				holder = fullInventoryList[20];
				break;
			case ENEMY_CHARGER_DROP:
				holder = fullInventoryList[16];
				std::cout << "Charger item collected\n";
				break;
			case ENEMY_FLY_DROP:
				holder = fullInventoryList[19];
				std::cout << "CHANGE item collected\n";
				break;
			case ENEMY_BOSS1_DROP:
				holder = fullInventoryList[17];
				std::cout << "CHANGE item collected\n";
				break;
			}

			Inventory::AddItem(holder);

			// Erase the element
			it = vecCollectables.erase(it);
		}
		else
		{
			// Move to the next element
			++it;
		}
	}
	//(update drops)---------------------------------------------------------------------------------------

	for (Enemy& enemy : vecEnemyVar)
	{
		if (enemy.isAlive)
		{
			EnemyUpdateChoose(enemy, player, vecCollectables);
		}
	}
}

void AllEnemyNBulletCollisionCheck(std::vector<Enemy>& vecEnemyVar, AABB gridBoxAABB)
{
	for (Enemy& enemy : vecEnemyVar)
	{
		//Check vertical box (Head + Feet) 
		if (AABBvsAABB(enemy.boxHeadFeet, gridBoxAABB))
		{
			enemy.collisionNormal = AABBNormalize(enemy.boxHeadFeet, gridBoxAABB);

			ResolveVerticalCollision(enemy.boxHeadFeet, gridBoxAABB,
			                         &enemy.collisionNormal, &enemy.obj.pos,
			                         &enemy.velocity, &enemy.onFloor, &enemy.gravityForce,
			                         &enemy.isFalling);
		}
		//Check horizontal box (Left arm -> Right arm)
		if (AABBvsAABB(enemy.boxArms, gridBoxAABB))
		{
			enemy.isCollision = true;
			enemy.collisionNormal = AABBNormalize(enemy.boxArms, gridBoxAABB);

			ResolveHorizontalCollision(enemy.boxArms, gridBoxAABB, &enemy.collisionNormal, &enemy.obj.pos,
			                           &enemy.velocity);
			enemy.loopIdle = false;
		}

		if (enemy.enemyType == ENEMY_FLY || enemy.enemyType == ENEMY_BOSS1)
		{
			for (Bullet& bullet : enemy.bullets)
			{
				if (AABBvsAABB(bullet.collisionBox, gridBoxAABB))
				{
					bullet.lifeTime = 0; //makes bullet erase
				}
			}
		}
	}
}

void AllEnemyDraw(std::vector<Enemy>& vecEnemyVar, AEGfxVertexList* pWhitesqrMesh,
                  std::vector<EnemyDrops>& vecCollectables)
{
	for (Enemy& enemy : vecEnemyVar)
	{
		if (enemy.isAlive)
		{
			if (enemy.isAttacking)
			{
				AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxTextureSet(enemy.angryTex, 0, 0);
				enemy.obj.UpdateTransformMatrix();
				AEGfxSetTransform(enemy.obj.transform.m);
				AEGfxMeshDraw(pWhitesqrMesh, AE_GFX_MDM_TRIANGLES);

				DrawBullets(enemy, pWhitesqrMesh); //drawing bullets
			}
			else
			{
				AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxTextureSet(enemy.obj.pTex, 0, 0);
				enemy.obj.UpdateTransformMatrix();
				AEGfxSetTransform(enemy.obj.transform.m);
				AEGfxMeshDraw(pWhitesqrMesh, AE_GFX_MDM_TRIANGLES);

				DrawBullets(enemy, pWhitesqrMesh); //drawing bullets
			}


			if (enemy.enemyType == ENEMY_BOSS1 && enemy.wing1.isAlive)
			{
				AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxTextureSet(enemy.wing1.obj.pTex, 0, 0);
				enemy.wing1.obj.UpdateTransformMatrix();
				AEGfxSetTransform(enemy.wing1.obj.transform.m);
				AEGfxMeshDraw(pWhitesqrMesh, AE_GFX_MDM_TRIANGLES);
				DrawEnemyHpParts(enemy.wing1, pWhitesqrMesh);
			}
			if (enemy.enemyType == ENEMY_BOSS1 && enemy.wing2.isAlive)
			{
				AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxTextureSet(enemy.wing2.obj.pTex, 0, 0);
				enemy.wing2.obj.UpdateTransformMatrix();
				AEGfxSetTransform(enemy.wing2.obj.transform.m);
				AEGfxMeshDraw(pWhitesqrMesh, AE_GFX_MDM_TRIANGLES);
				DrawEnemyHpParts(enemy.wing2, pWhitesqrMesh);
			}

			//healthbar
			DrawEnemyHp(enemy, pWhitesqrMesh);
		}
	}

	DrawEnemyLoot(vecCollectables, pWhitesqrMesh);
}
