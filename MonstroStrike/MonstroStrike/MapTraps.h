#pragma once
#include <AEEngine.h>
#include <AEMath.h>
#include <vector>
#include "Utils.h"
#include "CSVMapLoader.h"
#include "GridTypesList.h"

namespace {

	struct TrapDetails {
		Object obj; //Position, sprite, scale etc...
		GridTypes typeOfGrid = NONE; //To check what type of trap since it is recorded here
		bool hitPlayer = false;
		bool triggered = false;
		int trapDamage = 0;
		AABB collisionBox;

		void UpdateCollisionBox();
	};
}

void PreLoadTrapsTexture();

void UnloadTrapsTexture();

//Set the texture loaded previously 
bool SetSpecificTrapDetail(TrapDetails* trap);

//Increase number of traps in the level, what type it is based on grid type symbol, and position
void StoreTrapDetails(Grids2D theGrids);

void UpdateTraps(); //Update traps based on vector

void DrawTraps(AEGfxVertexList* mesh); //Update traps based on vector