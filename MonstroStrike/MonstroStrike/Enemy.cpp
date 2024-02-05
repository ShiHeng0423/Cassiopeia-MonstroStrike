#include "Enemy.h"
#include "Player.h"
#include "AEEngine.h"


#include <iostream>







f32 timeSinceLastFire;




Enemy* ENEMY_Init(AEVec2 scale, AEVec2 location, int enemy_type, int starting_state) {

	Enemy* enemy = new Enemy;
	switch (enemy_type) {
	case ENEMY_JUMPER:	
		enemy->obj.img.pTex = AEGfxTextureLoad("Assets/Kronii_Pixel.png"); //assign the texture
		break;
	case ENEMY_CHARGER:
		break;
	case ENEMY_FLY:
		enemy->obj.img.pTex = AEGfxTextureLoad("Assets/Kronii_Pixel_2.png"); //assign the texture
		break;
	case ENEMY_PASSIVE:
		break;
	case ENEMY_BOSS1:
		enemy->obj.img.pTex = AEGfxTextureLoad("Assets/SubaDuck.png"); //assign the texture
		break;
	case ENEMY_BOSS1_WING1:
		enemy->obj.img.pTex = AEGfxTextureLoad("Assets/RedCircle.png"); //assign the texture
		break;
	case ENEMY_BOSS1_WING2:
		enemy->obj.img.pTex = AEGfxTextureLoad("Assets/BlueCircle.png"); //assign the texture
		break;
	case ENEMY_BOSS2:
		break;

	default:
		break;
		
	}

	enemy->starting_position = location;
	enemy->waypoint1.x = location.x + 200.f;


	AEVec2Set(&enemy->obj.pos, location.x, location.y); //set starting location
	AEVec2Set(&enemy->obj.img.scale, scale.x, scale.y); //set scale of the image






	enemy->enemyCurrent = starting_state;
	enemy->enemyNext = starting_state;
	enemy->enemyType = enemy_type;

	return enemy;
}










































void ENEMY_Update(Enemy &enemy, Player& player)
{
	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos); 

	switch (enemy.enemyCurrent)
	{
	case ENEMY_IDLE:
		if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
			enemy.enemyNext = ENEMY_CHASE;
			enemy.loop_idle = false;
			//std::cout << "CHASING\n";
		}
		else {
			enemy.enemyNext = ENEMY_IDLE;
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


			//std::cout << enemy.obj.pos.x << "IDLE\n";
		}
		break;

	case ENEMY_CHASE:

		if (distanceFromPlayer <= enemy.shootingRange) {
			enemy.enemyNext = ENEMY_SHOOT;
		}
		else if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
			enemy.enemyNext = ENEMY_CHASE;
			MoveTowards(enemy.obj.pos, player.obj.pos, enemy.speed); 
			//std::cout  << "CHASING HERE!\n";

		}
		else {
			enemy.enemyNext = ENEMY_IDLE;
			//std::cout << "Going back to IDLE!\n";
		}
		break;

	case ENEMY_SHOOT:
		if (distanceFromPlayer <= enemy.shootingRange) {
			enemy.isShooting = true;
			if (CanFire(enemy.fireRate)) {
				//std::cout << "Shooting!\n";
			}
			enemy.enemyNext = ENEMY_SHOOT;
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

	enemy.enemyCurrent = enemy.enemyNext;


}









void ENEMY1_Update(Enemy& enemy, Player& player)
{
	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);

	switch (enemy.enemyCurrent)
	{
	case ENEMY_IDLE:
		if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
			enemy.enemyNext = ENEMY_CHASE;
			enemy.loop_idle = false;
			std::cout << "HELLO CHASING\n";
		}
		else {
			enemy.enemyNext = ENEMY_IDLE;
			//long condition to stop jittering
			//loop_idle is a bool that works as a checker to start the "patrolling mode" for enemy, instead of standing still idling
			if (enemy.loop_idle) {
				MoveTowards(enemy.obj.pos, enemy.waypoint1, enemy.speed);
				if ((enemy.obj.pos.x >= enemy.waypoint1.x - 2.0f) && (enemy.obj.pos.x <= enemy.waypoint1.x + 2.0f)) {
					enemy.loop_idle = false;
				}
			}
			if ((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) {
				enemy.loop_idle = true;
			}
			if (!((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) && !(enemy.loop_idle)) {

				MoveTowards(enemy.obj.pos, enemy.starting_position, enemy.speed);
			}


			//std::cout << enemy.obj.pos.x << "HELLO IDLE\n";
		}
		break;

	case ENEMY_CHASE:

		if (distanceFromPlayer <= enemy.shootingRange) {
			enemy.enemyNext = ENEMY_SHOOT;
		}
		else if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
			enemy.enemyNext = ENEMY_CHASE;
			MoveTowards(enemy.obj.pos, player.obj.pos, enemy.speed);
			//std::cout << "HELLO CHASING HERE!\n";

		}
		else {
			enemy.enemyNext = ENEMY_IDLE;
			//std::cout << "HELLO Going back to IDLE!\n";
		}
		break;

	case ENEMY_SHOOT:
		if (distanceFromPlayer <= enemy.shootingRange) {
			enemy.isShooting = true;
			if (CanFire(enemy.fireRate)) {
				//std::cout << "HELLO Shooting!\n";
			}
			enemy.enemyNext = ENEMY_SHOOT;
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

	enemy.enemyCurrent = enemy.enemyNext;


}



void ENEMY_BOSS_Update(Enemy& enemy, Player& player)
{
	f32 distanceFromPlayer = AEVec2Distance(&player.obj.pos, &enemy.obj.pos);

	if (enemy.enemyType == ENEMY_BOSS1) {
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
				//long condition to stop jittering
				//loop_idle is a bool that works as a checker to start the "patrolling mode" for enemy, instead of standing still idling
				if (enemy.loop_idle) {
					MoveTowards(enemy.obj.pos, enemy.waypoint1, enemy.speed);
					if ((enemy.obj.pos.x >= enemy.waypoint1.x - 2.0f) && (enemy.obj.pos.x <= enemy.waypoint1.x + 2.0f)) {
						enemy.loop_idle = false;
					}
				}
				if ((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) {
					enemy.loop_idle = true;
				}
				if (!((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) && !(enemy.loop_idle)) {

					MoveTowards(enemy.obj.pos, enemy.starting_position, enemy.speed);
				}
				//std::cout << enemy.obj.pos.x << "HELLO IDLE\n";
			}
			break;

		case ENEMY_CHASE:

			if (distanceFromPlayer <= enemy.shootingRange) {
				enemy.enemyNext = ENEMY_SHOOT;
			}
			else if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
				enemy.enemyNext = ENEMY_CHASE;
				MoveTowards(enemy.obj.pos, player.obj.pos, enemy.speed);
				//std::cout << "HELLO CHASING HERE!\n";

			}
			else {
				enemy.enemyNext = ENEMY_IDLE;
				//std::cout << "HELLO Going back to IDLE!\n";
			}
			break;

		case ENEMY_SHOOT:
			if (distanceFromPlayer <= enemy.shootingRange) {
				if (CanFire(enemy.fireRate)) {
					//std::cout << "HELLO Shooting!\n";
				}
				enemy.enemyNext = ENEMY_SHOOT;
			}
			if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
				enemy.enemyNext = ENEMY_CHASE;
				//std::cout << "Shooting!\n";
			}
			break;
		default:
			break;
		}
		enemy.enemyCurrent = enemy.enemyNext;
	}//if boss1 AI

	if (enemy.enemyType == ENEMY_BOSS1_WING1) {
		if (AEInputCheckTriggered(AEVK_X)) {
			enemy.isAlive = false;
			return;
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
				//long condition to stop jittering
				//loop_idle is a bool that works as a checker to start the "patrolling mode" for enemy, instead of standing still idling
				if (enemy.loop_idle) {
					MoveTowards(enemy.obj.pos, enemy.waypoint1, enemy.speed);
					if ((enemy.obj.pos.x >= enemy.waypoint1.x - 2.0f) && (enemy.obj.pos.x <= enemy.waypoint1.x + 2.0f)) {
						enemy.loop_idle = false;
					}
				}
				if ((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) {
					enemy.loop_idle = true;
				}
				if (!((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) && !(enemy.loop_idle)) {

					MoveTowards(enemy.obj.pos, enemy.starting_position, enemy.speed);
				}
				//std::cout << enemy.obj.pos.x << "HELLO IDLE\n";
			}
			break;

		case ENEMY_CHASE:

			if (distanceFromPlayer <= enemy.shootingRange) {
				enemy.enemyNext = ENEMY_SHOOT;
			}
			else if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
				enemy.enemyNext = ENEMY_CHASE;
				MoveTowards(enemy.obj.pos, player.obj.pos, enemy.speed);
				//std::cout << "HELLO CHASING HERE!\n";

			}
			else {
				enemy.enemyNext = ENEMY_IDLE;
				//std::cout << "HELLO Going back to IDLE!\n";
			}
			break;

		case ENEMY_SHOOT:
			if (distanceFromPlayer <= enemy.shootingRange) {
				if (CanFire(enemy.fireRate)) {
					//std::cout << "HELLO Shooting!\n";
				}
				enemy.enemyNext = ENEMY_SHOOT;
			}
			if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
				enemy.enemyNext = ENEMY_CHASE;
				//std::cout << "Shooting!\n";
			}
			break;
		default:
			break;
		}
		enemy.enemyCurrent = enemy.enemyNext;

	
	}


	if (enemy.enemyType == ENEMY_BOSS1_WING2) {
		if (AEInputCheckTriggered(AEVK_Z)) {
			enemy.isAlive = false;
			return;
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
				//long condition to stop jittering
				//loop_idle is a bool that works as a checker to start the "patrolling mode" for enemy, instead of standing still idling
				if (enemy.loop_idle) {
					MoveTowards(enemy.obj.pos, enemy.waypoint1, enemy.speed);
					if ((enemy.obj.pos.x >= enemy.waypoint1.x - 2.0f) && (enemy.obj.pos.x <= enemy.waypoint1.x + 2.0f)) {
						enemy.loop_idle = false;
					}
				}
				if ((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) {
					enemy.loop_idle = true;
				}
				if (!((enemy.obj.pos.x >= enemy.starting_position.x - 1.0f) && (enemy.obj.pos.x <= enemy.starting_position.x + 1.0f)) && !(enemy.loop_idle)) {

					MoveTowards(enemy.obj.pos, enemy.starting_position, enemy.speed);
				}
				//std::cout << enemy.obj.pos.x << "HELLO IDLE\n";
			}
			break;

		case ENEMY_CHASE:

			if (distanceFromPlayer <= enemy.shootingRange) {
				enemy.enemyNext = ENEMY_SHOOT;
			}
			else if (distanceFromPlayer < enemy.lineOfSight && distanceFromPlayer > enemy.shootingRange) {
				enemy.enemyNext = ENEMY_CHASE;
				MoveTowards(enemy.obj.pos, player.obj.pos, enemy.speed);
				//std::cout << "HELLO CHASING HERE!\n";

			}
			else {
				enemy.enemyNext = ENEMY_IDLE;
				//std::cout << "HELLO Going back to IDLE!\n";
			}
			break;

		case ENEMY_SHOOT:
			if (distanceFromPlayer <= enemy.shootingRange) {
				enemy.isShooting = true;
				if (CanFire(enemy.fireRate)) {
					//std::cout << "HELLO Shooting!\n";
				}
				enemy.enemyNext = ENEMY_SHOOT;
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
		enemy.enemyCurrent = enemy.enemyNext;


	}


}




















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


void SpawnBullet(AEVec2& shooter) {
}


void Enemy_Update_Choose(Enemy& enemy, Player& player) {
	switch (enemy.enemyType) {
	case ENEMY_JUMPER:
		ENEMY_Update(enemy, player);
		break;
	case ENEMY_CHARGER:
		break;
	case ENEMY_FLY:
		ENEMY1_Update(enemy, player);
		break;
	case ENEMY_PASSIVE:
		break;
	case ENEMY_BOSS1:
		ENEMY_BOSS_Update(enemy, player);
		break;
	case ENEMY_BOSS1_WING1:
		ENEMY_BOSS_Update(enemy, player);
		break;
	case ENEMY_BOSS1_WING2:
		ENEMY_BOSS_Update(enemy, player);
		break;
	case ENEMY_BOSS2:
		break;

	default:
		break;

	}

}