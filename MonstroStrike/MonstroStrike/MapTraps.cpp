/*!************************************************************************
  \file					MapTraps.cpp
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (100%)
  \brief				File containing definitions of functions for managing map traps.

  This file contains definitions of functions and structures of the trap entities essential for managing map traps.
  The header defines a structure for representing trap details, along with functions for loading trap textures,
  storing trap information, updating traps, and drawing traps on the screen. This file also include private functions
  which handles the state of the trap entities such as the arrows and fireballs.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/
#include "MapTraps.h"
#include <iostream>
#include "main.h"
#include "ParticleSystem.h"

namespace {
	std::vector<TrapDetails> levelTrapsContainer;

	AEGfxTexture* sawTexture;
	AEGfxTexture* arrowTrapTexture;
	AEGfxTexture* fireCircleTexture;
	AEGfxTexture* fireTrapTexture;
	AEGfxTexture* arrowTexture;

	int numberOfFireBalls = 3;

#pragma region ArrowTrap
	void TriggerArrow(GridTypes type, AEVec2 trapPos, int damage);

	void UpdateArrow();

	enum ArrowType {
		POISON_ARROW,
		SLOWNESS_ARROW
	};

	struct Arrow {
		Object obj;
		AABB collisionBox;
		AEVec2 velocity;
		f32 currLifeTime = 3.f;
		f32 maxLifeTime = 3.f;
		int damage = 0;

		ArrowType type;

		void UpdateCollisionBox();
	};
	std::vector<Arrow> arrowContainer;

#pragma endregion


#pragma region FireBallsTrap

	struct FireBalls {
		Object obj;
		AABB collisionBox;
		AEVec2 originPoint;
		int damage;
		f32 hitPlayer;

		void UpdateCollisionBox();
	};

	std::vector<FireBalls> fireBallContainer;

	void SpawnFireballs(AEVec2 originPosition, int damage);
	void UpdateFireballs();

#pragma endregion
}

void TrapDetails::UpdateCollisionBox()
{
	collisionBox.minimum.x = obj.pos.x - obj.scale.x * 0.5f;
	collisionBox.minimum.y = obj.pos.y - obj.scale.y * 0.5f;
	collisionBox.maximum.x = obj.pos.x + obj.scale.x * 0.5f;
	collisionBox.maximum.y = obj.pos.y + obj.scale.y * 0.5f;
}

void PreLoadTrapsTexture()
{
	sawTexture = AEGfxTextureLoad("Assets/Traps/Trap_RotatingSaw.png");
	
	arrowTrapTexture = AEGfxTextureLoad("Assets/Traps/Trap_ArrowTrigger.png");

	fireCircleTexture = AEGfxTextureLoad("Assets/Traps/Trap_FireCircle.png");

	fireTrapTexture = AEGfxTextureLoad("Assets/Traps/Trap_FireOrigin.png");

	arrowTexture = AEGfxTextureLoad("Assets/Traps/Arrow.png");
}

void StoreTrapDetails(Grids2D theGrid)
{
	TrapDetails theTrap;

	theTrap.typeOfGrid = theGrid.typeOfGrid;
	theTrap.obj.pos = theGrid.position;
	if (!SetSpecificTrapDetail(&theTrap))
		return;

	theTrap.obj.scale = theGrid.size;
	theTrap.obj.speed = { 0 };
	theTrap.hitPlayer = false;

	levelTrapsContainer.push_back(theTrap);
}

bool SetSpecificTrapDetail(TrapDetails* trap)
{
	switch (trap->typeOfGrid)
	{
	case TRAPS_ROTATING_SAW_GRID:
		trap->obj.pTex = sawTexture;
		trap->trapDamage = 10;
		trap->obj.pos.y -= GRID_SIZE * 0.5f;
		return true;
		break;
	case TRAPS_POISON_ARROW_GRID:
		trap->obj.pTex = arrowTrapTexture; //To be multiply color to purple
		trap->trapDamage = 5;
		return true;
		break;
	case TRAPS_SLOWNESS_ARROW_GRID:
		trap->obj.pTex = arrowTrapTexture; //To be multiply color to grey
		trap->trapDamage = 5;
		return true;
		break;
	case TRAPS_FIRE_CIRCLE_GRID:
		trap->obj.pTex = fireTrapTexture;
		trap->trapDamage = 15;
		SpawnFireballs(trap->obj.pos, trap->trapDamage);
		return true;
		break;
	}

	return false;
}

void UpdateTraps() 
{
	for (size_t i = 0; i < levelTrapsContainer.size(); i++)
	{

		switch (levelTrapsContainer[i].typeOfGrid)
		{
		case TRAPS_ROTATING_SAW_GRID:
			levelTrapsContainer[i].obj.rotate = fmod(levelTrapsContainer[i].obj.rotate + PI * 1.1f, 2 * PI);
			break;
		}

		//Check collision
		if (AABBvsAABB(levelTrapsContainer[i].collisionBox, gameManager->GetPlayer()->GetPlayerCollisionBox()))
		{
			//For specific details
			switch (levelTrapsContainer[i].typeOfGrid)
			{
			case TRAPS_ROTATING_SAW_GRID:
				if (!levelTrapsContainer[i].hitPlayer)
				{
					levelTrapsContainer[i].hitPlayer = true;
					gameManager->GetPlayer()->DamageToPlayer(levelTrapsContainer[i].trapDamage);
					gameManager->GetPlayer()->GetPlayerVelocity().x =
						-gameManager->GetPlayer()->GetPlayerVelocity().x * 2.f;
				}
				break;
			case TRAPS_POISON_ARROW_GRID:
				if (!levelTrapsContainer[i].hitPlayer)
				{
					levelTrapsContainer[i].hitPlayer = true;
					TriggerArrow(TRAPS_POISON_ARROW_GRID, levelTrapsContainer[i].obj.pos, levelTrapsContainer[i].trapDamage);
				}
				break;
			case TRAPS_SLOWNESS_ARROW_GRID:
				if (!levelTrapsContainer[i].hitPlayer)
				{
					levelTrapsContainer[i].hitPlayer = true;
					TriggerArrow(TRAPS_SLOWNESS_ARROW_GRID, levelTrapsContainer[i].obj.pos, levelTrapsContainer[i].trapDamage);
				}
				break;
			}
		}
		else
		{
			levelTrapsContainer[i].hitPlayer = false;
		}

		levelTrapsContainer[i].obj.UpdateTransformMatrix();
		levelTrapsContainer[i].UpdateCollisionBox();
	}

	UpdateArrow();
	UpdateFireballs();
}

void DrawTraps(AEGfxVertexList* mesh)
{

	for (size_t i = 0; i < levelTrapsContainer.size(); i++)
	{
		if (levelTrapsContainer[i].obj.pTex == nullptr)
		{
			std::cout << "Ai\n";
			return;
		}
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(levelTrapsContainer[i].obj.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(
			levelTrapsContainer[i].obj.pos.x, levelTrapsContainer[i].obj.pos.y, levelTrapsContainer[i].obj.rotate,
			levelTrapsContainer[i].obj.scale.x, levelTrapsContainer[i].obj.scale.y).m);
		AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
	}

	for (size_t i = 0; i < arrowContainer.size(); i++)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

		if (arrowContainer[i].type == POISON_ARROW)
		{
			AEGfxSetColorToMultiply(1.f, 0.f, 1.f, 1.f);
		}
		else if (arrowContainer[i].type == SLOWNESS_ARROW)
		{
			AEGfxSetColorToMultiply(0.7f, 0.7f, 0.7f, 1.f);
		}
		AEGfxTextureSet(arrowTexture, 0, 0);

		AEGfxSetTransform(ObjectTransformationMatrixSet(
			arrowContainer[i].obj.pos.x, arrowContainer[i].obj.pos.y, 0.f,
			arrowContainer[i].obj.scale.x, arrowContainer[i].obj.scale.y).m);
		AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
		AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f);
	}

	for (size_t i = 0; i < fireBallContainer.size(); i++)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(fireCircleTexture, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(
			fireBallContainer[i].obj.pos.x, fireBallContainer[i].obj.pos.y, fireBallContainer[i].obj.rotate,
			fireBallContainer[i].obj.scale.x, fireBallContainer[i].obj.scale.y).m);
		AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
	}
}

void UnloadTrapsTexture()
{
	levelTrapsContainer.clear();
	arrowContainer.clear();
	fireBallContainer.clear();

	AEGfxTextureUnload(sawTexture);
	AEGfxTextureUnload(arrowTrapTexture);
	AEGfxTextureUnload(fireCircleTexture);
	AEGfxTextureUnload(fireTrapTexture);
	AEGfxTextureUnload(arrowTexture);

}

namespace {
	
#pragma region ArrowTraps
	void TriggerArrow(GridTypes type, AEVec2 trapPos, int damage)
	{
		AEVec2 spawnPos = { trapPos.x + AEGfxGetWindowWidth() * 0.5f, trapPos.y };

		Arrow newArrow;
		newArrow.obj.pos = spawnPos;
		newArrow.velocity = { -20.f, 0.f };  //Towards left only
		newArrow.obj.scale = { GRID_SIZE, GRID_SIZE };
		newArrow.damage = damage;
		newArrow.currLifeTime = newArrow.maxLifeTime;

		switch (type) {
		case TRAPS_POISON_ARROW_GRID:
			newArrow.type = POISON_ARROW;
			break;
		case TRAPS_SLOWNESS_ARROW_GRID:
			newArrow.type = SLOWNESS_ARROW;
			break;
		}
		arrowContainer.push_back(newArrow);
	}

	void UpdateArrow()
	{
		for (size_t i = 0; i < arrowContainer.size(); i++)
		{
			arrowContainer[i].obj.pos.x += arrowContainer[i].velocity.x; //Y shouldn't be changed
			arrowContainer[i].UpdateCollisionBox();
			arrowContainer[i].obj.UpdateTransformMatrix();

			//Check collision
			if (AABBvsAABB(arrowContainer[i].collisionBox, gameManager->GetPlayer()->GetPlayerCollisionBox()))
			{
				gameManager->GetPlayer()->DamageToPlayer(arrowContainer[i].damage);

				ParticleEmit(5, arrowContainer[i].obj.pos.x, arrowContainer[i].obj.pos.y, 5.f, 5.f, 0.f,
					ENEMY_DEATH_EFFECT, nullptr);

				switch (arrowContainer[i].type)
				{
				case POISON_ARROW:
					gameManager->GetPlayer()->SetPlayerPoisoned(true);
					//Inflict status effect
					break;
				case SLOWNESS_ARROW:
					gameManager->GetPlayer()->SetPlayerSlowed(true);

					//Inflict status effect
					break;
				}
				arrowContainer.erase(arrowContainer.begin() + i);
				i--;
			}
			else
			{
				if (arrowContainer[i].currLifeTime > 0.f)
				{
					arrowContainer[i].currLifeTime -= (f32)AEFrameRateControllerGetFrameTime();
				}
				else //Time less than 0.f
				{
					arrowContainer.erase(arrowContainer.begin() + i);
					i--;
				}
			}
		}
	}

	void Arrow::UpdateCollisionBox()
	{
		collisionBox.minimum.x = obj.pos.x - obj.scale.x * 0.5f;
		collisionBox.minimum.y = obj.pos.y - obj.scale.y * 0.5f;
		collisionBox.maximum.x = obj.pos.x + obj.scale.x * 0.5f;
		collisionBox.maximum.y = obj.pos.y + obj.scale.y * 0.5f;
	}


#pragma endregion


	void SpawnFireballs(AEVec2 originPosition, int damage)  //Spawn fireballs INITIALLY based on fire grid position
	{
		for (int i = 0; i < 3; i++)
		{

			FireBalls newFireBall;
			f32 angle = (f32)i * (2.f * PI / 3.f); // Angle for each fireball

			// Calculate positions based on angle
			newFireBall.obj.pos.x = originPosition.x + GRID_SIZE * 2.f * cos(angle);
			newFireBall.obj.pos.y = originPosition.y + GRID_SIZE * 2.f * sin(angle);
			newFireBall.obj.scale = { GRID_SIZE, GRID_SIZE };
			newFireBall.damage = damage;

			newFireBall.originPoint = originPosition;

			fireBallContainer.push_back(newFireBall);
		}
	}

	void UpdateFireballs()
	{
		f32 angle = 0.1f; // Adjust rotation speed as needed

		for (int i = 0; i < fireBallContainer.size(); i++)
		{
			f32 relativeX = fireBallContainer[i].obj.pos.x - fireBallContainer[i].originPoint.x;
			f32 relativeY = fireBallContainer[i].obj.pos.y - fireBallContainer[i].originPoint.y;

			f32 newX = relativeX * cos(angle) - relativeY * sin(angle);
			f32 newY = relativeX * sin(angle) + relativeY * cos(angle);

			fireBallContainer[i].obj.pos.x = newX + fireBallContainer[i].originPoint.x;
			fireBallContainer[i].obj.pos.y = newY + fireBallContainer[i].originPoint.y;

			if (AABBvsAABB(fireBallContainer[i].collisionBox, gameManager->GetPlayer()->GetPlayerCollisionBox()))
			{
				if (!fireBallContainer[i].hitPlayer)
				{
					fireBallContainer[i].hitPlayer = true;
					gameManager->GetPlayer()->DamageToPlayer(fireBallContainer[i].damage);
					gameManager->GetPlayer()->GetPlayerVelocity().x = -gameManager->GetPlayer()->GetPlayerVelocity().x * 2.f;
					gameManager->GetPlayer()->GetPlayerVelocity().y = -gameManager->GetPlayer()->GetPlayerVelocity().y;
				}
			}
			else
			{
				fireBallContainer[i].hitPlayer = false;
			}
			// Update transform matrix and collision box if needed
			fireBallContainer[i].obj.UpdateTransformMatrix();
			fireBallContainer[i].UpdateCollisionBox();
		}
	}


	void FireBalls::UpdateCollisionBox()
	{
		collisionBox.minimum.x = obj.pos.x - obj.scale.x * 0.5f;
		collisionBox.minimum.y = obj.pos.y - obj.scale.y * 0.5f;
		collisionBox.maximum.x = obj.pos.x + obj.scale.x * 0.5f;
		collisionBox.maximum.y = obj.pos.y + obj.scale.y * 0.5f;
	}
}