#pragma once
/*!************************************************************************
  \file					MapTransition.h
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (100%)
  \brief				File containing declarations of functions and structures for map transition effects.

  This file contains declarations of functions and structures essential for managing map transition effects providing visual effects
  when moving between different game levels or areas. The header defines structures for representing transition image objects and
  enumerates different transition directions, along with functions for loading, initializing, updating, drawing, and unloading map 
  transition effects.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#include <AEEngine.h>
#include "GameStateManager.h"

enum TransitionDirection {

	TRANSITION_UP = 0,
	
	TRANSITION_DOWN,
	
	TRANSITION_RIGHT,

	TRANSITION_LEFT
};

//Private namespace
namespace {
	enum TransitionState {

		TRANSITION_ENTER = 0,

		TRANSITION_UPDATE,

		TRANSITION_EXIT
	};
}

struct TransitionImageObj {

	AEVec2 position;
	AEVec2 destPosition;
	AEVec2 velocity;

	AEVec2 size;

	f32 animationSpeed;

	AEMtx33 translation;
	AEMtx33 rotation;
	AEMtx33 scale;
	AEMtx33 transformation;
	AEGfxTexture* transitionImage;
	AEGfxVertexList* pMesh;

	void PlayMapTransition(TransitionDirection directionToPlay, GameStates nextLevel);

	bool active;
	bool enteringLevel;
};

void MapTransitionLoad();

void MapTransitionInit();

void MapTransitionUpdate();

void MapTransitionUnload();

void MapTransitionDraw();

extern struct TransitionImageObj transitionalImageOBJ;