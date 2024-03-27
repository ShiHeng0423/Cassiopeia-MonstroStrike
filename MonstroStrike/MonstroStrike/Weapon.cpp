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
    void Equip_Weapon(class Player& player, Weapon_System::WEAPON_GRADE newWeaponGrade)
    {
        Weapon_System::Weapon_Set tmp;
        tmp.rarity = newWeaponGrade;
        AEVec2Set(&tmp.position, player.GetCameraExpectedPosition().x, player.GetCameraExpectedPosition().y);

        switch (newWeaponGrade)
        {
        case Weapon_System::WEAPON_GRADE::TIER_1:
            tmp.name = "Short-Sword";
            tmp.damage = 15;
            break;
        case Weapon_System::WEAPON_GRADE::TIER_2:
            tmp.name = "Broad-Sword";
            tmp.damage = 10;
            break;
        case Weapon_System::WEAPON_GRADE::TIER_3:
            tmp.name = "GreatSword";
            tmp.damage = 35;
            break;
        default:
            break;
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

            player->GetWeaponSet().damage = 10;

            if (player->GetComboState() == 0)
            {
                xHitOffset = 10.0f;
                yHitOffset = 0.0f;
                xScale = 30.0f;
                yScale = 20.0f;

                playerEquip->position.x = player->GetPlayerCurrentPosition().x;
                playerEquip->position.y = player->GetPlayerCurrentPosition().y;

                // change the hitbox
                f32 hit = playerFacingRight ?  // change the hitbox
                    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
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
                f32 hit = playerFacingRight ?  // change the hitbox
                    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
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


                // change the hitbox
                f32 hit = playerFacingRight ?  // change the hitbox
                    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
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
            player->GetWeaponSet().damage = 20;

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
                f32 hit = playerFacingRight ?  // change the hitbox
                    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
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
                f32 hit = playerFacingRight ?  // change the hitbox
                    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
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
                f32 hit = playerFacingRight ?  // change the hitbox
                    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
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
            player->GetWeaponSet().damage = 20;
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
                f32 hit = playerFacingRight ?  // change the hitbox
                    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
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
                f32 hit = playerFacingRight ?  // change the hitbox
                    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
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
                f32 hit = playerFacingRight ?  // change the hitbox
                    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
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
            playerEquip->damage = 20;

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
                f32 hit = playerFacingRight ?  // change the hitbox
                    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
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
                playerEquip->damage = 20;
                std::cout << playerEquip->damage << std::endl;

            }

        }
        if (player->GetWeaponSet().name == "Broad-Sword")
        {
            playerEquip->damage = 40;
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
                f32 hit = playerFacingRight ?  // change the hitbox
                    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
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
                playerEquip->damage = 60;
                std::cout << playerEquip->damage << std::endl;

            }
        }
        if (player->GetWeaponSet().name == "GreatSword")
        {
            playerEquip->damage = 100;

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
                f32 hit = playerFacingRight ?  // change the hitbox
                    playerEquip->position.x = player->GetPlayerCurrentPosition().x + xHitOffset : playerEquip->position.x = player->GetPlayerCurrentPosition().x - xHitOffset;
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
                playerEquip->damage = 20;
                std::cout << playerEquip->damage << std::endl;

            }

        }
        else
        {


        }
    }


    void CheckWeaponCollision(struct Weapon_Set* playerEquip, struct Enemy& theEnemy, class Player& player)
    {
        //wings
        if (!playerEquip->weaponHIT && AABBvsAABB(playerEquip->collisionBox, theEnemy.wing1.collisionBox)) {
            theEnemy.wing1.health -= (int)playerEquip->damage;
            std::cout << "Attack landed wing1" << std::endl;

            playerEquip->weaponHIT = true;
            if (player.GetWeaponSet().extraEffect == Status_Effect_System::Weapon_Status_Effect::DRAINING)
            {
                player.GetCurrentHealth() = min(player.GetCurrentHealth() + player.GetWeaponSet().damage, player.GetMaxHealth());
            }
        }
        if (!playerEquip->weaponHIT && AABBvsAABB(playerEquip->collisionBox, theEnemy.wing2.collisionBox)) {
            theEnemy.wing2.health -= (int)playerEquip->damage;
            std::cout << "Attack landed wing2" << std::endl;

            playerEquip->weaponHIT = true;
            if (player.GetWeaponSet().extraEffect == Status_Effect_System::Weapon_Status_Effect::DRAINING)
            {
                player.GetCurrentHealth() = min(player.GetCurrentHealth() + player.GetWeaponSet().damage, player.GetMaxHealth());
            }
        }


        if (!playerEquip->weaponHIT && AABBvsAABB(playerEquip->collisionBox, theEnemy.collisionBox)) //Success
        {
            theEnemy.health -= (int)playerEquip->damage;
            std::cout << "Attack landed" << std::endl;
            std::cout << theEnemy.health << std::endl;


            playerEquip->weaponHIT = true;

            if (player.GetWeaponSet().extraEffect == Status_Effect_System::Weapon_Status_Effect::DRAINING)
            {
                player.GetCurrentHealth() = min(player.GetCurrentHealth() + player.GetWeaponSet().damage, player.GetMaxHealth());
            }
        }


    }
}