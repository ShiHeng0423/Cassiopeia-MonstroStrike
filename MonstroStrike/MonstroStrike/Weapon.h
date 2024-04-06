#pragma once
/*!************************************************************************
  \file                    Weapon.h
  \project name            Monstrostrike
  \primary author          Choo Jian Wei (80%)
  \secondary author        Teng Shi Heng (10%, provided the Equip_weapon function), Johny Yong Jun Siang (Provided Check Weapon Collision)
  \brief
  This file implements functions for the weapon system.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#include <string>
#include "Player.h"
#include "CollisionShape.h"
#include "Enemy.h"
#include "StatusEffect.h"

//shi heng
namespace Weapon_System
{
    //shi heng
    enum WEAPON_GRADE {
        TIER_1,
        TIER_2,
        TIER_3,

        NO_GRADE
    };

    //shi heng
    struct Weapon_Set
    {
        //shi heng
        WEAPON_GRADE rarity{ WEAPON_GRADE::NO_GRADE };
        Status_Effect_System::Status_Effect extraEffect{ Status_Effect_System::Status_Effect::NONE_EFFECT };

        //jian wei
        std::string name{ "" };
        int damage{ 0 };

        AABB collisionBox{};
        AABB hitBox{};

        AEVec2 position{ 0,0 };
        AEVec2 scale{ 0,0 };
        AEVec2 transformation{ 0,0 };

        bool weaponHIT{ false };
    };

    //shi heng
    void Equip_Weapon(class Player& player, Weapon_System::WEAPON_GRADE newWeaponGrade);

    //jian wei
    void UpdateWeaponHitBoxTrig(class Player* player, bool playerFacingRight, struct Weapon_Set*, f32);
    void UpdateWeaponHitBoxHeld(class Player* player, bool playerFacingRight, struct Weapon_Set* playerEquip, f32);

    //johny
    void CheckWeaponCollision(struct Weapon_Set* playerEquip, struct Enemy& theEnemy, class Player& player);
}
