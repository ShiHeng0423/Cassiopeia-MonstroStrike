#include "Enemy.h"
#include "Player.h"
#include "AEEngine.h"
#include "EnemyUtils.h"
#include "TransformMatrix.h"
#include <iostream>
#include <cmath> // For abs function



void MoveTowards(Enemy& enemy, AEVec2 target_position) {

	// Calculate the direction towards the target position
	float direction = (target_position.x > enemy.obj.pos.x) ? 1.0f : -1.0f;

	// Calculate the distance to move based on speed and elapsed time
	enemy.velocity.x = enemy.speed * (f32)AEFrameRateControllerGetFrameTime();

	// Update the position based on the calculated direction and distance
	enemy.obj.pos.x += direction * enemy.velocity.x;
}

void MoveTowardsFLY(Enemy& enemy, AEVec2 target_position) {

	f32 Offset = 50.f;	//lifts a bit above the ground

	// Calculate the direction towards the target position for both x and y axes
	float x_direction = (target_position.x > enemy.obj.pos.x) ? 1.0f : -1.0f;
	float y_direction = (target_position.y + Offset > enemy.obj.pos.y) ? 1.0f : -1.0f;

	// Calculate the distance to move based on speed and elapsed time
	float delta_time = (f32)AEFrameRateControllerGetFrameTime();
	enemy.velocity.x = enemy.speed * delta_time;
	enemy.velocity.y = enemy.speed * delta_time;

	// Update the position based on the calculated direction and distance
	enemy.obj.pos.x += x_direction * enemy.velocity.x;
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


void SpawnBullet(AEVec2& enemy_position, AEVec2& player_position, std::vector<Bullet>& vecbullets) {
	//dir vec from enemy to player
	AEVec2 direction;
	AEVec2Sub(&direction, &player_position, &enemy_position); //player - enemy

	//normalize direction of bullet
	AEVec2Normalize(&direction, &direction);

	//create a bullet
	Bullet bullet;
	bullet.lifetime = 100;													//lifetime
	bullet.obj.img.pTex = bulletTex;										//image
	AEVec2Set(&bullet.obj.pos, enemy_position.x, enemy_position.y);			//start position
	AEVec2Set(&bullet.obj.img.scale, 50.f, 50.f);							//set scale of the image

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

void Attack_Charge(Enemy& enemy, int target_position, f32 speed) {
	enemy.speed = speed;
	enemy.velocity.x = enemy.speed * (f32)AEFrameRateControllerGetFrameTime();
	if (target_position == ENEMY_RIGHT) {
		enemy.velocity.x *= 1.0f;
	}
	if (target_position == ENEMY_LEFT) {
		enemy.velocity.x *= -1.0f;
	}
	enemy.obj.pos.x += enemy.velocity.x;
}

void Attack_Charge_w_Reverse(Enemy& enemy, int target_position, f32 speed, f32 reverse_offset) {
	reverse_offset *= (enemy.target_position == ENEMY_LEFT) ? 1.0f : -1.0f;

	enemy.speed = speed;
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

void isStuck(Enemy& enemy) {
	//Check distance travelled to see if enemy is stuck
	f32 distanceTravelled = AEVec2Distance(&enemy.last_position, &enemy.obj.pos);
	enemy.last_position = enemy.obj.pos;
	//std::cout << distanceTravelled << "\n";
	if (distanceTravelled < 0.2f) {
		if (enemy.stuckTimer < 0) { enemy.stuckTimer = enemy.timePassed; }	//if timer not started start it
		if (enemy.timePassed > enemy.stuckTimer + 1) {	//if enemy has been stuck for more than 1 sec
			enemy.isShooting = false;
			enemy.enemyNext = ENEMY_IDLE;
		}
	}
}

bool reachedPos(Enemy& enemy, AEVec2 waypoint) {
	if ((enemy.obj.pos.x >= enemy.waypoint.x - 1.0f) && (enemy.obj.pos.x <= enemy.waypoint.x + 1.0f)) {
		return true;
	}
	else {
		return false;
	}
}