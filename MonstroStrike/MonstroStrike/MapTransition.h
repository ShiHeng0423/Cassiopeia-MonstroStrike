#pragma once
#include <AEEngine.h>

enum TransitionState {

	TRANSITION_ENTER = 0,

	TRANSITION_UPDATE,

	TRANSITION_EXIT
};

void MapTransitionLoad();

void MapTransitionUpdate();

void MapTransitionUnload();