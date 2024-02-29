#include "Enemy.h"
#include "Player.h"
#include "AEEngine.h"
#include "Physics.h"

#include <iostream>




void Enemy_Load(int enemy_type, std::vector<Enemy>& vecEnemy ) {

	Enemy enemy;

	switch (enemy_type) {
	case ENEMY_JUMPER:
		enemy.obj.img.pTex = AEGfxTextureLoad("Assets/Enemy_Assets/Enemy_Jumper_Normal.png");
		enemy.angrytex = AEGfxTextureLoad("Assets/Enemy_Assets/Enemy_Jumper_Angry.png");
		break;
	case ENEMY_CHARGER:
		enemy.obj.img.pTex = AEGfxTextureLoad("Assets/Enemy_Assets/Enemy_Charger_Normal.png");
		enemy.angrytex = AEGfxTextureLoad("Assets/Enemy_Assets/Enemy_Charger_Angry.png");
		break;
	case ENEMY_FLY:
		enemy.obj.img.pTex = AEGfxTextureLoad("Assets/Enemy_Assets/Enemy_FLY_Normal.png");
		enemy.angrytex = AEGfxTextureLoad("Assets/Enemy_Assets/Enemy_FLY_Angry.png");
		break;
	case ENEMY_PASSIVE:
		break;
	case ENEMY_BOSS1:
		enemy.obj.img.pTex = AEGfxTextureLoad("Assets/SubaDuck.png");
		enemy.wing1.obj.img.pTex = AEGfxTextureLoad("Assets/BlueCircle.png");
		enemy.wing2.obj.img.pTex = AEGfxTextureLoad("Assets/RedCircle.png");

		enemy.angrytex = AEGfxTextureLoad("Assets/SubaDuck.png");
		break;
	case ENEMY_BOSS2:
		break;

	default:
		std::cerr << "Unknown enemy type: " << enemy_type << std::endl;
		return;

	}

	enemy.enemyType = enemy_type; //initialize enemy_type
	vecEnemy.push_back(enemy);
}

void FreeEnemy(std::vector<Enemy>& vecEnemy) {
	for (Enemy& enemy : vecEnemy) {
		AEGfxTextureUnload(enemy.obj.img.pTex);	//default tex
		AEGfxTextureUnload(enemy.angrytex);		//angry tex

		if (enemy.enemyType == ENEMY_BOSS1) {	//check if need to free wings
			AEGfxTextureUnload(enemy.wing1.obj.img.pTex);
			AEGfxTextureUnload(enemy.wing2.obj.img.pTex);
		}
	}
}




void Enemy_Init(AEVec2 scale, AEVec2 location, int startingState, Enemy& enemy) {

	//Initializing Enemy struct variables
	AEVec2Set(&enemy.obj.pos, location.x, location.y); //set starting location
	AEVec2Set(&enemy.obj.img.scale, scale.x, scale.y); //set scale of the image

	enemy.starting_position = location;
	enemy.last_position = location;
	enemy.waypoint.x = location.x + 200.f;
	enemy.waypoint.y = 0;
	enemy.target_position = ENEMY_DEFAULT;
	enemy.loop_idle = true;

	enemy.enemyCurrent = ENEMY_IDLE;
	enemy.enemyNext = ENEMY_IDLE;

	enemy.isAlive = true;
	enemy.isShooting = false;
	enemy.isCollision = false;
	enemy.isFlying = false;

	enemy.timePassed = 0.f;

	enemy.stuckTimer = -1.f;
	//AABB Box init, Collision boxes
	enemy.onFloor = true;
	enemy.isFalling = false;

	enemy.collisionBox.minimum.x = enemy.obj.pos.x - enemy.obj.img.scale.x * 0.5f;
	enemy.collisionBox.minimum.y = enemy.obj.pos.y - enemy.obj.img.scale.y * 0.5f;
	enemy.collisionBox.maximum.x = enemy.obj.pos.x + enemy.obj.img.scale.x * 0.5f;
	enemy.collisionBox.maximum.y = enemy.obj.pos.y + enemy.obj.img.scale.y * 0.5f;

	AEVec2Set(&enemy.boxArms.maximum, 0.f, 0.f);
	AEVec2Set(&enemy.boxHeadFeet.maximum, 0.f, 0.f);
	AEVec2Set(&enemy.collisionNormal, 0.f, 0.f);

	AEVec2Set(&enemy.spawnPoint, 0.f, 0.f);


	enemy.gravityForce = 0.f;

	switch (enemy.enemyType) {
	case ENEMY_JUMPER:
		enemy.speed = 50.f;
		enemy.lineOfSight = 300.f;
		enemy.shootingRange = 250.f;
		enemy.fireRate = 1.0f;
		enemy.timeSinceLastFire = 0;
		enemy.health = 100;
		enemy.mass = 80.f;
		AEVec2Set(&enemy.velocity, 0.f, 0.f); //Begin with no velocity
		break;
	case ENEMY_CHARGER:
		enemy.speed = 80.f;
		enemy.lineOfSight = 300.f;
		enemy.shootingRange = 300.f;
		enemy.fireRate = 5.0f;
		enemy.timeSinceLastFire = 0;
		enemy.health = 100;
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
		enemy.health = 100;
		enemy.mass = 100.f;
		AEVec2Set(&enemy.velocity, 0.f, 0.f); //Begin with no velocity
		break;
	case ENEMY_PASSIVE:

		break;
	case ENEMY_BOSS1:
		//main body
		enemy.isFlying = true;
		enemy.speed = 80.f;
		enemy.lineOfSight = 300.f;
		enemy.shootingRange = 250.f;
		enemy.fireRate = 1.0f;
		enemy.timeSinceLastFire = 0;
		enemy.health = 100;
		enemy.mass = 10.f;
		AEVec2Set(&enemy.velocity, 0.f, 0.f); //Begin with no velocity

		//wing1
		enemy.wing1.isAlive = true;
		enemy.wing1.Offset = 100.f;
		AEVec2Set(&enemy.wing1.obj.pos, location.x + enemy.wing1.Offset, location.y); //set starting location
		AEVec2Set(&enemy.wing1.obj.img.scale, scale.x, scale.y); //set scale of the image
		enemy.wing1.collisionBox.minimum.x = enemy.wing1.obj.pos.x - enemy.wing1.obj.img.scale.x * 0.5f;
		enemy.wing1.collisionBox.minimum.y = enemy.wing1.obj.pos.y - enemy.wing1.obj.img.scale.y * 0.5f;
		enemy.wing1.collisionBox.maximum.x = enemy.wing1.obj.pos.x + enemy.wing1.obj.img.scale.x * 0.5f;
		enemy.wing1.collisionBox.maximum.y = enemy.wing1.obj.pos.y + enemy.wing1.obj.img.scale.y * 0.5f;
		enemy.wing1.fireRate = 1.0f;
		enemy.wing1.timeSinceLastFire = 0;
		enemy.wing1.health = 100;

		//wing2
		enemy.wing2.isAlive = true;
		enemy.wing2.Offset = -100.f;
		AEVec2Set(&enemy.wing2.obj.pos, location.x + enemy.wing2.Offset, location.y); //set starting location
		AEVec2Set(&enemy.wing2.obj.img.scale, scale.x, scale.y); //set scale of the image
		enemy.wing2.collisionBox.minimum.x = enemy.wing2.obj.pos.x - enemy.wing2.obj.img.scale.x * 0.5f;
		enemy.wing2.collisionBox.minimum.y = enemy.wing2.obj.pos.y - enemy.wing2.obj.img.scale.y * 0.5f;
		enemy.wing2.collisionBox.maximum.x = enemy.wing2.obj.pos.x + enemy.wing2.obj.img.scale.x * 0.5f;
		enemy.wing2.collisionBox.maximum.y = enemy.wing2.obj.pos.y + enemy.wing2.obj.img.scale.y * 0.5f;
		enemy.wing2.fireRate = 1.0f;
		enemy.wing2.timeSinceLastFire = 0;
		enemy.wing2.health = 100;
		break;
	case ENEMY_BOSS2:
		break;
	default:
		return;
	}


}







void Enemy_Update_Choose(Enemy& enemy, struct Player& player) {
//(update bullet)---------------------------------------------------------------------------------------
	for (std::vector<Bullet>::iterator it = enemy.bullets.begin(); it != enemy.bullets.end(); ) {
		it->obj.pos.x += it->bulletVel.x * (f32)AEFrameRateControllerGetFrameTime() * 100.f;
		it->obj.pos.y += it->bulletVel.y * (f32)AEFrameRateControllerGetFrameTime() * 100.f;

		it->collisionBox.minimum.x = it->obj.pos.x - it->obj.img.scale.x * 0.5f;
		it->collisionBox.minimum.y = it->obj.pos.y - it->obj.img.scale.y * 0.5f;
		it->collisionBox.maximum.x = it->obj.pos.x + it->obj.img.scale.x * 0.5f;
		it->collisionBox.maximum.y = it->obj.pos.y + it->obj.img.scale.y * 0.5f;


		it->lifetime--;	//decrease lifetime
		if (it->lifetime <= 0) {
			it = enemy.bullets.erase(it);
		}
		else {
			++it;
		}
	}
//(update bullet)---------------------------------------------------------------------------------------
	


	enemy.timeSinceLastFire += (f32)AEFrameRateControllerGetFrameTime();
	enemy.wing1.timeSinceLastFire += (f32)AEFrameRateControllerGetFrameTime();
	enemy.wing2.timeSinceLastFire += (f32)AEFrameRateControllerGetFrameTime();

	if (!enemy.isFlying) {
		ApplyGravity(&enemy.velocity, enemy.mass);
	}

	
	ApplyGravity(&enemy.velocity, enemy.mass, &enemy.onFloor, &enemy.gravityForce, &enemy.isFalling);

	switch (enemy.enemyType) {
	case ENEMY_JUMPER:
		ENEMY_JUMPER_Update(enemy, player);
		break;
	case ENEMY_CHARGER:
		ENEMY_CHARGER_Update(enemy, player);
		break;
	case ENEMY_FLY:
		ENEMY_FLY_Update(enemy, player);
		break;
	case ENEMY_PASSIVE:
		break;
	case ENEMY_BOSS1:
		ENEMY_BOSS_Update(enemy, player);
		break;
	case ENEMY_BOSS2:
		break;

	default:
		break;

	}


}

