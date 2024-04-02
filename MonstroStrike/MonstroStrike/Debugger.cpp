#include "DebuggerManager.h"
#include "Utils.h"
#include "main.h"
#include <sstream>
#include <vector>

namespace
{
	AEGfxVertexList* quad;
	ButtonUI displayFPS;
	ButtonUI displayPosition;
	ButtonUI playerImmortal;
	ButtonUI playerPowerMax;

	bool debugMsg[4]{ false };
}

void ShowFPS()
{
	if (displayFPS.str == "Off")
	{
		displayFPS.str = "On";
		debugMsg[0] = true;
	}
	else
	{
		displayFPS.str = "Off";
		debugMsg[0] = false;
	}
}

void ShowPlayerPosition()
{
	if (displayPosition.str == "Off")
	{
		displayPosition.str = "On";
		debugMsg[1] = true;
	}
	else
	{
		displayPosition.str = "Off";
		debugMsg[1] = false;
	}
}

void TurnOnImmortal()
{
	if (playerImmortal.str == "Off")
	{
		playerImmortal.str = "On";
		debugMsg[2] = true;
	}
	else
	{
		playerImmortal.str = "Off";
		debugMsg[2] = false;
	}
}

void TurnOnFullPower()
{
	if (playerPowerMax.str == "Off")
	{
		playerPowerMax.str = "On";
		debugMsg[3] = true;
	}
	else
	{
		playerPowerMax.str = "Off";
		debugMsg[3] = false;
	}
}

DebuggerManager::DebuggerManager(Player* player)
{
	quad = GenerateSquareMesh(0xFFFFFFFF);
	playerInfo = player;
	openDebugPanel = false;

	displayFPS.str = "Off";
	displayFPS.pos = { AEGfxGetWinMinX() + 175.f, AEGfxGetWinMaxY() - 75.f };
	displayFPS.scale = { 80.f, 40.f };
	displayFPS.UpdateTransformMatrix();
	displayFPS.Ptr = ShowFPS;

	displayPosition.str = "Off";
	displayPosition.pos = { AEGfxGetWinMinX() + 175.f, AEGfxGetWinMaxY() - 135.f };
	displayPosition.scale = { 80.f, 40.f };
	displayPosition.UpdateTransformMatrix();
	displayPosition.Ptr = ShowPlayerPosition;

	playerImmortal.str = "Off";
	playerImmortal.pos = { AEGfxGetWinMinX() + 175.f, AEGfxGetWinMaxY() - 195.f };
	playerImmortal.scale = { 80.f, 40.f };
	playerImmortal.UpdateTransformMatrix();
	playerImmortal.Ptr = TurnOnImmortal;
	
	playerPowerMax.str = "Off";
	playerPowerMax.pos = { AEGfxGetWinMinX() + 175.f, AEGfxGetWinMaxY() - 255.f };
	playerPowerMax.scale = { 80.f, 40.f };
	playerPowerMax.UpdateTransformMatrix();
	playerPowerMax.Ptr = TurnOnFullPower;
}

DebuggerManager::~DebuggerManager()
{
	AEGfxMeshFree(quad);
}

void DebuggerManager::Update()
{
	if (AEInputCheckTriggered(AEVK_0))
	{
		openDebugPanel = true;
	}

	if (openDebugPanel)
	{
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			s32 x, y;
			AEInputGetCursorPosition(&x, &y);
			AEVec2 mousePos{ 0, 0 };
			mousePos.x = x - AEGfxGetWindowWidth() * 0.5f;
			mousePos.y = AEGfxGetWindowHeight() * 0.5f - y;


			if (AETestPointToRect(&mousePos, &displayFPS.pos,
				displayFPS.scale.x, displayFPS.scale.y))
				displayFPS.Ptr();

			if (AETestPointToRect(&mousePos, &displayPosition.pos,
				displayPosition.scale.x, displayPosition.scale.y))
				displayPosition.Ptr();

			if (AETestPointToRect(&mousePos, &playerImmortal.pos,
				playerImmortal.scale.x, playerImmortal.scale.y))
				playerImmortal.Ptr();

			if (AETestPointToRect(&mousePos, &playerPowerMax.pos,
				playerPowerMax.scale.x, playerPowerMax.scale.y))
				playerPowerMax.Ptr();
		}
	}
}

void DebuggerManager::RenderDebuggerUI()
{
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	if (openDebugPanel)
	{
		//AEGfxSetTransform(ObjectTransformationMatrixSet(AEGfxGetWinMinX() + 175.f, AEGfxGetWinMaxY() - 75.f, 0.f, 350.f, 150.f).m);
		//AEGfxMeshDraw(quad, AE_GFX_MDM_TRIANGLES);

		AEGfxSetTransform(displayFPS.transform.m);
		AEGfxMeshDraw(quad, AE_GFX_MDM_TRIANGLES);

		AEGfxSetTransform(displayPosition.transform.m);
		AEGfxMeshDraw(quad, AE_GFX_MDM_TRIANGLES);

		AEGfxSetTransform(playerImmortal.transform.m);
		AEGfxMeshDraw(quad, AE_GFX_MDM_TRIANGLES);

		AEGfxSetTransform(playerPowerMax.transform.m);
		AEGfxMeshDraw(quad, AE_GFX_MDM_TRIANGLES);

		//Game FPS
		f32 width, height;
		AEGfxGetPrintSize(fontID, displayFPS.str.c_str(), 0.3f, &width, &height);
		AEGfxPrint(fontID, displayFPS.str.c_str(), -0.81, -height / 2 + 0.83f, 0.3f, 0, 0, 0, 1);

		//Player Position
		AEGfxGetPrintSize(fontID, displayPosition.str.c_str(), 0.3f, &width, &height);
		AEGfxPrint(fontID, displayPosition.str.c_str(), -0.81, -height / 2 + 0.7f, 0.3f, 0, 0, 0, 1);

		//Player HP Invincible
		AEGfxGetPrintSize(fontID, playerImmortal.str.c_str(), 0.3f, &width, &height);
		AEGfxPrint(fontID, playerImmortal.str.c_str(), -0.81, -height / 2 + 0.57f, 0.3f, 0, 0, 0, 1);

		//Player ATK Invincible
		AEGfxGetPrintSize(fontID, playerPowerMax.str.c_str(), 0.3f, &width, &height);
		AEGfxPrint(fontID, playerPowerMax.str.c_str(), -0.81, -height / 2 + 0.43f, 0.3f, 0, 0, 0, 1);
	}

	AEGfxSetTransform(ObjectTransformationMatrixSet(AEGfxGetWinMaxX() - 175.f, AEGfxGetWinMaxY() - 75.f, 0.f, 350.f, 150.f).m);
	AEGfxMeshDraw(quad, AE_GFX_MDM_TRIANGLES);

	size_t activeDebug = 0;
	for (size_t index = 0; index < (sizeof(debugMsg) / sizeof(bool)); index++)
	{
		if (!debugMsg[index])
			continue;

		f32 width, height;
		std::stringstream str;

		switch (index)
		{
		case 0:
			str << "FPS: " << AEFrameRateControllerGetFrameRate();
			break;
		case 1:
			str << "Player Pos:" << (int)playerInfo->GetPlayerCurrentPosition().x << "," << (int)playerInfo->GetPlayerCurrentPosition().y;
			break;
		case 2:
			str << "Player Immortal: True";
			break;
		case 3:
			str << "Player Max Attack: True";
			break;
		default:
			break;
		}

		AEGfxGetPrintSize(fontID, str.str().c_str(), 0.3f, &width, &height);
		AEGfxPrint(fontID, str.str().c_str(), 0.6, -height / 2 + 0.95f - activeDebug++ * 0.05f, 0.3f, 0, 0, 0, 1);
	}
}