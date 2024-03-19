#pragma once
#include "AEEngine.h"

namespace Armor_System
{
    enum ARMOR_GRADE {
        TIER_1,
        TIER_2,
        TIER_3,

        NO_GRADE
    };

    enum ARMOR_TYPE {
        HEAD,
        BODY,
        LEGS,
        FOOT,

        NO_TYPE
    };

    struct Armor
    {
        int boost{ 0 };
        ARMOR_GRADE rarity{ ARMOR_GRADE::NO_GRADE };
        ARMOR_TYPE type{ ARMOR_TYPE::NO_TYPE };
    };
}

Armor_System::Armor ArmorInformation(Armor_System::ARMOR_TYPE type, Armor_System::ARMOR_GRADE grade);

f32 ArmorSetBonusInformation(int armorSetID, bool fullSetBonus);

void Equip_Armor(class Player& player, Armor_System::Armor& playerCurrArmor, 
    Armor_System::ARMOR_TYPE newArmorType, Armor_System::ARMOR_GRADE newArmorGrade);

f32 Check_Set_Effect(class Player& player);