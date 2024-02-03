#pragma once
#include <string>
#include "Player.h"
struct Position {
    float x;
    float y;
};

struct Weapon {
    std::string name;
    int damage;
    Position position;
};

Weapon createWeapon(const std::string& name, float x, float y);