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

void MapTransitionInit() //Call when enter a new level
{
	AEVec2 transitInPos;
	AEGfxGetCamPosition(&transitInPos.x, &transitInPos.y);
	std::cout << transitInPos.x << " " << transitInPos.y << std::endl;

	transitionalImageOBJ.animationSpeed = 2500.f;
	transitionalImageOBJ.velocity = { 0 }; //No Veloicity
	transitionalImageOBJ.active = false; //False by default, play animation function will set it true later
	transitionalImageOBJ.position = { transitInPos.x, transitInPos.y }; //Set as player pos first but should not be visible
	transitionalImageOBJ.destPosition = { 0 }; //Final destination for the player to move against
	transitionalImageOBJ.translation = { 0 };
	transitionalImageOBJ.rotation = { 0 }; //Why I even have this?
	transitionalImageOBJ.size = {(f32)AEGfxGetWindowWidth() * 2.25f, (f32)AEGfxGetWindowHeight() * 2.25f };
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

	transitionalImageOBJ.PlayMapTransition(transitionDir, nextGameState);
}

void TransitionImageObj::PlayMapTransition(TransitionDirection directionToPlay, GameStates nextLevel) //To call when exiting a level
{
	std::cout << "Play Animation\n";

	transitionDir = directionToPlay;
	transitionalImageOBJ.active = true;
	nextGameState = nextLevel;
	currentTransitState = TRANSITION_ENTER;
}

void MapTransitionUpdate() //Update only when transition image is active
{
	if (!transitionalImageOBJ.active)
	{
		return;
	}

	switch (currentTransitState)
	{
	case TRANSITION_ENTER:
		AEVec2 transitPos;
		AEGfxGetCamPosition(&transitPos.x, &transitPos.y);

		if (nextGameState == GameStates::GAMESTATE_NONE) //Means just enter level
		{
			switch (transitionDir)
			{
			case TRANSITION_UP: //Play up animation
				transitionalImageOBJ.velocity.y = transitionalImageOBJ.animationSpeed;
				//Set destination position for this animation
				transitionalImageOBJ.destPosition = { transitPos.x, transitPos.y + AEGfxGetWindowHeight() * 1.75f };
				break;
			case TRANSITION_DOWN: //Play down animation
				transitionalImageOBJ.velocity.y = -transitionalImageOBJ.animationSpeed;
				//Set destination position for this animation
				transitionalImageOBJ.destPosition = { transitPos.x, transitPos.y - AEGfxGetWindowHeight() * 1.75f };
				break;
			case TRANSITION_LEFT: //Play left animation
				transitionalImageOBJ.velocity.x = -transitionalImageOBJ.animationSpeed;
				//Set destination position for this animation
				transitionalImageOBJ.destPosition = { transitPos.x - AEGfxGetWindowWidth() * 2.f, transitPos.y };
				break;
			case TRANSITION_RIGHT: //Play right animation
				transitionalImageOBJ.velocity.x = transitionalImageOBJ.animationSpeed;
				//Set destination position for this animation
				transitionalImageOBJ.destPosition = { transitPos.x + AEGfxGetWindowWidth() * 2.f, transitPos.y };
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
				transitionalImageOBJ.position = { transitPos.x , transitPos.y - AEGfxGetWindowHeight()};
				transitionalImageOBJ.destPosition = { transitPos.x, transitPos.y };
				break;
			case TRANSITION_DOWN: //Play down animation
				transitionalImageOBJ.velocity.y = -transitionalImageOBJ.animationSpeed;
				//Set initial starting position for this animation 
				transitionalImageOBJ.position = { transitPos.x , transitPos.y + AEGfxGetWindowHeight() };
				break;
			case TRANSITION_LEFT: //Play left animation
				transitionalImageOBJ.velocity.x = -transitionalImageOBJ.animationSpeed;
				//Set initial starting position for this animation 
				transitionalImageOBJ.position = { transitPos.x + AEGfxGetWindowWidth(), transitPos.y };
				transitionalImageOBJ.destPosition = { transitPos.x, transitPos.y};
				break;
			case TRANSITION_RIGHT: //Play right animation
				transitionalImageOBJ.velocity.x = transitionalImageOBJ.animationSpeed;
				//Set initial starting position for this animation 
				transitionalImageOBJ.position = { transitPos.x - AEGfxGetWindowWidth(), transitPos.y };
				transitionalImageOBJ.destPosition = { transitPos.x, transitPos.y };

				break;
			default:
				std::cerr << "Cannot find animation to play\n";
				break;
			}

			//std::cout << transitionalImageOBJ.position.x << " " << transitionalImageOBJ.destPosition.x << "\n";
		}
		currentTransitState = TRANSITION_UPDATE;
		break;
	case TRANSITION_UPDATE:

		//std::cout << AEVec2Distance(&transitionalImageOBJ.position, &transitionalImageOBJ.destPosition) << std::endl;
		if (AEVec2Distance(&transitionalImageOBJ.position, &transitionalImageOBJ.destPosition) > 45.f)
		{
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
			std::cout << "False\n";

			if (nextGameState != GameStates::GAMESTATE_NONE)
			{
				prevTransitionDir = transitionDir;
				std::cout << "Entering game state: " << GameStates::GAME_LOBBY << "\n";
				next = nextGameState; //Go next game state as long as it is not a none
			}
			transitionalImageOBJ.velocity = { 0 }; //Pause instance of moving the object
			currentTransitState = TRANSITION_EXIT;
		}
		break;
	case TRANSITION_EXIT:
		std::cout << "False\n";
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