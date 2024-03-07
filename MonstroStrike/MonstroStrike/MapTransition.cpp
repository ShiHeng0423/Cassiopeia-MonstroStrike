#include "MapTransition.h"
#include <iostream>

//Global
struct TransitionImageObj transitionalImageOBJ;

//Private to only this
namespace {

	TransitionDirection transitionDir;
	TransitionDirection prevTransitionDir;

	TransitionState currentTransitState;

	GameStates nextGameState;
	AEGfxTexture* transitionImage;
}

void MapTransitionLoad() {

	transitionImage = AEGfxTextureLoad("Assets/TransitionImage.png");
}

void MapTransitionInit() //Call when enter a new level
{
	transitionalImageOBJ.animationSpeed = 10.f;
	transitionalImageOBJ.velocity = { 0 }; //No Veloicity
	transitionalImageOBJ.active = false; //False by default, play animation function will set it true later
	transitionalImageOBJ.position = { 0 }; //Set as 0,0 first but should not be visible
	transitionalImageOBJ.destPosition = { 0 }; //Final destination for the player to move against
	transitionalImageOBJ.translation = { 0 };
	transitionalImageOBJ.rotation = { 0 }; //Why I even have this?
	transitionalImageOBJ.size = { (f32)AEGfxGetWindowWidth(), (f32)AEGfxGetWindowHeight() }; //Set both to fit window
	transitionalImageOBJ.scale = { 0 }; //Set scale as size
	transitionalImageOBJ.transformation = { 0 };
	
	nextGameState = GameStates::GAMESTATE_NONE; //Should always be none when entering the level

	switch (prevTransitionDir)
	{
	case TRANSITION_DOWN: //When player is moving upward, play transition down previously
		transitionDir = TRANSITION_UP; //Enter new level, play up now as player is moving upward
		break;
	case TRANSITION_UP: //When player is moving downward, play transition up previously
		transitionDir = TRANSITION_DOWN; //Play transition below as enter new level while player falls
		break;
	case TRANSITION_LEFT: //When player moving towards right side, play transition left previously
		transitionDir = TRANSITION_RIGHT; //Play transition right as enter new level
		break;
	case TRANSITION_RIGHT: //When player moving towards left side, play transition right previously
		transitionDir = TRANSITION_LEFT;
		break;
	default:
		std::cerr << "No Transition when enter level?\n";
		break;
	}

	transitionalImageOBJ.PlayMapTransition(transitionDir, nextGameState); //Play the newly set animation here
}

void TransitionImageObj::PlayMapTransition(TransitionDirection directionToPlay, GameStates nextLevel) //To call when exiting a level
{
	transitionDir = directionToPlay;
	transitionalImageOBJ.active = true;
	nextGameState = nextLevel;
	currentTransitState = TRANSITION_ENTER;
}

void MapTransitionUpdate() //Update only when transition image is active
{
	//Note to future self: Initial position need to get via min max XY of the entire map size
	//Either that or get just outside camera view *Recommended
	switch (currentTransitState)
	{
	case TRANSITION_ENTER:
		switch (transitionDir)
		{
		case TRANSITION_UP: //Play up animation
			transitionalImageOBJ.velocity.y = transitionalImageOBJ.animationSpeed;
			//Set initial starting position for this animation 
			
			//Set destination position for this animation
			break;
		case TRANSITION_DOWN: //Play down animation
			transitionalImageOBJ.velocity.y = -transitionalImageOBJ.animationSpeed;
			//Set initial starting position for this animation 

			//Set destination position for this animation
			break;
		case TRANSITION_LEFT: //Play left animation
			transitionalImageOBJ.velocity.x = -transitionalImageOBJ.animationSpeed;
			//Set initial starting position for this animation 

			//Set destination position for this animation
			break;
		case TRANSITION_RIGHT: //Play right animation
			transitionalImageOBJ.velocity.x = transitionalImageOBJ.animationSpeed;
			//Set initial starting position for this animation 

			//Set destination position for this animation
			break;
		default:
			std::cerr << "Cannot find animation to play\n";
			break;
		}

		currentTransitState = TRANSITION_UPDATE;
		break;
	case TRANSITION_UPDATE:

		if (AEVec2Distance(&transitionalImageOBJ.position, &transitionalImageOBJ.destPosition) > 0.f)
		{
			//Update velocity
			AEVec2Scale(&transitionalImageOBJ.velocity, &transitionalImageOBJ.velocity, (f32)AEFrameRateControllerGetFrameTime());

			//Update position
			AEVec2Add(&transitionalImageOBJ.position, &transitionalImageOBJ.position, &transitionalImageOBJ.velocity);

			//Update Scale
			AEMtx33Scale(&transitionalImageOBJ.scale, transitionalImageOBJ.size.x, transitionalImageOBJ.size.y);
			//Update rotation
			AEMtx33Rot(&transitionalImageOBJ.rotation, 0);
			//Update translation
			AEMtx33Trans(&transitionalImageOBJ.translation, transitionalImageOBJ.position.x, transitionalImageOBJ.position.y);

			//Concat the matrices
			AEMtx33Concat(&transitionalImageOBJ.transformation, &transitionalImageOBJ.scale, &transitionalImageOBJ.rotation);
			AEMtx33Concat(&transitionalImageOBJ.transformation, &transitionalImageOBJ.translation, &transitionalImageOBJ.transformation);

			//Set the transform if no work here then put in draw
			AEGfxSetTransform(transitionalImageOBJ.transformation.m);
		}
		else //Reached destination
		{
			transitionalImageOBJ.velocity = { 0 }; //Pause instance of moving the object
			if (nextGameState != GameStates::GAMESTATE_NONE)
			{
				next = nextGameState; //Go next game state as long as it is not a none
			}
			currentTransitState = TRANSITION_EXIT;
		}
		break;
	case TRANSITION_EXIT:
		transitionalImageOBJ.active = false; //Shouldn't be outside to the player view at this point
		break;
	}
}

void MapTransitionDraw(AEGfxVertexList* pMesh) //Always the last layer so that it covers EVERYTHING
{
	if (transitionalImageOBJ.active) //Only draw when it is active
	{
		//Draw the Map via the transformations
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE); //Set it here as well to confirm
		AEGfxTextureSet(transitionImage, 0, 0);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
}


void MapTransitionUnload()
{
	AEGfxTextureUnload(transitionImage);
}