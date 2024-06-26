/*!************************************************************************
  \file                    Weapon.cpp
  \project name            Monstrostrike
  \primary author          Choo Jian Wei (80%)
  \secondary author        Teng Shi Heng (10%, provided the Equip_weapon function), Johny Yong Jun Siang (Provided Check Weapon Collision)
  \brief
  This file implements functions for the weapon system.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/
#include "Player.h"
#include <string>
#include "Weapon.h"
#include "CollisionShape.h"
#include <iostream>
#include "Enemy.h"
#include <AEMath.h>

namespace Weapon_System
{
    #define M_PI 3.1415

    //player equipping and unequip logic
    void Equip_Weapon(class Player& player, Weapon_System::WEAPON_GRADE newWeaponGrade)
    {
        Weapon_System::Weapon_Set tmp;
        tmp.rarity = newWeaponGrade;
        AEVec2Set(&tmp.position, player.GetCameraExpectedPosition().x, player.GetCameraExpectedPosition().y);

        switch (newWeaponGrade)
        {
        case Weapon_System::WEAPON_GRADE::TIER_1:
            tmp.name = "Short-Sword";
            tmp.damage = 4;
            tmp.extraEffect = Status_Effect_System::NONE_EFFECT;
            break;
        case Weapon_System::WEAPON_GRADE::TIER_2:
            tmp.name = "Broad-Sword";
            tmp.damage = 7;
            tmp.extraEffect = Status_Effect_System::LIFE_STEAL;
            break;
        case Weapon_System::WEAPON_GRADE::TIER_3:
            tmp.name = "GreatSword";
            tmp.damage = 5;
            tmp.extraEffect = Status_Effect_System::NONE_EFFECT;
            break;
        case Weapon_System::WEAPON_GRADE::NO_GRADE:
            tmp.name = "No-Weapon";
            tmp.damage = 0;
            tmp.extraEffect = Status_Effect_System::NONE_EFFECT;
            break;
        default:
            break;
        }

        //check for weapon passive granted to player
        if (player.GetWeaponSet().rarity != tmp.rarity)
        {
            if (player.GetWeaponSet().extraEffect != Status_Effect_System::Status_Effect::NONE_EFFECT)
            {
                size_t index = 0;
                for (std::pair<Status_Effect_System::Status_Effect, Status_Effect_System::Status_Effect_Source> effect : player.playerStatusEffectList)
                {
                    if (effect.second == Status_Effect_System::Status_Effect_Source::WEAPON)
                    {
                        std::cout << index << std::endl;
                        break;
                    }
                    index++;
                }
                player.playerStatusEffectList.erase(player.playerStatusEffectList.begin() + index);
            }
            if (tmp.extraEffect != Status_Effect_System::Status_Effect::NONE_EFFECT)
            {
                player.playerStatusEffectList.push_back({ tmp.extraEffect, Status_Effect_System::Status_Effect_Source::WEAPON });
            }
        }

        player.GetWeaponSet() = tmp;
    }

    float xHitOffset = 0.0f;
    float yHitOffset = 0.0f;
    float xScale = 0.0f;
    float yScale = 0.0f;
    float xScaler = 0.0f;

    void UpdateWeaponHitBoxTrig(class Player* player, bool playerFacingRight, struct Weapon_Set* playerEquip, f32 attackProgress)
    {

        if (player->GetWeaponSet().name == "Short-Sword")
        {
            //Initializes weapon's position
            if (player->GetComboState() == 0)
            {
                xHitOffset = 10.0f;
                yHitOffset = 0.0f;
                xScale = 30.0f;
                yScale = 20.0f;

                playerEquip->position.x = player->GetPlayerCurrentPosition().x;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y;

                // change the hitbox
                //f32 hit = playerFacingRight ?  // change the hitbox
                //    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y;

                AEVec2Set(&player->GetWeaponSet().scale, xScale, yScale);

                f32 xOffset = playerFacingRight ? playerEquip->scale.x : -playerEquip->scale.x;

                playerEquip->position.x += xOffset;
                playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

                playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x;
                playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y;
                playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x;
                playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y;


                playerEquip->weaponHIT = false;


            }
            else if (player->GetComboState() == 1)
            {
                xHitOffset = 10.0f;
                yHitOffset = 10.0f;
                xScale = 30.0f;
                yScale = 20.0f;

                playerEquip->position.x = player->GetPlayerCurrentPosition().x;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y;
                // change the hitbox
                //f32 hit = playerFacingRight ?  // change the hitbox
                //    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y - yHitOffset;

                AEVec2Set(&player->GetWeaponSet().scale, xScale, yScale);

                f32 xOffset = playerFacingRight ? playerEquip->scale.x : -playerEquip->scale.x;

                playerEquip->position.x += xOffset;
                playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

                playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
                playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;

                //player->GetIsPlayerAttacking() = false;
                playerEquip->weaponHIT = false;



            }
            else if (player->GetComboState() == 2)
            {
                xHitOffset = 10.0f;
                yHitOffset = 20.0f;
                xScale = 30.0f;
                yScale = 20.0f;

                playerEquip->position.x = player->GetPlayerCurrentPosition().x;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y;
                // change the hitbox
                //f32 hit = playerFacingRight ?  // change the hitbox
                //    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y - yHitOffset;

                AEVec2Set(&player->GetWeaponSet().scale, xScale, yScale);

                f32 xOffset = playerFacingRight ? playerEquip->scale.x : -playerEquip->scale.x;

                playerEquip->position.x += xOffset;
                playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

                playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
                playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;

                std::cout << playerEquip->damage << std::endl;
                /*player->GetIsPlayerAttacking() = false;*/
                playerEquip->weaponHIT = false;



            }
        }

        else if (player->GetWeaponSet().name == "Broad-Sword")
        {
            //Initializes weapon's position
            playerEquip->position.x = player->GetPlayerCurrentPosition().x;
            playerEquip->position.y = player->GetPlayerCurrentPosition().y;

            if (player->GetComboState() == 0)
            {
                xHitOffset = 0.0f;
                yHitOffset = 27.0f;
                xScale = 45.0f;
                yScale = 55.0f;
                //Initializes weapon's position
                playerEquip->position.x = player->GetPlayerCurrentPosition().x;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y;

                // change the hitbox
                //f32 hit = playerFacingRight ?  // change the hitbox
                //    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y - yHitOffset;

                AEVec2Set(&player->GetWeaponSet().scale, xScale, yScale);

                f32 xOffset = playerFacingRight ? playerEquip->scale.x : -playerEquip->scale.x;

                playerEquip->position.x += xOffset;
                playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

                playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
                playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;


                //player->GetIsPlayerAttacking() = false; //If player is attacking render the hitbox
                playerEquip->weaponHIT = false;


            }
            else if (player->GetComboState() == 1)
            {
                xHitOffset = 0.0f;
                yHitOffset = 27.0f;
                xScale = 45.0f;
                yScale = 55.0f;

                // change the hitbox
                //f32 hit = playerFacingRight ?  // change the hitbox
                //    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y - yHitOffset;

                AEVec2Set(&player->GetWeaponSet().scale, xScale, yScale);

                f32 xOffset = playerFacingRight ? playerEquip->scale.x : -playerEquip->scale.x;

                playerEquip->position.x += xOffset;
                playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

                playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
                playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;

                playerEquip->weaponHIT = false;
            }
            else if (player->GetComboState() == 2)
            {
                xHitOffset = 0.0f;
                yHitOffset = 20.0f;
                xScale = 60.0f;
                yScale = 40.0f;
                xScaler = 0.9f;

                // change the hitbox
                //f32 hit = playerFacingRight ?  // change the hitbox
                //    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y - yHitOffset;

                AEVec2Set(&player->GetWeaponSet().scale, xScale, yScale);

                f32 xOffset = playerFacingRight ? playerEquip->scale.x * xScaler : -playerEquip->scale.x * xScaler;

                playerEquip->position.x += xOffset;
                playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

                playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
                playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;

                std::cout << playerEquip->damage << std::endl;
                playerEquip->weaponHIT = false;
            }
        }
        else if (player->GetWeaponSet().name == "GreatSword")
        {
            if (player->GetComboState() == 0)
            {
                xHitOffset = 0.0f;
                yHitOffset = 30.0f;
                xScale = 60.0f;
                yScale = 60.0f;
                xScaler = 0.9f;
                //Initializes weapon's position
                playerEquip->position.x = player->GetPlayerCurrentPosition().x;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y;

                // change the hitbox
                //f32 hit = playerFacingRight ?  // change the hitbox
                //    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y - yHitOffset;

                AEVec2Set(&player->GetWeaponSet().scale, xScale, yScale);

                f32 xOffset = playerFacingRight ? playerEquip->scale.x * xScaler : -playerEquip->scale.x * xScaler;

                playerEquip->position.x += xOffset;
                playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

                playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
                playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;

                playerEquip->weaponHIT = false;
            }
            else if (player->GetComboState() == 1)
            {
                xHitOffset = 0.0f;
                yHitOffset = 30.0f;
                xScale = 60.0f;
                yScale = 60.0f;
                xScaler = 0.9f;

                //Initializes weapon's position
                playerEquip->position.x = player->GetPlayerCurrentPosition().x;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y;

                // change the hitbox
                //f32 hit = playerFacingRight ?  // change the hitbox
                //    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y - yHitOffset;

                AEVec2Set(&player->GetWeaponSet().scale, xScale, yScale);

                f32 xOffset = playerFacingRight ? playerEquip->scale.x * xScaler : -playerEquip->scale.x * xScaler;

                playerEquip->position.x += xOffset;
                playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

                playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
                playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;

                //player->GetIsPlayerAttacking() = false;
                playerEquip->weaponHIT = false;



            }
            else if (player->GetComboState() == 2)
            {
                xHitOffset = 0.0f;
                yHitOffset = 30.0f;
                xScale = 80.0f;
                yScale = 60.0f;
                xScaler = 0.8f;

                //Initializes weapon's position
                playerEquip->position.x = player->GetPlayerCurrentPosition().x;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y;


                // change the hitbox
                //f32 hit = playerFacingRight ?  // change the hitbox
                //    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y - yHitOffset;

                AEVec2Set(&player->GetWeaponSet().scale, 80.f, 60.f);

                f32 xOffset = playerFacingRight ? playerEquip->scale.x * xScaler : -playerEquip->scale.x * xScaler;

                playerEquip->position.x += xOffset;
                playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

                playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
                playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;

                std::cout << playerEquip->damage << std::endl;
                playerEquip->weaponHIT = false;
            }
        }
    }

    void UpdateWeaponHitBoxHeld(class Player* player, bool playerFacingRight, struct Weapon_Set* playerEquip, f32 attackProgress)
    {
        if (player->GetWeaponSet().name == "Short-Sword")
        {
            if (player->GetComboState() == 2)
            {
                xHitOffset = 10.0f;
                yHitOffset = 30.0f;
                xScale = 30.0f;
                yScale = 50.0f;

                //Initializes weapon's position
                playerEquip->position.x = player->GetPlayerCurrentPosition().x;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y;

                // change the hitbox
                //f32 hit = playerFacingRight ?  // change the hitbox
                //    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y - yHitOffset;

                AEVec2Set(&player->GetWeaponSet().scale, xScale, yScale);

                f32 xOffset = playerFacingRight ? playerEquip->scale.x : -playerEquip->scale.x;

                playerEquip->position.x += xOffset;
                playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

                playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
                playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;

                std::cout << playerEquip->damage << std::endl;
                player->GetIsPlayerAttacking() = false;
                playerEquip->weaponHIT = false;

            }

        }
        if (player->GetWeaponSet().name == "Broad-Sword")
        {
            if (player->GetComboState() == 2)
            {

                xHitOffset = 0.0f;
                yHitOffset = 30.0f;
                xScale = 80.0f;
                yScale = 70.0f;
                xScaler = 0.8f;

                //Initializes weapon's position
                playerEquip->position.x = player->GetPlayerCurrentPosition().x;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y;


                // change the hitbox
                //f32 hit = playerFacingRight ?  // change the hitbox
                //    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y - yHitOffset;

                AEVec2Set(&player->GetWeaponSet().scale, xScale, yScale);

                f32 xOffset = playerFacingRight ? playerEquip->scale.x * xScaler : -playerEquip->scale.x * xScaler;

                playerEquip->position.x += xOffset;
                playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

                playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
                playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;

                std::cout << playerEquip->damage << std::endl;
                player->GetIsPlayerAttacking() = false;
                playerEquip->weaponHIT = false;

            }
        }
        if (player->GetWeaponSet().name == "GreatSword")
        {

            if (player->GetComboState() == 2)
            {
                xHitOffset = 0.0f;
                yHitOffset = 30.0f;
                xScale = 100.0f;
                yScale = 80.0f;
                xScaler = 0.7f;

                //Initializes weapon's position
                playerEquip->position.x = player->GetPlayerCurrentPosition().x;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y;

                // change the hitbox
                //f32 hit = playerFacingRight ?  // change the hitbox
                //    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y - yHitOffset;

                AEVec2Set(&player->GetWeaponSet().scale, xScale, yScale);

                f32 xOffset = playerFacingRight ? playerEquip->scale.x * xScaler : -playerEquip->scale.x * xScaler;

                playerEquip->position.x += xOffset;
                playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

                playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
                playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
                playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;

                std::cout << playerEquip->damage << std::endl;
                player->GetIsPlayerAttacking() = false;
                playerEquip->weaponHIT = false;

            }

        }
        else
        {


        }
    }

    void CheckWeaponCollision(struct Weapon_Set* playerEquip, struct Enemy& theEnemy, class Player& player)
    {
        //wings
        if (!playerEquip->weaponHIT && AABBvsAABB(playerEquip->collisionBox, theEnemy.wing1.collisionBox))
        {
            std::cout << "Attack landed wing1" << std::endl;
            playerEquip->weaponHIT = true;

            if (player.GetDebugModeOverpower())
            {
                theEnemy.wing1.health = 0;
            }
            else
            {
                int totalDmg = playerEquip->damage;
                if (player.GetArmorSet().extraEffect == Status_Effect_System::Status_Effect::BURNING)
                    totalDmg *= 2;
                if (player.GetWeaponSet().extraEffect == Status_Effect_System::Status_Effect::LIFE_STEAL)
                    player.RecoverHpToPlayer(totalDmg);

                theEnemy.wing1.health -= totalDmg;
            }
        }

        if (!playerEquip->weaponHIT && AABBvsAABB(playerEquip->collisionBox, theEnemy.wing2.collisionBox))
        {
            std::cout << "Attack landed wing1" << std::endl;
            playerEquip->weaponHIT = true;
            if (player.GetDebugModeOverpower())
            {
                theEnemy.wing2.health = 0;
            }
            else
            {
                int totalDmg = playerEquip->damage;
                if (player.GetArmorSet().extraEffect == Status_Effect_System::Status_Effect::BURNING)
                    totalDmg *= 2;
                if (player.GetWeaponSet().extraEffect == Status_Effect_System::Status_Effect::LIFE_STEAL)
                    player.RecoverHpToPlayer(totalDmg);

                theEnemy.wing2.health -= totalDmg;
            }
        }


        if (!playerEquip->weaponHIT && AABBvsAABB(playerEquip->collisionBox, theEnemy.collisionBox)) //Success
        {
            playerEquip->weaponHIT = true;

            if (player.GetDebugModeOverpower())
            {
                theEnemy.health = 0;
            }
            else
            {
                int totalDmg = playerEquip->damage;
                if (player.GetArmorSet().extraEffect == Status_Effect_System::Status_Effect::BURNING)
                    totalDmg *= 2;
                if (player.GetWeaponSet().extraEffect == Status_Effect_System::Status_Effect::LIFE_STEAL)
                    player.RecoverHpToPlayer(totalDmg);

                theEnemy.health -= totalDmg;
            }
        }

    }
}