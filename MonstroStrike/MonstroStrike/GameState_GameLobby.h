#pragma once
/*!************************************************************************
  \file					GameState_GameLobby.h
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (50%, NPCs, added initialisations of grids and traps etc, map transitions,
						particle effects)
  \secondary authors	Teng Shi Heng (35%, player, menu UI, Camera),
						Keith Ng Hon Yew (15%, inventory)
  \brief				File containing declarations of functions related to the lobby room state. Such as loading,
						initializing, update, drawing etc... of the level.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/
void Lobby_Load();
void Lobby_Initialize();
void Lobby_Update();
void Lobby_Draw();
void Lobby_Free();
void Lobby_Unload();
