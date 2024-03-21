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

float xHitOffset = 0.0f;
float yHitOffset = 0.0f;
float xScale = 0.0f;
float yScale = 0.0f;
float xScaler= 0.0f;

void UpdateWeaponHitBoxTrig(class Player* player, bool playerFacingRight, struct Weapon* playerEquip, f32 attackProgress)
{

    if (player->equippedWeapon.name == "Short-Sword")
    {
        //Initializes weapon's position
    
        player->equippedWeapon.damage = 10;

        if (player->comboState == 0)
        {
            xHitOffset = 10.0f;
            yHitOffset = 0.0f;
            xScale = 30.0f;
            yScale = 20.0f;

            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;

            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
            playerEquip->position.x = player->obj.pos.x + xHitOffset: playerEquip->position.x = player->obj.pos.x - xHitOffset;
            playerEquip->position.y = player->obj.pos.y;

            AEVec2Set(&player->equippedWeapon.scale, xScale, yScale);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x: -playerEquip->scale.x;

            playerEquip->position.x += xOffset;
            playerEquip->position.y += playerEquip->scale.y * 0.5f * (1.0f - attackProgress * 2);

            playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->scale.x;
            playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->scale.y;
            playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->scale.x;
            playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->scale.y;

        
            playerEquip->weaponHIT = false;


        }
        else if (player->comboState == 1)
        {
            xHitOffset = 10.0f;
            yHitOffset = 10.0f;
            xScale = 30.0f;
            yScale = 20.0f;

            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;
            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + xHitOffset : playerEquip->position.x = player->obj.pos.x - xHitOffset;
            playerEquip->position.y = player->obj.pos.y - yHitOffset;

            AEVec2Set(&player->equippedWeapon.scale, xScale, yScale);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x  : -playerEquip->scale.x ;

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
            xHitOffset = 10.0f;
            yHitOffset = 20.0f;
            xScale = 30.0f;
            yScale = 20.0f;
            

            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + xHitOffset : playerEquip->position.x = player->obj.pos.x - xHitOffset;
            playerEquip->position.y = player->obj.pos.y - yHitOffset;

            AEVec2Set(&player->equippedWeapon.scale, xScale, yScale);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x  : -playerEquip->scale.x;

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

    else if (player->equippedWeapon.name == "Broad-Sword")
    {
        //Initializes weapon's position
        playerEquip->position.x = player->obj.pos.x;
        playerEquip->position.y = player->obj.pos.y;
        player->equippedWeapon.damage = 20;

        if (player->comboState == 0)
        {
            xHitOffset = 0.0f;
            yHitOffset = 27.0f;
            xScale = 45.0f;
            yScale = 55.0f;

            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;

            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + xHitOffset : playerEquip->position.x = player->obj.pos.x - xHitOffset;
            playerEquip->position.y = player->obj.pos.y - yHitOffset;

            AEVec2Set(&player->equippedWeapon.scale, xScale, yScale);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x : -playerEquip->scale.x ;

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
            xHitOffset = 0.0f;
            yHitOffset = 27.0f;
            xScale = 45.0f;
            yScale = 55.0f;

            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + xHitOffset : playerEquip->position.x = player->obj.pos.x - xHitOffset;
            playerEquip->position.y = player->obj.pos.y - yHitOffset;

            AEVec2Set(&player->equippedWeapon.scale, xScale, yScale);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x  : -playerEquip->scale.x ;

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
            xHitOffset = 0.0f;
            yHitOffset = 20.0f;
            xScale = 60.0f;
            yScale = 40.0f;
            xScaler = 0.9f;

            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + xHitOffset : playerEquip->position.x = player->obj.pos.x - xHitOffset;
            playerEquip->position.y = player->obj.pos.y - yHitOffset;

            AEVec2Set(&player->equippedWeapon.scale, xScale, yScale);

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
    else if (player->equippedWeapon.name == "GreatSword")
    {
        player->equippedWeapon.damage = 20;
        if (player->comboState == 0)
        {
            xHitOffset = 0.0f;
            yHitOffset = 30.0f;
            xScale = 60.0f;
            yScale = 60.0f;
            xScaler = 0.9f;
            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;

            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + xHitOffset : playerEquip->position.x = player->obj.pos.x - xHitOffset;
            playerEquip->position.y = player->obj.pos.y - yHitOffset;

            AEVec2Set(&player->equippedWeapon.scale, xScale, yScale);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x * xScaler : -playerEquip->scale.x * xScaler;

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
            xHitOffset = 0.0f;
            yHitOffset = 30.0f;
            xScale = 60.0f;
            yScale = 60.0f;
            xScaler = 0.9f;

            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;

            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + xHitOffset : playerEquip->position.x = player->obj.pos.x - xHitOffset;
            playerEquip->position.y = player->obj.pos.y - yHitOffset;

            AEVec2Set(&player->equippedWeapon.scale, xScale, yScale);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x * xScaler : -playerEquip->scale.x * xScaler;

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
            xHitOffset = 0.0f;
            yHitOffset = 30.0f;
            xScale = 80.0f;
            yScale = 60.0f;
            xScaler = 0.8f;

            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;


            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + xHitOffset : playerEquip->position.x = player->obj.pos.x -xHitOffset;
            playerEquip->position.y = player->obj.pos.y - yHitOffset;

            AEVec2Set(&player->equippedWeapon.scale, 80.f, 60.f);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x * xScaler: -playerEquip->scale.x * xScaler;

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

void UpdateWeaponHitBoxHeld(class Player* player, bool playerFacingRight, struct Weapon* playerEquip, f32 attackProgress)
{
    if (player->equippedWeapon.name == "Short-Sword")
    {
        playerEquip->damage = 20;

        if (player->comboState == 2)
        {
            xHitOffset = 10.0f;
            yHitOffset = 30.0f;
            xScale = 30.0f;
            yScale = 50.0f;

            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;

            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + xHitOffset : playerEquip->position.x = player->obj.pos.x - xHitOffset;
            playerEquip->position.y = player->obj.pos.y - yHitOffset;

            AEVec2Set(&player->equippedWeapon.scale, xScale, yScale);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x : -playerEquip->scale.x;

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
    if (player->equippedWeapon.name == "Broad-Sword")
    {
        playerEquip->damage = 40;
        if (player->comboState == 2)
        {

            xHitOffset = 0.0f;
            yHitOffset = 30.0f;
            xScale = 80.0f;
            yScale = 70.0f;
            xScaler = 0.7f;

            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;


            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + xHitOffset : playerEquip->position.x = player->obj.pos.x - xHitOffset;
            playerEquip->position.y = player->obj.pos.y - yHitOffset;

            AEVec2Set(&player->equippedWeapon.scale, xScale, yScale);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x * xScaler : -playerEquip->scale.x *xScaler;

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
    if (player->equippedWeapon.name == "GreatSword")
    {
        playerEquip->damage = 100;

        if (player->comboState == 2)
        {
            xHitOffset = 0.0f;
            yHitOffset = 30.0f;
            xScale = 100.0f;
            yScale = 80.0f;
            xScaler = 0.7f;

            //Initializes weapon's position
            playerEquip->position.x = player->obj.pos.x;
            playerEquip->position.y = player->obj.pos.y;

            // change the hitbox
            f32 hit = playerFacingRight ?  // change the hitbox
                playerEquip->position.x = player->obj.pos.x + xHitOffset : playerEquip->position.x = player->obj.pos.x - xHitOffset;
            playerEquip->position.y = player->obj.pos.y - yHitOffset;

            AEVec2Set(&player->equippedWeapon.scale, xScale, yScale);

            f32 xOffset = playerFacingRight ? playerEquip->scale.x * xScaler : -playerEquip->scale.x * xScaler;

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
    else
    {
        std::cout << "Placeholder";

    }
}


void CheckWeaponCollision(struct Weapon* playerEquip, struct Enemy& theEnemy, class Player& player)
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
