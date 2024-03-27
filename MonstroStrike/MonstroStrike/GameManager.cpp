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
}

GameManager::~GameManager()
{

}

void GameManager::Render()
{
	// Debugger UI Entry Point
}

void GameManager::Update()
{

}

Player* GameManager::GetPlayer()
{
	return player;
}