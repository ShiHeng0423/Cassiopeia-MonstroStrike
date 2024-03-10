#include "Enemy.h"
#include "Player.h"
#include "AEEngine.h"
#include "EnemyUtils.h"
#include "TransformMatrix.h"
#include <iostream>
#include <cmath> // For abs function



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
	bullet.lifetime = 100;													//lifetime
	bullet.obj.img.pTex = bulletTex;										//image
	AEVec2Set(&bullet.obj.pos, enemyPosition.x, enemyPosition.y);			//start position
	AEVec2Set(&bullet.obj.img.scale, 25.f, 25.f);							//set scale of the image

	//set velocity of bullet
	bullet.bulletSpeed = 2.5f;
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
bool areAligned(AEVec2 playerPosition, AEVec2 enemyPosition) {
	float tolerance = 10.f;
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
			enemy.isShooting = false;
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

