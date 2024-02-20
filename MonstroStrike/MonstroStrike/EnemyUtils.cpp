#include "Enemy.h"
#include "Player.h"
#include "AEEngine.h"
#include <iostream>

f32 timeSinceLastFire;

void MoveTowards(AEVec2& moving_entity, AEVec2 target_position, f32 speed) {

	f32 speed_x = speed;

	if ((moving_entity.x != target_position.x)) {
		if (moving_entity.x >= target_position.x) {
			speed_x *= -1.0f;
		}
		if (moving_entity.x <= target_position.x) {
			speed_x *= 1.0f;

		}
	}

	moving_entity.x += speed_x * (f32)AEFrameRateControllerGetFrameTime();
}


bool CanFire(f32 fireRate) {

	timeSinceLastFire += (f32)AEFrameRateControllerGetFrameTime();

	if (timeSinceLastFire >= 1.0f / fireRate) {
		timeSinceLastFire = 0.0f; //reset
		return true;
	}
	return false;
}


void SpawnBullet(AEVec2& enemy_position, AEVec2& player_position, std::vector<Bullet>& bullets) {
	//dir vec from enemy to player
	AEVec2 direction;
	AEVec2Sub(&direction, &player_position, &enemy_position); //player - enemy

	//normalize direction of bullet
	AEVec2Normalize(&direction, &direction);

	//create a bullet
	Bullet bullet;
	bullet.lifetime = 50;													//lifetime
	bullet.obj.img.pTex = AEGfxTextureLoad("Assets/RedCircle.png");			//image
	AEVec2Set(&bullet.obj.pos, enemy_position.x, enemy_position.y);			//start position
	AEVec2Set(&bullet.obj.img.scale, 100.f, 100.f);							//set scale of the image

	//set velocity of bullet
	bullet.bulletSpeed = 5.f;
	AEVec2Set(&bullet.bulletVel, direction.x * bullet.bulletSpeed, direction.y * bullet.bulletSpeed);

	// Push the bullet into the vector
	bullets.push_back(bullet);

}

