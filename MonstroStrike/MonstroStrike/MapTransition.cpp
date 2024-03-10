#include "MapTransition.h"
#include <iostream>
#include "Player.h"

//Global
struct TransitionImageObj transitionalImageOBJ;

//Private to only this
namespace {

	static TransitionDirection transitionDir;
	static TransitionDirection prevTransitionDir = TRANSITION_DOWN;

	TransitionState currentTransitState;

	GameStates nextGameState;
}

void MapTransitionLoad() {

	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // top-left: blue

	transitionalImageOBJ.pMesh = AEGfxMeshEnd();

	transitionalImageOBJ.transitionImage = AEGfxTextureLoad("Assets/TransitionalScreen.png");
}

void MapTransitionInit(AEVec2 playerPos) //Call when enter a new level
{
	transitionalImageOBJ.animationSpeed = 1500.f;
	transitionalImageOBJ.velocity = { 0 }; //No Veloicity
	transitionalImageOBJ.active = true; //False by default, play animation function will set it true later
	transitionalImageOBJ.position = { playerPos.x, playerPos.y }; //Set as 0,0 first but should not be visible
	transitionalImageOBJ.destPosition = { 0 }; //Final destination for the player to move against
	transitionalImageOBJ.translation = { 0 };
	transitionalImageOBJ.rotation = { 0 }; //Why I even have this?
	transitionalImageOBJ.size = { (f32)AEGfxGetWindowWidth() * 1.75f, (f32)AEGfxGetWindowHeight() * 1.75f}; //Set both to fit window
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

	transitionalImageOBJ.PlayMapTransition(prevTransitionDir, GameStates::GAMESTATE_NONE);
}

void TransitionImageObj::PlayMapTransition(TransitionDirection directionToPlay, GameStates nextLevel) //To call when exiting a level
{
	//std::cout << "Play Animation\n";

	transitionDir = directionToPlay;
	transitionalImageOBJ.active = true;
	nextGameState = nextLevel;
	currentTransitState = TRANSITION_ENTER;
}

void MapTransitionUpdate(AEVec2 playerPos) //Update only when transition image is active
{
	//Note to future self: Initial position need to get via min max XY of the entire map size
	//Either that or get just outside camera view *Recommended
	if (!transitionalImageOBJ.active)
	{
		//std::cout << "Inactive obj\n";
		return;
	}

	switch (currentTransitState)
	{
	case TRANSITION_ENTER:

		if (nextGameState == GameStates::GAMESTATE_NONE) //Means just enter level
		{
			//transitionalImageOBJ.position = { playerPos.x , playerPos.y }; //Initial should be at player pos

			switch (transitionDir)
			{
			case TRANSITION_UP: //Play up animation
				transitionalImageOBJ.velocity.y = transitionalImageOBJ.animationSpeed;
				//Set destination position for this animation
				transitionalImageOBJ.destPosition = { playerPos.x, playerPos.y + AEGfxGetWindowHeight() * 1.5f};
				break;
			case TRANSITION_DOWN: //Play down animation
				transitionalImageOBJ.velocity.y = -transitionalImageOBJ.animationSpeed;
				//Set destination position for this animation
				transitionalImageOBJ.destPosition = { playerPos.x, playerPos.y - AEGfxGetWindowHeight() * 1.5f };
				break;
			case TRANSITION_LEFT: //Play left animation
				transitionalImageOBJ.velocity.x = -transitionalImageOBJ.animationSpeed;
				//Set destination position for this animation
				transitionalImageOBJ.destPosition = { playerPos.x - AEGfxGetWindowWidth() * 1.5f, playerPos.y };
				break;
			case TRANSITION_RIGHT: //Play right animation
				transitionalImageOBJ.velocity.x = transitionalImageOBJ.animationSpeed;
				//Set destination position for this animation
				transitionalImageOBJ.destPosition = { playerPos.x + AEGfxGetWindowWidth() * 1.5f, playerPos.y };
				break;
			default:
				std::cerr << "Cannot find animation to play\n";
				break;
			}
		}
		else //Exiting level
		{
			switch (transitionDir)
			{
			case TRANSITION_UP: //Play up animation
				transitionalImageOBJ.velocity.y = transitionalImageOBJ.animationSpeed;
				//Set initial starting position for this animation 
				transitionalImageOBJ.position = { playerPos.x , playerPos.y - AEGfxGetWindowHeight() };
				break;
			case TRANSITION_DOWN: //Play down animation
				transitionalImageOBJ.velocity.y = -transitionalImageOBJ.animationSpeed;
				//Set initial starting position for this animation 
				transitionalImageOBJ.position = { playerPos.x , playerPos.y + AEGfxGetWindowHeight() };
				break;
			case TRANSITION_LEFT: //Play left animation
				transitionalImageOBJ.velocity.x = -transitionalImageOBJ.animationSpeed;
				//Set initial starting position for this animation 
				transitionalImageOBJ.position = { playerPos.x + AEGfxGetWindowWidth(), playerPos.y };
				break;
			case TRANSITION_RIGHT: //Play right animation
				transitionalImageOBJ.velocity.x = transitionalImageOBJ.animationSpeed;
				//Set initial starting position for this animation 
				transitionalImageOBJ.position = { playerPos.x - AEGfxGetWindowWidth(), playerPos.y };
				break;
			default:
				std::cerr << "Cannot find animation to play\n";
				break;
			}
			transitionalImageOBJ.destPosition = { playerPos.x, playerPos.y };
		}
		currentTransitState = TRANSITION_UPDATE;
		break;
	case TRANSITION_UPDATE:

		//std::cout << AEVec2Distance(&transitionalImageOBJ.position, &transitionalImageOBJ.destPosition) << std::endl;
		if (AEVec2Distance(&transitionalImageOBJ.position, &transitionalImageOBJ.destPosition) > 5.f)
		{
			//std::cout << transitionalImageOBJ.position.x <<  " " << transitionalImageOBJ.position.y << std::endl;
			//std::cout << transitionalImageOBJ.velocity.x << " " << transitionalImageOBJ.velocity.y << std::endl;
			AEVec2 finalVelocity;
			//Update velocity
			AEVec2Scale(&finalVelocity, &transitionalImageOBJ.velocity, (f32)AEFrameRateControllerGetFrameTime());

			//Update position
			AEVec2Add(&transitionalImageOBJ.position, &transitionalImageOBJ.position, &finalVelocity);

			//Update Scale
			AEMtx33Scale(&transitionalImageOBJ.scale, transitionalImageOBJ.size.x, transitionalImageOBJ.size.y);
			//Update rotation
			AEMtx33Rot(&transitionalImageOBJ.rotation, 0);
			//Update translation
			AEMtx33Trans(&transitionalImageOBJ.translation, transitionalImageOBJ.position.x, transitionalImageOBJ.position.y);

			//Concat the matrices
			AEMtx33Concat(&transitionalImageOBJ.transformation, &transitionalImageOBJ.scale, &transitionalImageOBJ.rotation);
			AEMtx33Concat(&transitionalImageOBJ.transformation, &transitionalImageOBJ.translation, &transitionalImageOBJ.transformation);
		}
		else //Reached destination
		{
			if (nextGameState != GameStates::GAMESTATE_NONE)
			{
				prevTransitionDir = transitionDir;
				next = nextGameState; //Go next game state as long as it is not a none
			}
			transitionalImageOBJ.velocity = { 0 }; //Pause instance of moving the object
			currentTransitState = TRANSITION_EXIT;
		}
		break;
	case TRANSITION_EXIT:
		transitionalImageOBJ.active = false; //Shouldn't be outside to the player view at this point
		break;
	}
}

void MapTransitionDraw() //Always the last layer so that it covers EVERYTHING
{
	if (transitionalImageOBJ.active) //Only draw when it is active
	{
		//Draw the Map via the transformations
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE); //Set it here as well to confirm
		AEGfxTextureSet(transitionalImageOBJ.transitionImage, 0, 0);
		AEGfxSetTransform(transitionalImageOBJ.transformation.m);
		AEGfxMeshDraw(transitionalImageOBJ.pMesh, AE_GFX_MDM_TRIANGLES);
	}
}


void MapTransitionUnload()
{
	AEGfxMeshFree(transitionalImageOBJ.pMesh);
	AEGfxTextureUnload(transitionalImageOBJ.transitionImage);
}