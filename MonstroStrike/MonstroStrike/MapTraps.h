#pragma once
/*!************************************************************************
  \file					MapTraps.h
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (100%)
  \brief				File containing declarations of functions and structures for managing map traps.

  This file contains declarations of functions and structures essential for managing map traps.
  The header defines a structure for representing trap details, along with functions for loading trap textures, 
  storing trap information, updating traps, and drawing traps on the screen.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/
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