#pragma once
/*!************************************************************************
  \file					GridTypesList.h
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (100%)
  \brief				File containing the types of grid available in game, along with the value assigned to it.

All content � 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/
enum GridTypes
{
    EMPTY = 0,
    NORMAL_GROUND = 1,
    
    //Moving platform grids
    HORIZONTAL_PLATFORM_POS = 2,
    VERTICAL_PLATFORM_POS = 3,
    DIAGONAL_PLATFORM_POS = 4,
    LAVA_GRID = 5,

    RETURN_PORTAL_GRID = 6,


    //NPC Positions grid,
    NPC_BLACKSMITH_A_POS = 10,
    NPC_BLACKSMITH_B_POS = 11,
    NPC_QUEST_GIVER_POS = 12,

    //Enemy positions grid
    ENEMY_CHARGER_POS = 20,
    ENEMY_FLY_POS = 21,
    ENEMY_SLIME_POS = 22,

    //Level gimmicks grid
    TRAPS_POISON_ARROW_GRID = 30,
    TRAPS_SLOWNESS_ARROW_GRID = 31,
    TRAPS_FIRE_CIRCLE_GRID = 32,
    TRAPS_ROTATING_SAW_GRID = 33,


    //Player positions
    PLAYER_POS_GRID_1 = 91,
    PLAYER_POS_GRID_2 = 92,
    PLAYER_POS_GRID_3 = 93,
    PLAYER_POS_GRID_4 = 94,

    //Map transition grids
    MAP_TRANSITION_GRID_1 = 95,
    MAP_TRANSITION_GRID_2 = 96,
    MAP_TRANSITION_GRID_3 = 97,
    MAP_TRANSITION_GRID_4 = 98,
    NONE = 100
};