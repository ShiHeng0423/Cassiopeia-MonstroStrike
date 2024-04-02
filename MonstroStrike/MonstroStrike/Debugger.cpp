#include "DebuggerManager.h"
#include "Utils.h"
#include "main.h"
#include <sstream>

namespace
{
	AEGfxVertexList* quad;
}

DebuggerManager::DebuggerManager()
{
	quad = GenerateSquareMesh(0xFFFFFFFF);
	playerInfo = gameManager->GetPlayer();
}

DebuggerManager::~DebuggerManager()
{
	AEGfxMeshFree(quad);
}

void DebuggerManager::RenderDebuggerUI()
{
	//Game FPS
	std::stringstream strFPS;
	strFPS << "FPS: " << AEFrameRateControllerGetFrameRate();
	f32 width, height;
	AEGfxGetPrintSize(fontID, strFPS.str().c_str(), 0.3f, &width, &height);
	AEGfxPrint(fontID, strFPS.str().c_str(), -width / 2 - 0.8, -height / 2 + 0.95f, 0.3f, 0, 0, 0, 1);

	//Player Position
	std::stringstream str;
	str << "Player Pos:" << playerInfo->GetPlayerCurrentPosition().x << playerInfo->GetPlayerCurrentPosition().y;
	AEGfxGetPrintSize(fontID, str.str().c_str(), 0.3f, &width, &height);
	AEGfxPrint(fontID, str.str().c_str(), -width / 2 - 0.8, -height / 2 + 0.95f, 0.3f, 0, 0, 0, 1);

	//Player HP Invincible
	std::string strMode = "Player HP Invincible: True";
	AEGfxGetPrintSize(fontID, strMode.c_str(), 0.3f, &width, &height);
	AEGfxPrint(fontID, strMode.c_str(), -width / 2 - 0.8, -height / 2 + 0.95f, 0.3f, 0, 0, 0, 1);

	//Player ATK Invincible
	strMode = "Player Max Attack: True";
	AEGfxGetPrintSize(fontID, str.str().c_str(), 0.3f, &width, &height);
	AEGfxPrint(fontID, str.str().c_str(), -width / 2 - 0.8, -height / 2 + 0.95f, 0.3f, 0, 0, 0, 1);
}