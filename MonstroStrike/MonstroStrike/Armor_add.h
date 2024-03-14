#pragma once
#include "Player.h"
struct Armor {
    enum class Type {
        FIRST,
        SECOND
    };



    Type armorType;
    union {
        struct LeatherArmor {
            int defense;
            // Additional properties specific to leather armor
        } leather;

        struct SteelArmor {
            int defense;
            // Additional properties specific to steel armor
        } steel;
    };
};
