#include "Player.h"
#include <string>
#include "Weapon.h"

Weapon createWeapon(const std::string& name, float x, float y) {
    Weapon weapon;
    weapon.name = name;
    weapon.damage = 10;
    weapon.position.x = x;
    weapon.position.y = y;
    return weapon;
}