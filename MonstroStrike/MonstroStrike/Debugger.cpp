#include "DebuggerManager.h"
#include "Utils.h"
#include "GameStateManager.h"
#include "main.h"
#include <sstream>
#include <vector>

namespace
{
	AEGfxVertexList* quad;
	ButtonUI debugFunction[DEBUGGER_TOTAL];

	bool debugMsg[DEBUGGER_TOTAL]{ false };
	int totalDebugFunctionUsed{ 0 };
}

int DebuggerManager::currArea = 0;

void ShowFPS()
{
	if (debugFunction[FPS].str == "Off")
	{
		debugFunction[FPS].str = "On";
		debugMsg[FPS] = true;
		totalDebugFunctionUsed++;
	}
	else
	{
		debugFunction[FPS].str = "Off";
		debugMsg[FPS] = false;
		totalDebugFunctionUsed--;
	}
}

void ShowPlayerPosition()
{
	if (debugFunction[POSITION].str == "Off")
	{
		debugFunction[POSITION].str = "On";
		debugMsg[POSITION] = true;
		totalDebugFunctionUsed++;
	}
	else
	{
		debugFunction[POSITION].str = "Off";
		debugMsg[POSITION] = false;
		totalDebugFunctionUsed--;
	}
}

void TurnOnImmortal()
{
	if (debugFunction[IMMORTAL].str == "Off")
	{
		debugFunction[IMMORTAL].str = "On";
		debugMsg[IMMORTAL] = true;
		totalDebugFunctionUsed++;
	}
	else
	{
		debugFunction[IMMORTAL].str = "Off";
		debugMsg[IMMORTAL] = false;
		totalDebugFunctionUsed--;
	}
}

void TurnOnFullPower()
{
	if (debugFunction[MAX_POWER].str == "Off")
	{
		debugFunction[MAX_POWER].str = "On";
		debugMsg[MAX_POWER] = true;
		totalDebugFunctionUsed++;
	}
	else
	{
		debugFunction[MAX_POWER].str = "Off";
		debugMsg[MAX_POWER] = false;
		totalDebugFunctionUsed--;
	}
}

void GoNextLevel()
{
	DebuggerManager::GetCurrentArea() = min(DebuggerManager::GetCurrentArea() + 1, AreaNumber::AREA_LAST);
}

void GoPrevLevel()
{
	DebuggerManager::GetCurrentArea() = max(AreaNumber::AREA_0, DebuggerManager::GetCurrentArea() - 1);
}

DebuggerManager::DebuggerManager(Player* player)
{
	quad = GenerateSquareMesh(0xFFFFFFFF);
	playerInfo = player;
	openDebugPanel = false;

	for (size_t index = 0; index < DEBUGGER_TOTAL; index++)
	{
		debugFunction[index].str = "Off";
		debugFunction[index].pos = { AEGfxGetWinMinX() + 175.f, AEGfxGetWinMaxY() - 75.f - index * 60.f };
		debugFunction[index].scale = { 80.f, 40.f };
		debugFunction[index].UpdateTransformMatrix();

		switch (index)
		{
		case DebuggerFunction::FPS:
			debugFunction[index].Ptr = ShowFPS;
			break;
		case DebuggerFunction::POSITION:
			debugFunction[index].Ptr = ShowPlayerPosition;
			break;
		case DebuggerFunction::IMMORTAL:
			debugFunction[index].Ptr = TurnOnImmortal;
			break;
		case DebuggerFunction::MAX_POWER:
			debugFunction[index].Ptr = TurnOnFullPower;
			break;
		case DebuggerFunction::TELEPORT_LEVEL_UP:
			debugFunction[index].Ptr = GoPrevLevel;
			break;
		case DebuggerFunction::TELEPORT_LEVEL_DOWN:
			debugFunction[index].Ptr = GoNextLevel;
			break;
		default:
			break;
		}
	}
	debugFunction[TELEPORT_LEVEL_UP].str = "<-";

	debugFunction[TELEPORT_LEVEL_DOWN].str = "->";
	debugFunction[TELEPORT_LEVEL_DOWN].pos = { AEGfxGetWinMinX() + 255.f, AEGfxGetWinMaxY() - 75.f - DebuggerFunction::TELEPORT_LEVEL_UP * 60.f };
	debugFunction[TELEPORT_LEVEL_DOWN].UpdateTransformMatrix();
	
	currArea = current - GameStates::GAME_LOBBY;
}

DebuggerManager::~DebuggerManager()
{
	AEGfxMeshFree(quad);
}

void DebuggerManager::Update()
{
	if (AEInputCheckTriggered(AEVK_0))
	{
		openDebugPanel = !openDebugPanel;
		currArea = current - GameStates::GAME_LOBBY;
	}

	if (openDebugPanel)
	{
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			s32 cx, cy;
			AEInputGetCursorPosition(&cx, &cy);
			AEVec2 mousePos{ 0, 0 };
			mousePos.x = cx - AEGfxGetWindowWidth() * 0.5f;
			mousePos.y = AEGfxGetWindowHeight() * 0.5f - cy;

			f32 x, y;
			AEGfxGetCamPosition(&x, &y);

			std::cout << y << std::endl;
			for (size_t index = 0; index < DEBUGGER_TOTAL; index++)
			{
				AEVec2 translateOrigin = debugFunction[index].pos;
				translateOrigin.x -= x;
				translateOrigin.y -= y;
				if (AETestPointToRect(&mousePos, &translateOrigin,
					debugFunction[index].scale.x, debugFunction[index].scale.y))
				{
					debugFunction[index].Ptr();
					switch (index)
					{
					case DebuggerFunction::FPS:
					case DebuggerFunction::POSITION:
						break;
					case DebuggerFunction::IMMORTAL:
						playerInfo->SetDebugModeImmortal(!playerInfo->GetDebugModeImmortal());
						break;
					case DebuggerFunction::MAX_POWER:
						playerInfo->SetDebugModeOverpower(!playerInfo->GetDebugModeOverpower());
						break;
					case DebuggerFunction::TELEPORT_LEVEL_UP:
					case DebuggerFunction::TELEPORT_LEVEL_DOWN:
					default:
						break;
					}
				}
			}
		}
		for (size_t index = 0; index < TELEPORT_LEVEL_DOWN; index++)
		{
			debugFunction[index].pos.x = AEGfxGetWinMinX() + 175.f;
			debugFunction[index].pos.y = AEGfxGetWinMaxY() - 75.f - index * 60.f;
			debugFunction[index].UpdateTransformMatrix();
		}
		debugFunction[TELEPORT_LEVEL_DOWN].pos = { AEGfxGetWinMinX() + 275.f, AEGfxGetWinMaxY() - 75.f - DebuggerFunction::TELEPORT_LEVEL_UP * 60.f };
		debugFunction[TELEPORT_LEVEL_DOWN].UpdateTransformMatrix();
	}

}

void DebuggerManager::RenderDebuggerUI()
{
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	if (openDebugPanel)
	{
		for (size_t index = 0; index < DEBUGGER_TOTAL; index++)
		{
			AEGfxSetTransform(debugFunction[index].transform.m);
			AEGfxMeshDraw(quad, AE_GFX_MDM_TRIANGLES);

			f32 width, height;
			AEGfxGetPrintSize(fontID, debugFunction[index].str.c_str(), 0.3f, &width, &height);
			AEGfxPrint(fontID, debugFunction[index].str.c_str(), -0.81, -height / 2 + 0.83f - index * 0.13f, 0.3f, 0, 0, 0, 1);
		}
	}

	if (totalDebugFunctionUsed > 0)
	{
		AEGfxSetTransform(ObjectTransformationMatrixSet(AEGfxGetWinMaxX() - 175.f, AEGfxGetWinMaxY() - 75.f, 0.f, 350.f, 150.f).m);
		AEGfxMeshDraw(quad, AE_GFX_MDM_TRIANGLES);

		size_t activeDebug = 0;
		for (size_t index = 0; index < DEBUGGER_TOTAL; index++)
		{
			if (!debugMsg[index])
				continue;

			f32 width, height;
			std::stringstream str;

			switch (index)
			{
			case DebuggerFunction::FPS:
				str << "FPS: " << AEFrameRateControllerGetFrameRate();
				break;
			case DebuggerFunction::POSITION:
				str << "Player Pos:" << (int)playerInfo->GetPlayerCurrentPosition().x << "," << (int)playerInfo->GetPlayerCurrentPosition().y;
				break;
			case DebuggerFunction::IMMORTAL:
				str << "Player Immortal: True";
				break;
			case DebuggerFunction::MAX_POWER:
				str << "Player Max Attack: True";
				break;
			default:
				break;
			}

			AEGfxGetPrintSize(fontID, str.str().c_str(), 0.3f, &width, &height);
			AEGfxPrint(fontID, str.str().c_str(), 0.6, -height / 2 + 0.95f - activeDebug++ * 0.05f, 0.3f, 0, 0, 0, 1);
		}
	}
}

int& DebuggerManager::GetCurrentArea()
{
	return currArea;
}