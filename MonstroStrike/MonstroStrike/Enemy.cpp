#include "Enemy.h"
#include "Player.h"
#include "AEEngine.h"


#include <iostream>






int enemyCurrent = 0, enemyNext = 0;
f32 timeSinceLastFire;
f32 distanceFromPlayer;



Enemy* ENEMY_Init(const char* filename, AEVec2 scale, AEVec2 location, AEVec2 speed, int startingState) {

	Enemy* enemy = new Enemy;
	enemy->obj.img.pTex = AEGfxTextureLoad(filename); //assign the texture
	enemy->obj.speed = speed; //idk what this speed is doing
	enemy->starting_position = location;
	enemy->waypoint1.x = location.x + 200.f;


	AEVec2Set(&enemy->obj.pos, location.x, location.y); //set starting location
	AEVec2Set(&enemy->obj.img.scale, scale.x, scale.y); //set scale of the image


	//AEMtx33Scale(&enemy->scale, 50.f, 50.f);
	//AEMtx33Trans(&enemy->translation, enemy->pos_x, enemy->pos_y);
	//AEMtx33Concat(&enemy->transform, &enemy->translation, &enemy->scale);


	//enemy->transform = ObjectTransformationMatrixSet(enemy->pos_x, enemy->pos_y, 0.f, 50.f, 50.f);



	enemyCurrent = enemyNext = startingState;

	return enemy;
}



void ENEMY_Update(Enemy &enemy, Player& player)
{
	distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos); 

	switch (enemyCurrent)
	{
	case ENEMY_IDLE:
		if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
			enemyNext = ENEMY_CHASE;
			enemy.loop_idle = false;
			std::cout << "CHASING\n";
		}
		else {
			enemyNext = ENEMY_IDLE;
			//long condition to stop jittering
			//loop_idle is a bool that works as a checker to start the "patrolling mode" for enemy, instead of standing still idling
			if (enemy.loop_idle) {
				MoveTowards(enemy.obj.pos, enemy.waypoint1, enemy.speed);
				if ((enemy.obj.pos.x >= enemy.waypoint1.x - 2.0f) && (enemy.obj.pos.x <= enemy.waypoint1.x + 2.0f)) {
					enemy.loop_idle = false;
				}
			}
			if ( (enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f) ) {
				enemy.loop_idle = true;
			}
			if (!((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) && !(enemy.loop_idle)) {
				
				MoveTowards(enemy.obj.pos, enemy.starting_position, enemy.speed);
			}


			std::cout << enemy.obj.pos.x << "IDLE\n";
		}
		break;

	case ENEMY_CHASE:

		if (distanceFromPlayer <= enemy.shootingRange) {
			enemyNext = ENEMY_SHOOT;
		}
		else if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
			enemyNext = ENEMY_CHASE;
			MoveTowards(enemy.obj.pos, player.obj.pos, enemy.speed); 
			std::cout  << "CHASING HERE!\n";

		}
		else {
			enemyNext = ENEMY_IDLE;
			std::cout << "Going back to IDLE!\n";
		}
		break;

	case ENEMY_SHOOT:
		if (distanceFromPlayer <= enemy.shootingRange) {
			if (CanFire(enemy.fireRate)) {
				std::cout << "Shooting!\n";
			}
			enemyNext = ENEMY_SHOOT;
		}
		if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
			enemyNext = ENEMY_CHASE;
			std::cout << "Shooting!\n";
		}
		break;
	case ENEMY_RETURN:
		break;
	default:
		std::cout << "HELLO\n";
		break;
	}

	enemyCurrent = enemyNext;


}


//void Enemy_Draw(Enemy& enemy, AEGfxVertexList* pWhiteSquareMesh) {
//	AEGfxTextureSet(enemy.obj.img.pTex, 0, 0);
//	AEGfxSetTransform(ObjectTransformationMatrixSet(enemy.pos_x, enemy.pos_y, 0.f, 50.f, 50.f).m);
//	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
//}

void Enemy_Free(Enemy* enemy) {
	delete enemy;
}






























void MoveTowards(AEVec2& moving_entity, AEVec2 target_position, f32 speed) {

	f32 speed_x = speed;

	if ((moving_entity.x != target_position.x)) {
		if (moving_entity.x >= target_position.x) {
			speed_x *= -1.0f;
			//std::cout << "x greater\n";
		}
		if (moving_entity.x <= target_position.x) {
			speed_x *= 1.0f;

		}
		//if (moving_entity.y >= target_position.y) {
		//	speed_y *= -1.0f;
		//	//std::cout << "y greater\n";
		//}
		//if (moving_entity.y <= target_position.y) {
		//	speed_y *= 1.0f;

		//}
	}


	moving_entity.x += speed_x * (f32)AEFrameRateControllerGetFrameTime();
	/*moving_entity.y += speed_y * (f32)AEFrameRateControllerGetFrameTime();*/


	//AEMtx33Trans(&entity.translation, moving_entity.x, moving_entity.y);
	//AEMtx33Concat(&entity.transform, &entity.translation, &entity.scale);
}




bool CanFire(f32 fireRate) {

	timeSinceLastFire += (f32)AEFrameRateControllerGetFrameTime();

	if (timeSinceLastFire >= 1.0f / fireRate) {
		timeSinceLastFire = 0.0f; //reset
		return true;
	}
	return false;
}