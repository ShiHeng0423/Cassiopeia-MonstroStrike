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
	void Update(Camera* cam);
	void Render();
};