#include "MapTraps.h"
#include <iostream>
#include "main.h"

namespace {
	std::vector<TrapDetails> levelTrapsContainer;

	AEGfxTexture* sawTexture;
	AEGfxTexture* arrowTrapTexture;
	AEGfxTexture* fireCircleTexture;
	AEGfxTexture* arrowTexture;

	int numberOfFireBalls = 3;

	void TriggerArrow(GridTypes type, AEVec2 trapPos, f32 damage);

	void UpdateArrow();

	enum ArrowType {
		POISON_ARROW,
		SLOWNESS_ARROW
	};

	struct Arrow {
		Object obj;
		AABB collisionBox;
		AEVec2 velocity;
		f32 currLifeTime = 5.f;
		f32 maxLifeTime = 5.f;
		f32 damage = 0.f;

		ArrowType type;

		void UpdateCollisionBox();
	};

	std::vector<Arrow> arrowContainer;

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
	sawTexture = AEGfxTextureLoad("Assets/RedCircle.png");
	
	arrowTrapTexture = AEGfxTextureLoad("Assets/RedCircle.png");

	fireCircleTexture = AEGfxTextureLoad("Assets/RedCircle.png");

	arrowTexture = AEGfxTextureLoad("Assets/PlanetTexture.png");
}

void StoreTrapDetails(Grids2D theGrid)
{
	TrapDetails theTrap;

	theTrap.typeOfGrid = theGrid.typeOfGrid;
	if (!SetSpecificTrapDetail(&theTrap))
		return;

	theTrap.obj.pos = theGrid.position;
	theTrap.obj.scale = theGrid.size;
	theTrap.obj.speed = { 0 };
	theTrap.hitPlayer = false;

	//Create the initial collision box
	theTrap.collisionBox.minimum.x = theTrap.obj.pos.x - theTrap.obj.scale.x * 0.5f;
	theTrap.collisionBox.minimum.y = theTrap.obj.pos.y - theTrap.obj.scale.y * 0.5f;
	theTrap.collisionBox.maximum.x = theTrap.obj.pos.x + theTrap.obj.scale.x * 0.5f;
	theTrap.collisionBox.maximum.y = theTrap.obj.pos.y + theTrap.obj.scale.y * 0.5f;

	levelTrapsContainer.push_back(theTrap);
}

bool SetSpecificTrapDetail(TrapDetails* trap)
{
	switch (trap->typeOfGrid)
	{
	case TRAPS_ROTATING_SAW_GRID:
		trap->obj.pTex = sawTexture;
		trap->trapDamage = 10.f;
		return true;
		break;
	case TRAPS_POISON_ARROW_GRID:
		trap->obj.pTex = arrowTrapTexture; //To be multiply color to purple
		trap->trapDamage = 5.f;
		return true;
		break;
	case TRAPS_SLOWNESS_ARROW_GRID:
		trap->obj.pTex = arrowTrapTexture; //To be multiply color to grey
		trap->trapDamage = 5.f;
		return true;
		break;
	case TRAPS_FIRE_CIRCLE_GRID:
		trap->obj.pTex = fireCircleTexture;
		trap->trapDamage = 15.f;
		return true;
		break;
	}

	return false;
}

void UpdateTraps() 
{
	for (size_t i = 0; i < levelTrapsContainer.size(); i++)
	{
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
					gameManager->GetPlayer()->GetCurrentHealth() -= levelTrapsContainer[i].trapDamage;
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
			case TRAPS_FIRE_CIRCLE_GRID: //Only the initial position for this one so need to spawn the fireball later
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
}

void DrawTraps(AEGfxVertexList* mesh)
{
	for (size_t i = 0; i < levelTrapsContainer.size(); i++)
	{
		if (levelTrapsContainer[i].obj.pTex == nullptr)
		{
			return;
		}
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(levelTrapsContainer[i].obj.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(
			levelTrapsContainer[i].obj.pos.x, levelTrapsContainer[i].obj.pos.y, 0.f, 
			levelTrapsContainer[i].obj.scale.x, levelTrapsContainer[i].obj.scale.y).m);
		AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
	}

	for (size_t i = 0; i < arrowContainer.size(); i++)
	{
		if (arrowContainer[i].obj.pTex == nullptr)
		{
			return;
		}
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(arrowTexture, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(
			arrowContainer[i].obj.pos.x, arrowContainer[i].obj.pos.y, 0.f,
			arrowContainer[i].obj.scale.x, arrowContainer[i].obj.scale.y).m);
		AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
	}
}

void UnloadTrapsTexture()
{
	levelTrapsContainer.clear();
	arrowContainer.clear();

	AEGfxTextureUnload(sawTexture);
	AEGfxTextureUnload(arrowTrapTexture);
	AEGfxTextureUnload(fireCircleTexture);
	AEGfxTextureUnload(arrowTexture);

}

namespace {
	void TriggerArrow(GridTypes type, AEVec2 trapPos, f32 damage)
	{
		AEVec2 spawnPos = { trapPos.x + AEGfxGetWindowWidth() * 0.5f, trapPos.y };
		
		Arrow newArrow;
		newArrow.obj.pos = spawnPos;
		newArrow.velocity = { -10.f, 0.f };  //Towards left
		newArrow.obj.scale = { GRID_SIZE * 0.25f, GRID_SIZE * 0.25f };
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
				gameManager->GetPlayer()->GetCurrentHealth() -= arrowContainer[i].damage;
				switch (arrowContainer[i].type) 
				{
				case POISON_ARROW:
					std::cout << "Poison Arrow Hit\n";
					//Inflict status effect
					break;
				case SLOWNESS_ARROW:
					std::cout << "Slowness Arrow Hit\n";
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
}