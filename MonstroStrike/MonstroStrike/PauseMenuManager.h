/*!************************************************************************
  \file					PauseMenuManager.h
  \project name			Monstrostrike
  \primary author		Teng Shi heng (100%)
  \brief				This file contains the declarations of class 
						PauseMenu_Manager and the button triggered 
						functions when the user presses the buttons in the
						pause menu when the user pauses the game.

						There area 2 Enumerator defined where the "CurrentScene"
						enum tracks which page of the interface of the main menu
						that the user is currently interacting to and "Interactable"
						refers to the different types of interactable buttons
						available int the pause menu.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#pragma once
#include "Camera.h"

enum CurrentScene {
	MAIN_SCENE,
	PAUSE_SCENE,
	CONTROL_SCENE,
	QUIT_SCENE
};

enum Interactable {
	RESUME,
	RETURN,
	CONTROLS,
	GO_MAINMENU
};

void ResumeGame();
void ReturnLobby();
void OpenControls();
void QuitMainmenu();
void QuitConfirmation();
void BackPauseMenu();

extern int currScene;

class PauseMenu_Manager
{
public:

	PauseMenu_Manager();
	~PauseMenu_Manager();

	void Init(Camera* cam);
	void Update();
	void Render();
};