#pragma once
#include "AEEngine.h"

namespace Status_Effect_System
{
    enum Armor_Status_Effect {
        BURNING,
        REGEN,

        NONE_ARMOR_EFFECT
    };

    enum Weapon_Status_Effect {
        DRAINING,
        
        NONE_WEAPON_EFFECT
    };
}

namespace Weapon_System
{
    enum WEAPON_GRADE {
        TIER_1,
        TIER_2,
        TIER_3,

        NO_GRADE
    };

    struct Weapon
    {
        int boost{ 0 };
        WEAPON_GRADE rarity{ WEAPON_GRADE::NO_GRADE };
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
        f32 effectTimer{ 0.f };
    };
}

void Equip_Weapon(class Player& player, Weapon_System::Weapon& playerCurrWeapon, Weapon_System::WEAPON_GRADE newWeaponGrade);

Armor_System::Armor ArmorInformation(Armor_System::ARMOR_TYPE type, Armor_System::ARMOR_GRADE grade);

f32 ArmorSetBonusInformation(int armorSetID, bool fullSetBonus, Status_Effect_System::Armor_Status_Effect& effect);

void Equip_Armor(class Player& player, Armor_System::Armor& playerCurrArmor, 
    Armor_System::ARMOR_TYPE newArmorType, Armor_System::ARMOR_GRADE newArmorGrade);

f32 Check_Set_Effect(class Player& player);

void Armor_Effect_Update(class Player& player);