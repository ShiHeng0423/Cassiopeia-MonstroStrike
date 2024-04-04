/*!************************************************************************
  \file					EnemyUtils.cpp
  \project name			Monstrostrike
  \primary author		Goh Jun Jie
  \secondary author

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/



#include "EnemyUtils.h"


AEGfxTexture* bulletTex;
AEGfxTexture* enemyJumperDropTex = nullptr;
AEGfxTexture* enemyChargerDropTex = nullptr;
AEGfxTexture* enemyFlyDropTex = nullptr;
AEGfxTexture* enemyBoss1DropTex = nullptr;


void MoveTowards(Enemy& enemy, AEVec2 targetPosition) {

	// Calculate the direction towards the target position
	float direction = (targetPosition.x > enemy.obj.pos.x) ? 1.0f : -1.0f;

	// Calculate the distance to move based on speed and elapsed time
	enemy.velocity.x = enemy.speed * (f32)AEFrameRateControllerGetFrameTime();

	// Update the position based on the calculated direction and distance
	enemy.obj.pos.x += direction * enemy.velocity.x;
}

void MoveTowardsFLY(Enemy& enemy, AEVec2 targetPosition) {

	f32 Offset = 50.f;	//lifts a bit above the ground

	// Calculate the direction towards the target position for both x and y axes
	float x_direction = (targetPosition.x > enemy.obj.pos.x) ? 1.0f : -1.0f;
	float y_direction = (targetPosition.y + Offset > enemy.obj.pos.y) ? 1.0f : -1.0f;

	// Calculate the distance to move based on speed and elapsed time
	float delta_time = (f32)AEFrameRateControllerGetFrameTime();
	if (enemy.enemyCurrent != ENEMY_TRANSITION) {
		enemy.velocity.x = enemy.speed * delta_time;
		enemy.obj.pos.x += x_direction * enemy.velocity.x;
	} 

	enemy.velocity.y = enemy.speed * delta_time;
	enemy.obj.pos.y += y_direction * enemy.velocity.y;
}



bool CanFire(Enemy& enemy) {

	if (enemy.timeSinceLastFire >= 1.0f / enemy.fireRate) {
		enemy.timeSinceLastFire = 0.0f; //reset
		return true;
	}
	return false;
}

bool CanPartFire(EnemyPart& part) {

	if (part.timeSinceLastFire >= 1.0f / part.fireRate) {
		part.timeSinceLastFire = 0.0f; //reset
		return true;
	}
	return false;
}

void SpawnBullet(AEVec2& enemyPosition, AEVec2& playerPosition, std::vector<Bullet>& vecbullets) {
	//dir vec from enemy to player
	AEVec2 direction;
	AEVec2Sub(&direction, &playerPosition, &enemyPosition); //player - enemy

	//normalize direction of bullet
	AEVec2Normalize(&direction, &direction);

	//create a bullet
	Bullet bullet;
	bullet.lifeTime = 100;													//lifetime
	bullet.obj.pTex = bulletTex;										//image
	AEVec2Set(&bullet.obj.pos, enemyPosition.x, enemyPosition.y);			//start position
	AEVec2Set(&bullet.obj.scale, 25.f, 25.f);							//set scale of the image

	//set velocity of bullet
	bullet.bulletSpeed = 2.5f;
	AEVec2Set(&bullet.bulletVel, direction.x * bullet.bulletSpeed, direction.y * bullet.bulletSpeed);

	bullet.collisionBox.minimum.x = bullet.obj.pos.x - bullet.obj.scale.x * 0.5f;
	bullet.collisionBox.minimum.y = bullet.obj.pos.y - bullet.obj.scale.y * 0.5f;
	bullet.collisionBox.maximum.x = bullet.obj.pos.x + bullet.obj.scale.x * 0.5f;
	bullet.collisionBox.maximum.y = bullet.obj.pos.y + bullet.obj.scale.y * 0.5f;

	// Push the bullet into the vector
	vecbullets.push_back(bullet);
}

void DrawBullets(Enemy& enemy, AEGfxVertexList* pWhiteSquareMesh) {
	for (const Bullet& bullet : enemy.bullets) {
		AEGfxTextureSet(bullet.obj.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(bullet.obj.pos.x, bullet.obj.pos.y, 0.f, bullet.obj.scale.x, bullet.obj.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void Attack_Charge(Enemy& enemy, s8 targetPosition, f32 speed) {
	enemy.speed = speed;
	enemy.velocity.x = enemy.speed * (f32)AEFrameRateControllerGetFrameTime();
	if (targetPosition == ENEMY_RIGHT) {
		enemy.velocity.x *= 1.0f;
	}
	if (targetPosition == ENEMY_LEFT) {
		enemy.velocity.x *= -1.0f;
	}
	enemy.obj.pos.x += enemy.velocity.x;

}
bool AreAligned(AEVec2 playerPosition, AEVec2 enemyPosition) {
	float tolerance = 25.f;
	return std::abs(playerPosition.y - enemyPosition.y) < tolerance;
}

void Jump(Enemy& enemy, f32 value) {
	enemy.onFloor = false;
	enemy.velocity.y = value;
}

void IsStuck(Enemy& enemy) {
	//Check distance travelled to see if enemy is stuck
	f32 distanceTravelled = AEVec2Distance(&enemy.lastPosition, &enemy.obj.pos);
	enemy.lastPosition = enemy.obj.pos;
	//std::cout << distanceTravelled << "\n";
	if (distanceTravelled < 0.2f) {
		if (enemy.stuckTimer < 0) { enemy.stuckTimer = enemy.timePassed; }	//if timer not started start it
		if (enemy.timePassed > enemy.stuckTimer + 1) {	//if enemy has been stuck for more than 1 sec
			enemy.isAttacking = false;
			enemy.enemyNext = ENEMY_IDLE;
		}
	}
}

bool ReachedPos(Enemy& enemy, AEVec2 wayPoint) {
	if ((enemy.obj.pos.x >= enemy.wayPoint.x - 1.0f) && (enemy.obj.pos.x <= enemy.wayPoint.x + 1.0f)) {
		return true;
	}
	else {
		return false;
	}
}

void EnemyLootSpawn(Enemy& enemy, std::vector<EnemyDrops>& vecCollectables) {
	
	EnemyDrops holder;
	AEVec2Set(&holder.obj.pos, enemy.obj.pos.x, enemy.obj.pos.y);			//start position
	AEVec2Set(&holder.obj.scale, 25.f, 25.f);							//set scale of the image

	switch (enemy.enemyType) {
	case ENEMY_JUMPER:
		holder.dropType = ENEMY_JUMPER_DROP;
		holder.obj.pTex = enemyJumperDropTex;
		break;
	case ENEMY_CHARGER:
		holder.dropType = ENEMY_CHARGER_DROP;
		holder.obj.pTex = enemyChargerDropTex;
		break;
	case ENEMY_FLY:
		holder.dropType = ENEMY_FLY_DROP;
		holder.obj.pTex = enemyFlyDropTex;
		break;
	case ENEMY_BOSS1:
		holder.dropType = ENEMY_BOSS1_DROP;
		holder.obj.pTex = enemyBoss1DropTex;
		break;
	}

	holder.collisionBox.minimum.x = holder.obj.pos.x - holder.obj.scale.x * 0.5f;
	holder.collisionBox.minimum.y = holder.obj.pos.y - holder.obj.scale.y * 0.5f;
	holder.collisionBox.maximum.x = holder.obj.pos.x + holder.obj.scale.x * 0.5f;
	holder.collisionBox.maximum.y = holder.obj.pos.y + holder.obj.scale.y * 0.5f;

	vecCollectables.push_back(holder);
}


void DrawEnemyLoot(std::vector<EnemyDrops>& vecCollectables, AEGfxVertexList* pWhiteSquareMesh) {
	for (const EnemyDrops& holder : vecCollectables) {
		AEGfxTextureSet(holder.obj.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(holder.obj.pos.x, holder.obj.pos.y, 0.f, holder.obj.scale.x, holder.obj.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void DrawEnemyHp(Enemy& enemy, AEGfxVertexList* pWhitesqrMesh) {
	//healthbar
// Calculate health bar position and size
	float healthBarWidth = 80.0f; //  width of health bar
	float healthBarHeight = 10.0f; //  height of health bar
	float healthBarX = enemy.obj.pos.x; // Center the health bar horizontally
	float healthBarY = enemy.obj.pos.y + 40.0f; // offset above the enemy

	// Calculate percentage of health remaining
	float healthPercentage = static_cast<float>(enemy.health) / static_cast<float>(enemy.maxHealth);

	// Calculate the width of the health bar based on the health percentage
	float remainingWidth = healthBarWidth * healthPercentage;

	float edgeX = healthBarX - (healthBarWidth - remainingWidth) / 2;

	// Draw health bar background
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(0, 0, 0);
	AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 1.0f); // Red color for background
	AEGfxSetTransform(ObjectTransformationMatrixSet(healthBarX, healthBarY, 0.f, healthBarWidth, healthBarHeight).m);
	AEGfxMeshDraw(pWhitesqrMesh, AE_GFX_MDM_TRIANGLES);

	// Draw health bar with remaining health
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(0, 0, 0);
	AEGfxSetColorToAdd(0.0f, 1.0f, 0.0f, 1.0f); // Green color for remaining health
	AEGfxSetTransform(ObjectTransformationMatrixSet(edgeX, healthBarY, 0.f, remainingWidth, healthBarHeight).m);
	AEGfxMeshDraw(pWhitesqrMesh, AE_GFX_MDM_TRIANGLES);


	//reset
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
}


void DrawEnemyHpParts(EnemyPart& enemy, AEGfxVertexList* pWhitesqrMesh) {
	//healthbar
// Calculate health bar position and size
	float healthBarWidth = 80.0f; //  width of health bar
	float healthBarHeight = 10.0f; //  height of health bar
	float healthBarX = enemy.obj.pos.x; // Center the health bar horizontally
	float healthBarY = enemy.obj.pos.y + 40.0f; // offset above the enemy

	// Calculate percentage of health remaining
	float healthPercentage = static_cast<float>(enemy.health) / static_cast<float>(enemy.maxHealth);

	// Calculate the width of the health bar based on the health percentage
	float remainingWidth = healthBarWidth * healthPercentage;

	float edgeX = healthBarX - (healthBarWidth - remainingWidth) / 2;

	// Draw health bar background
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(0, 0, 0);
	AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 1.0f); // Red color for background
	AEGfxSetTransform(ObjectTransformationMatrixSet(healthBarX, healthBarY, 0.f, healthBarWidth, healthBarHeight).m);
	AEGfxMeshDraw(pWhitesqrMesh, AE_GFX_MDM_TRIANGLES);

	// Draw health bar with remaining health
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(0, 0, 0);
	AEGfxSetColorToAdd(0.0f, 1.0f, 0.0f, 1.0f); // Green color for remaining health
	AEGfxSetTransform(ObjectTransformationMatrixSet(edgeX, healthBarY, 0.f, remainingWidth, healthBarHeight).m);
	AEGfxMeshDraw(pWhitesqrMesh, AE_GFX_MDM_TRIANGLES);


	//reset
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
}