#pragma once
#include "AEEngine.h"

namespace Status_Effect_System
{
    enum Armor_Status_Effect {
        BURNING,
        REGEN,

        NONE_ARMOR_EFFECT
    };
}

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

    struct Armor_Set
    {
        Armor pieces[4];
        Status_Effect_System::Armor_Status_Effect extraEffect{ Status_Effect_System::Armor_Status_Effect::NONE_ARMOR_EFFECT };
        f32 effectTimer{ 0.f };
    };
}


Armor_System::Armor ArmorInformation(Armor_System::ARMOR_TYPE type, Armor_System::ARMOR_GRADE grade);

f32 ArmorSetBonusInformation(int armorSetID, bool fullSetBonus, Status_Effect_System::Armor_Status_Effect& effect);

void Equip_Armor(class Player& player, Armor_System::ARMOR_TYPE newArmorType, Armor_System::ARMOR_GRADE newArmorGrade);

f32 Check_Set_Effect(class Player& player);

void Armor_Effect_Update(class Player& player);