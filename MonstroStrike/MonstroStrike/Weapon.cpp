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
    weapon.damage = 50;
    weapon.position.x = x;
    weapon.position.y = y;
    return weapon;
}

void UpdateWeaponHitBox(struct Player* player, bool playerFacingRight, struct Weapon* playerEquip, f32 attackProgress)
{
    // Calculate the offset based on attack progress
    f32 yOffset = playerEquip->Scale.y * 0.5f * (1.0f - attackProgress * 2);

    // Use sine function for circular ease-out motion
    f32 xOffset = playerFacingRight
        ? attackProgress * playerEquip->Scale.x * 2.f * cos(attackProgress * M_PI / 2.0f)
        : -attackProgress * playerEquip->Scale.x * 2.f * cos(attackProgress * M_PI / 2.0f);

    // Set the weapon's position
    playerEquip->position.x = player->obj.pos.x + xOffset * 4;
    playerEquip->position.y = player->obj.pos.y + yOffset * 5;

    //Resetting main AABB box...
    playerEquip->collisionBox.minimum.x = playerEquip->position.x - playerEquip->Scale.x * 0.5f;
    playerEquip->collisionBox.minimum.y = playerEquip->position.y - playerEquip->Scale.y * 0.5f;
    playerEquip->collisionBox.maximum.x = playerEquip->position.x + playerEquip->Scale.x * 0.5f;
    playerEquip->collisionBox.maximum.y = playerEquip->position.y + playerEquip->Scale.y * 0.5f;
}

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