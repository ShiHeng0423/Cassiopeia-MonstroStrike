#include "DebuggerManager.h"
#include "Utils.h"
#include "GameStateManager.h"
#include "MapTransition.h"
#include "main.h"
#include <sstream>
#include <vector>

namespace
{
	AEGfxVertexList* quad;
	AEGfxTexture* debugBackgroundTex;

	Sprite debugBackground;
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

void TeleportPlayer()
{
	int playerCurrArea = DebuggerManager::GetCurrentArea() + GameStates::GAME_LOBBY;
	if (playerCurrArea > current)
	{
		next = current = playerCurrArea - 1;
		switch (current)
		{
		case GAME_LOBBY:
			transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1_A);
			break;
		case AREA1_A:
			transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1_B);
			break;
		case AREA1_B:
			transitionalImageOBJ.PlayMapTransition(TRANSITION_RIGHT, AREA1_C);
			break;
		case AREA1_C:
			transitionalImageOBJ.PlayMapTransition(TRANSITION_RIGHT, AREA1_D);
			break;
		case AREA1_D:
			transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1_E);
			break;
		case AREA1_E:
			transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1_F);
			break;
		case AREA1_F:
			transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA_BOSS);
			break;
		case AREA_BOSS:
		default:
			break;
		}
	}
	else if (playerCurrArea < current)
	{
		next = current = playerCurrArea + 1;
		switch (current)
		{
		case GAME_LOBBY:
			break;
		case AREA1_A:
			transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, GAME_LOBBY);
			break;
		case AREA1_B:
			transitionalImageOBJ.PlayMapTransition(TRANSITION_UP, AREA1_A);
			break;
		case AREA1_C:
			transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1_B);
			break;
		case AREA1_D:
			transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1_C);
			break;
		case AREA1_E:
			transitionalImageOBJ.PlayMapTransition(TRANSITION_RIGHT, AREA1_D);
			break;
		case AREA1_F:
			transitionalImageOBJ.PlayMapTransition(TRANSITION_RIGHT, AREA1_E);
			break;
		case AREA_BOSS:
			transitionalImageOBJ.PlayMapTransition(TRANSITION_LEFT, AREA1_F);
			break;
		default:
			break;
		}
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
	debugBackgroundTex = AEGfxTextureLoad("Assets/panelInset_beige.png");
	playerInfo = player;
	openDebugPanel = false;

	for (size_t index = 0; index < DEBUGGER_TOTAL; index++)
	{
		debugFunction[index].str = "Off";
		debugFunction[index].pos = { AEGfxGetWinMinX() + 175.f, AEGfxGetWinMaxY() - 40.f - index * 60.f };
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
		case DebuggerFunction::TELEPORT_CONFIRMATION:
			debugFunction[index].Ptr = TeleportPlayer;
			break;
		default:
			break;
		}
	}
	debugFunction[TELEPORT_LEVEL_UP].str = "<-";

	debugFunction[TELEPORT_LEVEL_DOWN].str = "->";
	debugFunction[TELEPORT_LEVEL_DOWN].pos = { AEGfxGetWinMinX() + 255.f, AEGfxGetWinMaxY() - 40.f - DebuggerFunction::TELEPORT_LEVEL_UP * 60.f };
	debugFunction[TELEPORT_LEVEL_DOWN].UpdateTransformMatrix();

	debugFunction[TELEPORT_CONFIRMATION].str = "Ok";

	debugBackground.pTex = debugBackgroundTex;
	debugBackground.scale={ 400,400 };
	debugBackground.pos = { AEGfxGetWinMinX() + 200.f, AEGfxGetWinMaxY() - debugBackground.scale.y * 0.5f };
	debugBackground.UpdateTransformMatrix();
	
	currArea = current - GameStates::GAME_LOBBY;
}

DebuggerManager::~DebuggerManager()
{
	AEGfxMeshFree(quad);
	AEGfxTextureUnload(debugBackgroundTex);
}

void DebuggerManager::Update()
{
	if (AEInputCheckTriggered(AEVK_0) && current >= GameStates::GAME_LOBBY)
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
						break;
					case DebuggerFunction::TELEPORT_CONFIRMATION:
						openDebugPanel = false;
						break;
					default:
						break;
					}
				}
			}
		}
		for (size_t index = 0; index < TELEPORT_LEVEL_DOWN; index++)
		{
			debugFunction[index].pos.x = AEGfxGetWinMinX() + 175.f;
			debugFunction[index].pos.y = AEGfxGetWinMaxY() - 40.f - index * 60.f;
			debugFunction[index].UpdateTransformMatrix();
		}
		debugFunction[TELEPORT_LEVEL_DOWN].pos = { AEGfxGetWinMinX() + 275.f, AEGfxGetWinMaxY() - 40.f - DebuggerFunction::TELEPORT_LEVEL_UP * 60.f };
		debugFunction[TELEPORT_LEVEL_DOWN].UpdateTransformMatrix();

		debugFunction[TELEPORT_CONFIRMATION].pos = { AEGfxGetWinMinX() + 225.f, AEGfxGetWinMaxY() - 40.f - DebuggerFunction::TELEPORT_LEVEL_DOWN * 60.f };
		debugFunction[TELEPORT_CONFIRMATION].UpdateTransformMatrix();

		debugBackground.pos = { AEGfxGetWinMinX() + 200.f, AEGfxGetWinMaxY() - debugBackground.scale.y * 0.5f };
		debugBackground.UpdateTransformMatrix();
	}

}

void DebuggerManager::RenderDebuggerUI()
{

	if (openDebugPanel)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE); 
		AEGfxTextureSet(debugBackground.pTex, 0, 0);
		AEGfxSetTransform(debugBackground.transform.m);
		AEGfxMeshDraw(quad, AE_GFX_MDM_TRIANGLES);

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		for (size_t index = 0; index < DEBUGGER_TOTAL; index++)
		{
			AEGfxSetTransform(debugFunction[index].transform.m);
			AEGfxMeshDraw(quad, AE_GFX_MDM_TRIANGLES);

			f32 width, height;
			std::stringstream str;

			switch (index)
			{
			case DebuggerFunction::FPS:
				str << "FPS";
				AEGfxGetPrintSize(fontID, str.str().c_str(), 0.3f, &width, &height);
				AEGfxPrint(fontID, str.str().c_str(), -0.99f, -height / 2 + 0.90f - index * 0.13f, 0.3f, 0, 0, 0, 1);
				break;
			case DebuggerFunction::POSITION:
				str << "Position";
				AEGfxGetPrintSize(fontID, str.str().c_str(), 0.3f, &width, &height);
				AEGfxPrint(fontID, str.str().c_str(), -0.99f, -height / 2 + 0.90f - index * 0.13f, 0.3f, 0, 0, 0, 1);
				break;
			case DebuggerFunction::IMMORTAL:
				str << "Immortal";
				AEGfxGetPrintSize(fontID, str.str().c_str(), 0.3f, &width, &height);
				AEGfxPrint(fontID, str.str().c_str(), -0.99f, -height / 2 + 0.90f - index * 0.13f, 0.3f, 0, 0, 0, 1);
				break;
			case DebuggerFunction::MAX_POWER:
				str << "Max Atk";
				AEGfxGetPrintSize(fontID, str.str().c_str(), 0.3f, &width, &height);
				AEGfxPrint(fontID, str.str().c_str(), -0.99f, -height / 2 + 0.90f - index * 0.13f, 0.3f, 0, 0, 0, 1);
				break;
			case DebuggerFunction::TELEPORT_LEVEL_UP:
				str << "Level " << currArea;
				AEGfxGetPrintSize(fontID, str.str().c_str(), 0.3f, &width, &height);
				AEGfxPrint(fontID, str.str().c_str(), -0.99f, -height / 2 + 0.90f - index * 0.13f, 0.3f, 0, 0, 0, 1);
				break;
			default:
				break;
			}

			AEGfxGetPrintSize(fontID, debugFunction[index].str.c_str(), 0.3f, &width, &height);

			if (index == TELEPORT_LEVEL_DOWN)
				AEGfxPrint(fontID, debugFunction[index].str.c_str(), -0.67f, -height / 2 + 0.90f - TELEPORT_LEVEL_UP * 0.13f, 0.3f, 0, 0, 0, 1);
			else if (index == TELEPORT_CONFIRMATION)
				AEGfxPrint(fontID, debugFunction[index].str.c_str(), -0.74f, -height / 2 + 0.90f - TELEPORT_LEVEL_DOWN * 0.13f, 0.3f, 0, 0, 0, 1);
			else
				AEGfxPrint(fontID, debugFunction[index].str.c_str(), -0.81f, -height / 2 + 0.90f - index * 0.13f, 0.3f, 0, 0, 0, 1);
		}
	}

	if (totalDebugFunctionUsed > 0)
	{
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

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
			AEGfxPrint(fontID, str.str().c_str(), 0.6f, -height / 2 + 0.95f - activeDebug++ * 0.05f, 0.3f, 0, 0, 0, 1);
		}
	}
}

int& DebuggerManager::GetCurrentArea()
{
	return currArea;
}