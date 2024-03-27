#include "Player.h"
#include "Armor.h"

f32 REGEN_TIME = 2.0f;
f32 BURN_TIME = 3.0f;

void Equip_Weapon(class Player& player, Weapon_System::Weapon& playerCurrWeapon, Weapon_System::WEAPON_GRADE newWeaponGrade)
{
    Weapon_System::Weapon tmp;
    tmp.rarity = newWeaponGrade;

    switch (newWeaponGrade)
    {
    case Weapon_System::WEAPON_GRADE::TIER_1:
        player.equippedWeapon = createWeapon("Short-Sword", player.expectedLocation.x, player.expectedLocation.y);
        player.equippedWeapon.damage = 15;
        tmp.boost = 15;

        break;
    case Weapon_System::WEAPON_GRADE::TIER_2:
        player.equippedWeapon = createWeapon("Broad-Sword", player.expectedLocation.x, player.expectedLocation.y);
        tmp.boost = 10;
        player.equippedWeapon.damage = 10;
        player.weaponExtraEffect = Status_Effect_System::DRAINING;
        break;
    case Weapon_System::WEAPON_GRADE::TIER_3:
        player.equippedWeapon = createWeapon("GreatSword", player.expectedLocation.x, player.expectedLocation.y);
        tmp.boost = 35;
        player.equippedWeapon.damage = 35;
        break;
    default:
        break;
    }

    player.wp = tmp;
}

void Armor_Effect_Update(class Player& player)
{
    player.armorSet.effectTimer -= AEFrameRateControllerGetFrameTime();
    switch (player.armorExtraEffect)
    {
    case Status_Effect_System::Armor_Status_Effect::BURNING:
    {
        if (player.armorSet.effectTimer < 0.f)
        {
            player.armorSet.effectTimer = BURN_TIME;
            player.currHealth = max(1, player.currHealth - 5);
        }
        break;
    }
    case Status_Effect_System::Armor_Status_Effect::REGEN:
    {
        if (player.armorSet.effectTimer < 0.f)
        {
            player.armorSet.effectTimer = REGEN_TIME;
            player.currHealth = min(player.currHealth + 10, player.maxHealth);
        }
        break;
    }
    default:
        break;
    }
}

void Equip_Armor(class Player& player, Armor_System::Armor& playerCurrArmor, Armor_System::ARMOR_TYPE newArmorType, Armor_System::ARMOR_GRADE newArmorGrade)
{
    playerCurrArmor = ArmorInformation(newArmorType, newArmorGrade);

    player.maxHealth = PlayerMaxBasehealth + Check_Set_Effect(player);

    if (player.currHealth > player.maxHealth)
        player.currHealth = player.maxHealth;
}

f32 Check_Set_Effect(class Player& player)
{
    int setEffect = 0; /// xxx ->Tier3,2,1
    f32 bonusHealth = 0;
    for (size_t loop = 0; loop < 4; loop++)
    {
        switch (player.armorSet.pieces[loop].rarity)
        {
        case Armor_System::ARMOR_GRADE::TIER_1:
            setEffect += 1;
            break;
        case Armor_System::ARMOR_GRADE::TIER_2:
            setEffect += 10;
            break;
        case Armor_System::ARMOR_GRADE::TIER_3:
            setEffect += 100;
            break;
        default:
            break;
        }

        bonusHealth += player.armorSet.pieces[loop].boost;
    }

    int setID = Armor_System::ARMOR_GRADE::TIER_1;
    while (setEffect > 0)
    {
        if (setEffect % 10 >= 2)
        {
            if (setEffect == 4)
            {
                bonusHealth += ArmorSetBonusInformation(setID, true, player.armorExtraEffect);
            }
            else
            {
                bonusHealth += ArmorSetBonusInformation(setID, false, player.armorExtraEffect);
            }
        }
        setID++;
        setEffect /= 10;
    }

    return bonusHealth;
}

Armor_System::Armor ArmorInformation(Armor_System::ARMOR_TYPE type, Armor_System::ARMOR_GRADE grade)
{
    Armor_System::Armor tmp;
    tmp.rarity = grade;
    tmp.type = type;

    switch (type)
    {
    case Armor_System::ARMOR_TYPE::HEAD:
    {
        switch (grade)
        {
        case Armor_System::ARMOR_GRADE::TIER_1:
            tmp.boost = 15;
            break;
        case Armor_System::ARMOR_GRADE::TIER_2:
            tmp.boost = 25;
            break;
        case Armor_System::ARMOR_GRADE::TIER_3:
            tmp.boost = 50;
            break;
        default:
            break;
        }

        break;
    }
    case Armor_System::ARMOR_TYPE::BODY:
    {
        switch (grade)
        {
        case Armor_System::ARMOR_GRADE::TIER_1:
            tmp.boost = 50;
            break;
        case Armor_System::ARMOR_GRADE::TIER_2:
            tmp.boost = 100;
            break;
        case Armor_System::ARMOR_GRADE::TIER_3:
            tmp.boost = 200;
            break;
        default:
            break;
        }

        break;
    }
    case Armor_System::ARMOR_TYPE::LEGS:
    {
        switch (grade)
        {
        case Armor_System::ARMOR_GRADE::TIER_1:
            tmp.boost = 20;
            break;
        case Armor_System::ARMOR_GRADE::TIER_2:
            tmp.boost = 50;
            break;
        case Armor_System::ARMOR_GRADE::TIER_3:
            tmp.boost = 80;
            break;
        default:
            break;
        }

        break;
    }
    case Armor_System::ARMOR_TYPE::FOOT:
    {
        switch (grade)
        {
        case Armor_System::ARMOR_GRADE::TIER_1:
            tmp.boost = 10;
            break;
        case Armor_System::ARMOR_GRADE::TIER_2:
            tmp.boost = 20;
            break;
        case Armor_System::ARMOR_GRADE::TIER_3:
            tmp.boost = 40;
            break;
        default:
            break;
        }

        break;
    }
    default:
        break;
    }

    return tmp;
}


f32 ArmorSetBonusInformation(int armorSetID, bool fullSetBonus, Status_Effect_System::Armor_Status_Effect& effect)
{
    if (fullSetBonus)
    {
        switch (armorSetID)
        {

        case Armor_System::ARMOR_GRADE::TIER_1:
            effect = Status_Effect_System::NONE_ARMOR_EFFECT;
            return 80;
            break;
        case Armor_System::ARMOR_GRADE::TIER_2:
            effect = Status_Effect_System::BURNING;
            return 150;
            break;
        case Armor_System::ARMOR_GRADE::TIER_3:
            effect = Status_Effect_System::REGEN;
            return 250;
            break;
        default:
            break;

        }
    }
    else
    {
        //half set bonus
        switch (armorSetID)
        {

        case Armor_System::ARMOR_GRADE::TIER_1:
            return 30;
            break;
        case Armor_System::ARMOR_GRADE::TIER_2:
            return 100;
            break;
        case Armor_System::ARMOR_GRADE::TIER_3:
            return 200;
            break;
        default:
            break;

        }
        effect = Status_Effect_System::NONE_ARMOR_EFFECT;
    }

    return 0;
}