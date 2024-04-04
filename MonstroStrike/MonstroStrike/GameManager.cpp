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