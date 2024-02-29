#include "Player.h"
#include <string>
#include "Weapon.h"
#include "CollisionShape.h"
#include <iostream>
#include "Enemy.h"
#include <AEMath.h>


#define M_PI 3.1415
Weapon createWeapon(const std::string& name, float x, float y) {
    Weapon weapon;
    weapon.name = name;
    weapon.damage = 5;
    weapon.position.x = x;
    weapon.position.y = y;
    return weapon;
}



void UpdateWeaponHitBoxTrig(struct Player* player, bool playerFacingRight, struct Weapon* playerEquip, f32 attackProgress)
{
    if (player->comboState == 0)
    {
        /*std::cout << "Entering Input Check - Combo State1: " << player->comboState << ", Combo Time1: " << player->comboTime << std::endl;*/
        // Calculate the offset based on attack progress
        f32 yOffset = playerEquip->Scale.y * 0.5f * (1.0f - attackProgress * 2);

        // Use sine function for circular ease-out motion
        f32 xOffset = playerFacingRight
            ? attackProgress * playerEquip->Scale.x * 0.5f * cos(attackProgress * M_PI / 2.0f)
            : -attackProgress * playerEquip->Scale.x * 0.5f * cos(attackProgress * M_PI / 2.0f);

        // Set the weapon's position
        playerEquip->position.x = player->obj.pos.x + xOffset * 4;
        playerEquip->position.y = player->obj.pos.y + yOffset * 5;
     

        //Resetting main AABB box...
        playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->Scale.x * 0.5f;
        playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->Scale.y * 0.5f;
        playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->Scale.x * 0.5f;
        playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->Scale.y * 0.5f;

        //playerEquip->weaponHIT = false;  // Reset the hit state for each combo
        /*std::cout << "Exiting Input Check - Combo State1: " << player->comboState << ", Combo Time1: " << player->comboTime << std::endl;*/



    }
    else if(player->comboState == 1)
    {
           /* std::cout << "Entering Input Check - Combo State2: " << player->comboState << ", Combo Time2: " << player->comboTime << std::endl;*/

            // Calculate the offset based on attack progress
            f32 yOffset = playerEquip->Scale.y * 0.5f * (1.0f - attackProgress * 2);


            f32 xOffset = playerFacingRight
                ? attackProgress * playerEquip->Scale.x * 0.5f * cos(attackProgress * M_PI / 2.0f)
                : attackProgress * playerEquip->Scale.x * 0.5f * cos(attackProgress * M_PI / 2.0f);  // Change the sign

            // Set the weapon's position
            playerEquip->position.x = player->obj.pos.x + xOffset * 4;
            playerEquip->position.y = player->obj.pos.y + yOffset * 5;
           

            //Resetting main AABB box...
            playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->Scale.x * 0.5f;
            playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->Scale.y * 0.5f;
            playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->Scale.x * 0.5f;
            playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->Scale.y * 0.5f;

            //playerEquip->weaponHIT = false;  // Reset the hit state for each combo

          /*  std::cout << "Exiting Input Check - Combo State2: " << player->comboState << ", Combo Time2: " << player->comboTime << std::endl;*/

    


    }
    else if (player->comboState == 2)
    {
        /*std::cout << "Entering Input Check - Combo State3: " << player->comboState << ", Combo Time3: " << player->comboTime << std::endl;*/

        // Calculate the offset based on attack progress
        f32 yOffset = playerEquip->Scale.y * 0.5f * (1.0f - attackProgress * 2);


        f32 xOffset = playerFacingRight
            ? attackProgress * playerEquip->Scale.x * 0.5f * sin(attackProgress * M_PI / 2.0f)
            : attackProgress * playerEquip->Scale.x * 0.5f * sin(attackProgress * M_PI / 2.0f);  // Change the sign

        // Set the weapon's position
        playerEquip->position.x = player->obj.pos.x + xOffset * 4;
        playerEquip->position.y = player->obj.pos.y + yOffset * 5;
       

        //Resetting main AABB box...
        playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->Scale.x * 0.5f;
        playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->Scale.y * 0.5f;
        playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->Scale.x * 0.5f;
        playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->Scale.y * 0.5f;

        //playerEquip->weaponHIT = false;  // Reset the hit state for each combo
        

        /*std::cout << "Exiting Input Check - Combo State3: " << player->comboState << ", Combo Time3: " << player->comboTime << std::endl;*/

    }

    std::cout << "Combo: " << player->comboState << std::endl;

  }

void UpdateWeaponHitBoxHeld(struct Player* player, bool playerFacingRight, struct Weapon* playerEquip, f32 attackProgress)
{
    if (player->comboState == 1)
    {
        std::cout << "Entering Input Check - Combo StateHeld: " << player->comboState << ", Combo TimeHeld: " << player->comboTime << std::endl;
        // Calculate the offset based on attack progress
        f32 yOffset = playerEquip->Scale.y * 0.5f * (1.0f - attackProgress * 2);

        // Use sine function for circular ease-out motion
        f32 xOffset = playerFacingRight
            ? attackProgress * playerEquip->Scale.x * 0.5f * sin(attackProgress * M_PI / 2.0f)
            : -attackProgress * playerEquip->Scale.x * 0.5f * sin(attackProgress * M_PI / 2.0f);

        // Set the weapon's position
        playerEquip->position.x = player->obj.pos.x + xOffset * 4;
        playerEquip->position.y = player->obj.pos.y + yOffset * 5;
        

        //Resetting main AABB box...
        playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->Scale.x * 0.5f;
        playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->Scale.y * 0.5f;
        playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->Scale.x * 0.5f;
        playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->Scale.y * 0.5f;


        std::cout << "Exiting Input Check - Combo StateHeld: " << player->comboState << ", Combo TimeHeld: " << player->comboTime << std::endl;



    }
    else {
        std::cout << "Placeholder";
    }
}

void CheckWeaponCollision(struct Weapon* playerEquip, struct Enemy& theEnemy, struct Player& player)
{
    if (!playerEquip->weaponHIT && AABBvsAABB(playerEquip->collisionBox, theEnemy.collisionBox)) //Success
    {
        theEnemy.health -= playerEquip->damage;
        std::cout << "Attack landed" << std::endl;
        std::cout << theEnemy.health << std::endl;
        playerEquip->weaponHIT = true;
    }
}