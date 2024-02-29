#pragma once
#include <string>
#include "Player.h"
#include "CollisionShape.h"
#include "Enemy.h"

struct Position {
    float x;
    float y;
};

struct Weapon {
    std::string name;
    int damage = 50.f;
    Position position;

    AABB collisionBox;
    AEVec2 Scale;
    AEVec2 transformation;
    bool weaponHIT;

};

Weapon createWeapon(const std::string& name, float x, float y);
void UpdateWeaponHitBoxTrig(struct Player* player, bool playerFacingRight, struct Weapon* playerEquip, f32);
void UpdateWeaponHitBoxHeld (struct Player* player, bool playerFacingRight, struct Weapon* playerEquip, f32);
void CheckWeaponCollision(struct Weapon* playerEquip, struct Enemy& theEnemy, struct Player& player);
