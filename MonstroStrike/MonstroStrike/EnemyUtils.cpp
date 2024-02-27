#include "Enemy.h"
#include "Player.h"
#include "AEEngine.h"
#include "EnemyUtils.h"
#include "TransformMatrix.h"
#include <iostream>
#include <cmath> // For abs function



void MoveTowards(Enemy& enemy, AEVec2 target_position) {

	//enemy.velocity.x = enemy.speed * AEFrameRateControllerGetFrameTime();

	//if ((enemy.obj.pos.x != target_position.x)) {
	//	if (enemy.obj.pos.x >= target_position.x) {
	//		enemy.velocity.x *= -1.0f;
	//	}
	//	if (enemy.obj.pos.x <= target_position.x) {
	//		enemy.velocity.x *= 1.0f;

	//	}
	//}
	//enemy.obj.pos.x += enemy.velocity.x;




	// Calculate the direction towards the target position
	float direction = (target_position.x > enemy.obj.pos.x) ? 1.0f : -1.0f;

	// Calculate the distance to move based on speed and elapsed time
	enemy.velocity.x = enemy.speed * (f32)AEFrameRateControllerGetFrameTime();

	// Update the position based on the calculated direction and distance
	enemy.obj.pos.x += direction * enemy.velocity.x;
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


void SpawnBullet(AEVec2& enemy_position, AEVec2& player_position, std::vector<Bullet>& vecbullets) {
	//dir vec from enemy to player
	AEVec2 direction;
	AEVec2Sub(&direction, &player_position, &enemy_position); //player - enemy

	//normalize direction of bullet
	AEVec2Normalize(&direction, &direction);

	//create a bullet
	Bullet bullet;
	bullet.lifetime = 50;													//lifetime
	bullet.obj.img.pTex = bulletTex;										//image
	AEVec2Set(&bullet.obj.pos, enemy_position.x, enemy_position.y);			//start position
	AEVec2Set(&bullet.obj.img.scale, 80.f, 80.f);							//set scale of the image

	//set velocity of bullet
	bullet.bulletSpeed = 5.f;
	AEVec2Set(&bullet.bulletVel, direction.x * bullet.bulletSpeed, direction.y * bullet.bulletSpeed);

	bullet.collisionBox.minimum.x = bullet.obj.pos.x - bullet.obj.img.scale.x * 0.5f;
	bullet.collisionBox.minimum.y = bullet.obj.pos.y - bullet.obj.img.scale.y * 0.5f;
	bullet.collisionBox.maximum.x = bullet.obj.pos.x + bullet.obj.img.scale.x * 0.5f;
	bullet.collisionBox.maximum.y = bullet.obj.pos.y + bullet.obj.img.scale.y * 0.5f;

	// Push the bullet into the vector
	vecbullets.push_back(bullet);


}

void DrawBullets(Enemy& enemy, AEGfxVertexList* pWhiteSquareMesh) {
	for (const Bullet& bullet : enemy.bullets) {
		AEGfxTextureSet(bullet.obj.img.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(bullet.obj.pos.x, bullet.obj.pos.y, 0.f, bullet.obj.img.scale.x, bullet.obj.img.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	}
	if (enemy.enemyType == ENEMY_BOSS1 && enemy.wing1.isAlive) {
		AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 0.0f);

		AEGfxTextureSet(enemy.wing1.obj.img.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(enemy.wing1.obj.pos.x, enemy.wing1.obj.pos.y, 0.f, enemy.wing1.obj.img.scale.x, enemy.wing1.obj.img.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	}
	if (enemy.enemyType == ENEMY_BOSS1 && enemy.wing2.isAlive) {
		AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 0.0f);

		AEGfxTextureSet(enemy.wing2.obj.img.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(enemy.wing2.obj.pos.x, enemy.wing2.obj.pos.y, 0.f, enemy.wing2.obj.img.scale.x, enemy.wing2.obj.img.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void Attack_Charge(Enemy& enemy, int target_position) {
	enemy.speed = 200.f;
	enemy.velocity.x = enemy.speed * (f32)AEFrameRateControllerGetFrameTime();
	if (target_position == ENEMY_RIGHT) {
		enemy.velocity.x *= 1.0f;
	}
	if (target_position == ENEMY_LEFT) {
		enemy.velocity.x *= -1.0f;
	}
	enemy.obj.pos.x += enemy.velocity.x;
}


bool areAligned(AEVec2 player_position, AEVec2 enemy_position) {
	float tolerance = 10.f;
	return std::abs(player_position.y - enemy_position.y) < tolerance;
}

void Jump(Enemy& enemy, f32 value) {
	enemy.onFloor = false;
	enemy.velocity.y = value;
}