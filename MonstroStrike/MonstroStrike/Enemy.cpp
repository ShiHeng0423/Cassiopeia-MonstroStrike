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






void Enemy_Init(AEVec2 scale, AEVec2 location, int startingState, Enemy& enemy) {

	//Initializing Enemy struct variables
	AEVec2Set(&enemy.obj.pos, location.x, location.y); //set starting location
	AEVec2Set(&enemy.obj.img.scale, scale.x, scale.y); //set scale of the image

	enemy.starting_position = location;
	enemy.waypoint.x = location.x + 200.f;
	enemy.waypoint.y = 0;
	enemy.target_position = ENEMY_DEFAULT;
	enemy.loop_idle = true;

	enemy.enemyCurrent = ENEMY_IDLE;
	enemy.enemyNext = ENEMY_IDLE;

	enemy.isAlive = true;
	enemy.isShooting = false;
	enemy.isCollision = false;

	enemy.timePassed = 0;

	//AABB Box init, Collision boxes
	enemy.onFloor = false;

	enemy.collisionBox.minimum.x = enemy.obj.pos.x - enemy.obj.img.scale.x * 0.5f;
	enemy.collisionBox.minimum.y = enemy.obj.pos.y - enemy.obj.img.scale.y * 0.5f;
	enemy.collisionBox.maximum.x = enemy.obj.pos.x + enemy.obj.img.scale.x * 0.5f;
	enemy.collisionBox.maximum.y = enemy.obj.pos.y + enemy.obj.img.scale.y * 0.5f;

	AEVec2Set(&enemy.boxArms.maximum, 0.f, 0.f);
	AEVec2Set(&enemy.boxHeadFeet.maximum, 0.f, 0.f);
	AEVec2Set(&enemy.collisionNormal, 0.f, 0.f);




	switch (enemy.enemyType) {
	case ENEMY_JUMPER:
		enemy.speed = 50.f;
		enemy.lineOfSight = 300.f;
		enemy.shootingRange = 250;
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
		enemy.speed = 80.f;
		enemy.lineOfSight = 300.f;
		enemy.shootingRange = 300.f;
		enemy.fireRate = 5.0f;
		enemy.timeSinceLastFire = 0;
		enemy.health = 100;
		enemy.mass = 100.f;
		AEVec2Set(&enemy.velocity, 0.f, 0.f); //Begin with no velocity
		break;
	case ENEMY_PASSIVE:

		break;
	case ENEMY_BOSS1:
		//main body
		enemy.speed = 80.f;
		enemy.lineOfSight = 300.f;
		enemy.shootingRange = 250;
		enemy.fireRate = 5.0f;
		enemy.timeSinceLastFire = 0;
		enemy.health = 100;
		enemy.mass = 10.f;
		AEVec2Set(&enemy.velocity, 0.f, 0.f); //Begin with no velocity

		//wing1
		enemy.wing1.isAlive = true;
		enemy.wing1.Offset = 100.f;
		AEVec2Set(&enemy.wing1.obj.pos, location.x + enemy.wing1.Offset, location.y); //set starting location
		AEVec2Set(&enemy.wing1.obj.img.scale, scale.x, scale.y); //set scale of the image
		enemy.wing1.collisionBox.minimum.x = enemy.wing1.obj.pos.x - enemy.wing1.obj.img.scale.x * 0.25f;
		enemy.wing1.collisionBox.minimum.y = enemy.wing1.obj.pos.y - enemy.wing1.obj.img.scale.y * 0.25f;
		enemy.wing1.collisionBox.maximum.x = enemy.wing1.obj.pos.x + enemy.wing1.obj.img.scale.x * 0.25f;
		enemy.wing1.collisionBox.maximum.y = enemy.wing1.obj.pos.y + enemy.wing1.obj.img.scale.y * 0.25f;
		enemy.wing1.fireRate = 5.0f;
		enemy.wing1.timeSinceLastFire = 0;
		enemy.wing1.health = 100;

		//wing2
		enemy.wing2.isAlive = true;
		enemy.wing2.Offset = -100.f;
		AEVec2Set(&enemy.wing2.obj.pos, location.x + enemy.wing2.Offset, location.y); //set starting location
		AEVec2Set(&enemy.wing2.obj.img.scale, scale.x, scale.y); //set scale of the image
		enemy.wing2.collisionBox.minimum.x = enemy.wing2.obj.pos.x - enemy.wing2.obj.img.scale.x * 0.25f;
		enemy.wing2.collisionBox.minimum.y = enemy.wing2.obj.pos.y - enemy.wing2.obj.img.scale.y * 0.25f;
		enemy.wing2.collisionBox.maximum.x = enemy.wing2.obj.pos.x + enemy.wing2.obj.img.scale.x * 0.25f;
		enemy.wing2.collisionBox.maximum.y = enemy.wing2.obj.pos.y + enemy.wing2.obj.img.scale.y * 0.25f;
		enemy.wing2.fireRate = 5.0f;
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

	if (!enemy.onFloor) {
		ApplyGravity(&enemy.velocity, enemy.mass);
	}

	
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

































//void ENEMY_JUMPER_Update(Enemy &enemy, struct Player& player)
//{
//	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos); 
//
//	if (enemy.health <= 0)
//	{
//		enemy.isAlive = false;
//	}
//
//	switch (enemy.enemyCurrent)
//	{
//	case ENEMY_IDLE:
//		if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
//			enemy.enemyNext = ENEMY_CHASE;
//			enemy.loop_idle = false;
//		}
//		else {
//			enemy.enemyNext = ENEMY_IDLE;
//	
//			if (enemy.loop_idle) {
//				MoveTowards(enemy, enemy.waypoint);
//
//				
//				if ((enemy.obj.pos.x >= enemy.waypoint.x - 2.0f) && (enemy.obj.pos.x <= enemy.waypoint.x + 2.0f)) {
//					enemy.loop_idle = false;
//				}
//			}
//			if ( (enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f) ) {
//				enemy.loop_idle = true;
//			}
//			if (!((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) && !(enemy.loop_idle)) {
//				
//				MoveTowards(enemy, enemy.starting_position);
//
//			}		
//		}
//		break;
//
//	case ENEMY_CHASE:
//
//		if (distanceFromPlayer <= enemy.shootingRange) {
//			enemy.enemyNext = ENEMY_ATTACK;
//		}
//		else if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
//			enemy.enemyNext = ENEMY_CHASE;
//			MoveTowards(enemy, player.obj.pos); 
//
//
//		}
//		else {
//			enemy.enemyNext = ENEMY_IDLE;
//			//std::cout << "Going back to IDLE!\n";
//		}
//		break;
//
//	case ENEMY_ATTACK:
//		if (distanceFromPlayer <= enemy.shootingRange) {
//			enemy.isShooting = true;
//			if (CanFire(enemy)) {
//				SpawnBullet(enemy.obj.pos, player.obj.pos, enemy.bullets);
//
//			}
//			enemy.enemyNext = ENEMY_ATTACK;
//		}
//		if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
//			enemy.isShooting = false;
//
//			enemy.enemyNext = ENEMY_CHASE;
//			//std::cout << "Shooting!\n";
//		}
//		break;
//	default:
//		break;
//	}
//
//	//for gravity
//	enemy.obj.pos.y += enemy.velocity.y * AEFrameRateControllerGetFrameTime();
//
//	enemy.enemyCurrent = enemy.enemyNext;
//	enemy.collisionBox.minimum.x = enemy.obj.pos.x - enemy.obj.img.scale.x * 0.25f;
//	enemy.collisionBox.minimum.y = enemy.obj.pos.y - enemy.obj.img.scale.y * 0.25f;
//	enemy.collisionBox.maximum.x = enemy.obj.pos.x + enemy.obj.img.scale.x * 0.25f;
//	enemy.collisionBox.maximum.y = enemy.obj.pos.y + enemy.obj.img.scale.y * 0.25f;
//
//	//Vertical
//	enemy.boxHeadFeet = enemy.collisionBox; // Get original collision box size
//	enemy.boxHeadFeet.minimum.y -= enemy.obj.img.scale.y * 0.25f;
//	enemy.boxHeadFeet.maximum.y += enemy.obj.img.scale.y * 0.25f;
//
//	//Horizontal
//	enemy.boxArms = enemy.collisionBox;
//	enemy.boxArms.minimum.x -= enemy.obj.img.scale.x * 0.25f;
//	enemy.boxArms.maximum.x += enemy.obj.img.scale.x * 0.25f;
//
//}
//








void ENEMY_FLY_Update(Enemy& enemy, struct Player& player)
{

	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);
	if (enemy.health <= 0)
	{
		enemy.isAlive = false;
	}

	switch (enemy.enemyCurrent)
	{
	case ENEMY_IDLE:
		if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
			enemy.enemyNext = ENEMY_CHASE;
			enemy.loop_idle = false;
			//std::cout << "HELLO CHASING\n";
		}
		else {
			enemy.enemyNext = ENEMY_IDLE;
			if (enemy.loop_idle) {
				MoveTowards(enemy, enemy.waypoint);

				if ((enemy.obj.pos.x >= enemy.waypoint.x - 2.0f) && (enemy.obj.pos.x <= enemy.waypoint.x + 2.0f)) {
					enemy.loop_idle = false;
				}
			}
			if ((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) {
				enemy.loop_idle = true;
			}
			if (!((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) && !(enemy.loop_idle)) {

				MoveTowards(enemy, enemy.starting_position);

			}


		}
		break;

	case ENEMY_CHASE:

		if (distanceFromPlayer <= enemy.shootingRange) {
			enemy.enemyNext = ENEMY_ATTACK;
		}
		else if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
			enemy.enemyNext = ENEMY_CHASE;
			MoveTowards(enemy, player.obj.pos);

			//std::cout << "HELLO CHASING HERE!\n";

		}
		else {
			enemy.enemyNext = ENEMY_IDLE;
			//std::cout << "HELLO Going back to IDLE!\n";
		}
		break;

	case ENEMY_ATTACK:
		if (distanceFromPlayer <= enemy.shootingRange) {
			enemy.isShooting = true;
			if (CanFire(enemy)) {
				SpawnBullet(enemy.obj.pos, player.obj.pos, enemy.bullets);
			}
			enemy.enemyNext = ENEMY_ATTACK;
		}
		if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
			enemy.isShooting = false;

			enemy.enemyNext = ENEMY_CHASE;
			//std::cout << "Shooting!\n";
		}
		break;
	default:
		break;
	}


	//for gravity
	enemy.obj.pos.y += enemy.velocity.y * AEFrameRateControllerGetFrameTime();

	enemy.enemyCurrent = enemy.enemyNext;
	enemy.collisionBox.minimum.x = enemy.obj.pos.x - enemy.obj.img.scale.x * 0.5f;
	enemy.collisionBox.minimum.y = enemy.obj.pos.y - enemy.obj.img.scale.y * 0.5f;
	enemy.collisionBox.maximum.x = enemy.obj.pos.x + enemy.obj.img.scale.x * 0.5f;
	enemy.collisionBox.maximum.y = enemy.obj.pos.y + enemy.obj.img.scale.y * 0.5f;


		//Vertical
	enemy.boxHeadFeet = enemy.collisionBox; // Get original collision box size
	enemy.boxHeadFeet.minimum.y -= enemy.obj.img.scale.y * 0.01f;
	enemy.boxHeadFeet.maximum.y += enemy.obj.img.scale.y * 0.01f;

	//Horizontal
	enemy.boxArms = enemy.collisionBox;
	enemy.boxArms.minimum.x -= enemy.obj.img.scale.x * 0.01f;
	enemy.boxArms.maximum.x += enemy.obj.img.scale.x * 0.01f;
}

