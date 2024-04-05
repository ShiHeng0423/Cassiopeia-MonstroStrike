/*!************************************************************************
  \file					GameManager.cpp
  \project name			Monstrostrike
  \primary author		Teng Shi heng (100%)
  \brief				File containing the definitions of functions declared
						GameManager.h files.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#include "GameManager.h"
#include "AEEngine.h"
#include "main.h"

//private namespace that the variables will only be used in this file
namespace 
{
	AEGfxVertexList* pLineMesh;
	AEGfxVertexList* pWhiteSquareMesh;
	AEGfxVertexList* pMeshRed;
}

GameManager::GameManager()
{
	player = new Player({ 0.f, 0.f }, { 0.f, 0.f }, { 40.f, 0.f }, true);
	debug = new DebuggerManager(player);
	debuggerModeOn = false;
}

GameManager::~GameManager()
{
	delete player;
	delete debug;
}

void GameManager::Render()
{
	// Debugger UI Entry Point
	debug->RenderDebuggerUI();
}

void GameManager::Update()
{
	debug->Update();
}

Player* GameManager::GetPlayer()
{
	return player;
}

DebuggerManager* GameManager::getDebugManager()
{
	return debug;
}
