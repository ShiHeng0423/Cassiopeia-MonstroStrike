#include <AEMath.h>
#include <thread>
#include <chrono>
#include "Player.h"

#define M_PI 3.1415
Weapon* createWeapon(const std::string& name, float x, float y) {
    Weapon* weapon = new Weapon();
    weapon->name = name;
    weapon->damage;
    weapon->position.x = x;
    weapon->position.y = y;



    return weapon;
};




void UpdateWeaponHitBoxTrig(struct Player* player, bool playerFacingRight, struct Weapon* playerEquip, f32 attackProgress)
{
    if (player->equippedWeapon->name == "Short-Sword")
    {
        player->equippedWeapon->damage = 10;
        if (player->comboState == 0)
        {

            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;

            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + 20.0f : playerEquip->position.x = player->obj.pos.x - 20.0f;
            playerEquip->position.y = player->obj.pos.y;

            AEVec2Set(&player->equippedWeapon->scale, 30.f, 20.f);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x * 1.0f : -playerEquip->scale.x * 1.0f;

            playerEquip->position.x += xOffset;
            playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

            playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
            playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;


            //player->isAttacking = false; //If player is attacking render the hitbox
            playerEquip->weaponHIT = false;

<<<<<<< Updated upstream
     /*   playerEquip->collisionBox.minimum.x = xOffset - playerEquip->scale.x * 5.5f;
        playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 5.5f;
        playerEquip->collisionBox.maximum.x = xOffset + playerEquip->scale.x * 5.5f;
        playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 5.5f;*/
=======
>>>>>>> Stashed changes

        }
        else if (player->comboState == 1)
        {
            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;

<<<<<<< Updated upstream
        ///*std::cout << "Entering Input Check - Combo State1: " << player->comboState << ", Combo Time1: " << player->comboTime << std::endl;*/
        //// Calculate the offset based on attack progress
        //f32 yOffset = playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

        //// Use sine function for circular ease-out motion
        //f32 xOffset = playerFacingRight
        //    ? attackProgress * playerEquip->scale.x * 0.5f * cos(attackProgress * M_PI / 2.0f)
        //    : -attackProgress * playerEquip->scale.x * 0.5f * cos(attackProgress * M_PI / 2.0f);
=======
            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + 20.0f : playerEquip->position.x = player->obj.pos.x - 20.0f;
            playerEquip->position.y = player->obj.pos.y;
            playerEquip->position.y = player->obj.pos.y - 10.0f;

            AEVec2Set(&player->equippedWeapon->scale, 30.f, 20.f);
>>>>>>> Stashed changes

            f32 xOffset = playerFacingRight ? playerEquip->scale.x * 1.0f : -playerEquip->scale.x * 1.0f;

            playerEquip->position.x += xOffset;
            playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

<<<<<<< Updated upstream
        ////Resetting main AABB box...
        //playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
        //playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
        //playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
        //playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;
=======
            playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
            playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;
>>>>>>> Stashed changes

            //player->isAttacking = false;
            playerEquip->weaponHIT = false;



        }
        else if (player->comboState == 2)
        {

            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;


            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + 20.0f : playerEquip->position.x = player->obj.pos.x - 20.0f;
            playerEquip->position.y = player->obj.pos.y;
            playerEquip->position.y = player->obj.pos.y - 20.0f;

            AEVec2Set(&player->equippedWeapon->scale, 30.f, 20.f);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x * 1.0f : -playerEquip->scale.x * 1.0f;

            playerEquip->position.x += xOffset;
            playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

            playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
            playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;

            std::cout << playerEquip->damage << std::endl;
            /*player->isAttacking = false;*/
            playerEquip->weaponHIT = false;



        }
    }

    else if (player->equippedWeapon->name == "Broad-Sword")
    {
        player->equippedWeapon->damage = 20;
        if (player->comboState == 0)
        {

            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;

            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + 13.0f : playerEquip->position.x = player->obj.pos.x - 13.0f;
            playerEquip->position.y = player->obj.pos.y - 30.0f;

            AEVec2Set(&player->equippedWeapon->scale, 45.f, 55.0f);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x * 1.0f : -playerEquip->scale.x * 1.0f;

            playerEquip->position.x += xOffset;
            playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

            playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
            playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;


            //player->isAttacking = false; //If player is attacking render the hitbox
            playerEquip->weaponHIT = false;


        }
        else if (player->comboState == 1)
        {
            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;

            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + 13.0f : playerEquip->position.x = player->obj.pos.x - 13.0f;
            playerEquip->position.y = player->obj.pos.y - 30.0f;

            AEVec2Set(&player->equippedWeapon->scale, 45.f, 55.0f);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x * 1.0f : -playerEquip->scale.x * 1.0f;

            playerEquip->position.x += xOffset;
            playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

            playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
            playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;

            //player->isAttacking = false;
            playerEquip->weaponHIT = false;



        }
        else if (player->comboState == 2)
        {

            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;


            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + 7.0f : playerEquip->position.x = player->obj.pos.x - 7.0f;
            playerEquip->position.y = player->obj.pos.y - 20.0f;

            AEVec2Set(&player->equippedWeapon->scale, 60.f, 40.f);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x * 1.0f : -playerEquip->scale.x * 1.0f;

            playerEquip->position.x += xOffset;
            playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

            playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
            playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;

            std::cout << playerEquip->damage << std::endl;
            /*player->isAttacking = false;*/
            playerEquip->weaponHIT = false;
        }
    }
    else if (player->equippedWeapon->name == "GreatSword")
    {
        player->equippedWeapon->damage = 20;
        if (player->comboState == 0)
        {

            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;

            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + 13.0f : playerEquip->position.x = player->obj.pos.x - 13.0f;
            playerEquip->position.y = player->obj.pos.y - 30.0f;

            AEVec2Set(&player->equippedWeapon->scale, 60.f, 60.0f);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x * 1.0f : -playerEquip->scale.x * 1.0f;

            playerEquip->position.x += xOffset;
            playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

            playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
            playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;


            playerEquip->weaponHIT = false;


        }
        else if (player->comboState == 1)
        {
            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;

            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + 13.0f : playerEquip->position.x = player->obj.pos.x - 13.0f;
            playerEquip->position.y = player->obj.pos.y - 30.0f;

            AEVec2Set(&player->equippedWeapon->scale, 60.f, 60.0f);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x * 1.0f : -playerEquip->scale.x * 1.0f;

            playerEquip->position.x += xOffset;
            playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

            playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
            playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;

            playerEquip->weaponHIT = false;



        }
        else if (player->comboState == 2)
        {

            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;


            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + 7.0f : playerEquip->position.x = player->obj.pos.x - 7.0f;
            playerEquip->position.y = player->obj.pos.y - 30.0f;

            AEVec2Set(&player->equippedWeapon->scale, 80.f, 60.f);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x * 1.0f : -playerEquip->scale.x * 1.0f;

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

void UpdateWeaponHitBoxHeld(struct Player* player, bool playerFacingRight, struct Weapon* playerEquip, f32 attackProgress)
{
    if (player->equippedWeapon->name == "Short-Sword")
    {
        playerEquip->damage = 20;

        if (player->comboState == 2)
        {
            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;



            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + 20.0f : playerEquip->position.x = player->obj.pos.x - 20.0f;
            playerEquip->position.y = player->obj.pos.y - 30.0f;

            AEVec2Set(&player->equippedWeapon->scale, 30.f, 50.f);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x * 1.0f : -playerEquip->scale.x * 1.0f;

            playerEquip->position.x += xOffset;
            playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

            playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
            playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;

            std::cout << playerEquip->damage << std::endl;
            player->isAttacking = false;
            playerEquip->weaponHIT = false;
            playerEquip->damage = 20;
            std::cout << playerEquip->damage << std::endl;

        }

    }
    if (player->equippedWeapon->name == "Broad-Sword")
    {
        playerEquip->damage = 40;
        if (player->comboState == 2)
        {
            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;



            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + 5.0f : playerEquip->position.x = player->obj.pos.x - 5.0f;
            playerEquip->position.y = player->obj.pos.y - 30.0f;

            AEVec2Set(&player->equippedWeapon->scale, 80.f, 70.f);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x * 1.0f : -playerEquip->scale.x * 1.0f;

            playerEquip->position.x += xOffset;
            playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

            playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y * 0.5f;
            playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x * 0.5f;
            playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y * 0.5f;

            std::cout << playerEquip->damage << std::endl;
            player->isAttacking = false;
            playerEquip->weaponHIT = false;
            playerEquip->damage = 60;
            std::cout << playerEquip->damage << std::endl;

        }
    }

    else
    {
        std::cout << "Placeholder";

    }
}
<<<<<<< Updated upstream

=======
void deleteWeapon(class Player* player)
{
    delete player->equippedWeapon;
}
>>>>>>> Stashed changes
void CheckWeaponCollision(struct Weapon* playerEquip, struct Enemy& theEnemy, struct Player& player)
{
    //wings
    if (!playerEquip->weaponHIT && AABBvsAABB(playerEquip->collisionBox, theEnemy.wing1.collisionBox)) {
        theEnemy.wing1.health -= playerEquip->damage;
        std::cout << "Attack landed wing1" << std::endl;

        playerEquip->weaponHIT = true;
    }
    if (!playerEquip->weaponHIT && AABBvsAABB(playerEquip->collisionBox, theEnemy.wing2.collisionBox)) {
        theEnemy.wing2.health -= playerEquip->damage;
        std::cout << "Attack landed wing2" << std::endl;

        playerEquip->weaponHIT = true;
    }


    if (!playerEquip->weaponHIT && AABBvsAABB(playerEquip->collisionBox, theEnemy.collisionBox)) //Success
    {
        theEnemy.health -= playerEquip->damage;
        std::cout << "Attack landed" << std::endl;
        std::cout << theEnemy.health << std::endl;
        std::cout << theEnemy.enemyType << std::endl;

        playerEquip->weaponHIT = true;
    }


}