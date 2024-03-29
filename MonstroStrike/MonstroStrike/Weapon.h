#pragma once
#include <string>
#include "Player.h"
#include "CollisionShape.h"
#include "Enemy.h"

//shi heng
namespace Status_Effect_System
{
    enum Weapon_Status_Effect {
        DRAINING,

        NONE_WEAPON_EFFECT
    };
}

//shi heng
namespace Weapon_System
{
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
        Status_Effect_System::Weapon_Status_Effect extraEffect;

        //jian wei
        std::string name;
        f32 damage{ 0 };

        AABB collisionBox;
        AABB hitBox;

        AEVec2 position;
        AEVec2 scale;
        AEVec2 transformation;

        bool weaponHIT;
    };

    //shi heng
    void Equip_Weapon(class Player& player, Weapon_System::WEAPON_GRADE newWeaponGrade);

    //jian wei
    void UpdateWeaponHitBoxTrig(class Player* player, bool playerFacingRight, struct Weapon_Set*, f32);
    void UpdateWeaponHitBoxHeld(class Player* player, bool playerFacingRight, struct Weapon_Set* playerEquip, f32);
    void CheckWeaponCollision(struct Weapon_Set* playerEquip, struct Enemy& theEnemy, class Player& player);
}
