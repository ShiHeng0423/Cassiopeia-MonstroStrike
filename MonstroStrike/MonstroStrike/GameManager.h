#pragma once

#include "Player.h"

//game manager
//hold player data
//inventory


class GameManager {

public:
	GameManager();
	~GameManager();

	void Update();
	
	//Update/Get Player Stats etc.
	Player* GetPlayer();

	void Render();

private:
	Player* player;

};