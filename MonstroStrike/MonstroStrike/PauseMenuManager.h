#pragma once
#include "Camera.h"

enum CurrentScene {
	MainScene,
	PauseScene,
	ControlScene,
	QuitScene
};

enum Interactable {
	Resume,
	Return,
	Controls,
	GoMainMenu
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