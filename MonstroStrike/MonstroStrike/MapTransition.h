#pragma once
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

	void PlayMapTransition(TransitionDirection directionToPlay, GameStates nextLevel);

	bool active;
};

void MapTransitionLoad();

void MapTransitionInit();

void MapTransitionUpdate(GameStates nextGameState);

void MapTransitionUnload();

void MapTransitionDraw(AEGfxVertexList* pMesh);

extern struct TransitionImageObj transitionalImageOBJ;