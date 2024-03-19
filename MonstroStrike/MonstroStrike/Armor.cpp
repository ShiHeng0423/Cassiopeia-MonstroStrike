#include "Player.h"
#include "Armor.h"

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
        switch (player.piece[loop].rarity)
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

        bonusHealth += player.piece[loop].boost;
    }

    int setID = Armor_System::ARMOR_GRADE::TIER_1;
    while (setEffect > 0)
    {
        if (setEffect % 10 >= 2)
        {
            if (setEffect == 4)
            {
                bonusHealth += ArmorSetBonusInformation(setID, true);
            }
            else
            {
                bonusHealth += ArmorSetBonusInformation(setID, false);
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


f32 ArmorSetBonusInformation(int armorSetID, bool fullSetBonus)
{
    if (fullSetBonus)
    {
        switch (armorSetID)
        {

        case Armor_System::ARMOR_GRADE::TIER_1:
            return 80;
            break;
        case Armor_System::ARMOR_GRADE::TIER_2:
            return 150;
            break;
        case Armor_System::ARMOR_GRADE::TIER_3:
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
    }

    return 0;
}