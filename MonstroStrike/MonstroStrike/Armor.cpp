/*!************************************************************************
  \file					Armor.cpp
  \project name			Monstrostrike
  \primary author		Teng Shi heng (100%)
  \brief				File containing definitions of all the functions
						declared in Armor.h files

All content � 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#include "Player.h"
#include "Armor.h"
#include "Inventory.h"

//status effect duration
f32 REGEN_TIME = 2.0f;
f32 BURN_TIME = 3.0f;

//armor's extra status effect updates to player when same set of gears is equipped
void Armor_Effect_Update(class Player& player)
{
    player.GetArmorSet().effectTimer -= (f32)AEFrameRateControllerGetFrameTime();
    switch (player.GetArmorSet().extraEffect)
    {
    case Status_Effect_System::Status_Effect::BURNING:
    {
        if (player.GetArmorSet().effectTimer < 0.f)
        {
            player.GetArmorSet().effectTimer = BURN_TIME;

            if (player.GetCurrentHealth() - 5 < 1)
                player.DamageToPlayer(player.GetCurrentHealth() - 1);
            else
                player.DamageToPlayer(5);
        }
        break;
    }
    case Status_Effect_System::Status_Effect::REGEN:
    {
        if (player.GetArmorSet().effectTimer < 0.f)
        {
            player.GetArmorSet().effectTimer = REGEN_TIME;
            player.RecoverHpToPlayer(10);
        }
        break;
    }
    default:
        break;
    }
}

//updates the player stats when unequipping & equipping an armor
void Equip_Armor(class Player& player, Armor_System::ARMOR_TYPE newArmorType, Armor_System::ARMOR_GRADE newArmorGrade)
{
	player.GetArmorSet().pieces[newArmorType] = ArmorInformation(newArmorType, newArmorGrade);

	player.GetMaxHealth() = PlayerMaxBasehealth + Check_Set_Effect(player);

	if (player.GetCurrentHealth() > player.GetMaxHealth())
		player.GetCurrentHealth() = player.GetMaxHealth();
}

//Calculates all the armor stats and bonus stats stats granted to player  
int Check_Set_Effect(class Player& player)
{
	int setEffect = 0; /// xxx ->Tier3,2,1
	int bonusHealth = 0;
	for (size_t loop = 0; loop < 4; loop++)
	{
		switch (player.GetArmorSet().pieces[loop].rarity)
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

		bonusHealth += player.GetArmorSet().pieces[loop].boost;
	}

	int setID = Armor_System::ARMOR_GRADE::TIER_1;

	Status_Effect_System::Status_Effect currEffect = player.GetArmorSet().extraEffect;
	while (setEffect > 0)
	{
		if (setEffect % 10 >= 2)
		{
			if (setEffect == 4)
			{
				bonusHealth += ArmorSetBonusInformation(setID, true, player.GetArmorSet().extraEffect);
			}
			else
			{
				bonusHealth += ArmorSetBonusInformation(setID, false, player.GetArmorSet().extraEffect);
			}
		}
		setID++;
		setEffect /= 10;
	}
	if (currEffect != player.GetArmorSet().extraEffect)
	{
		if (currEffect != Status_Effect_System::Status_Effect::NONE_EFFECT)
		{
			size_t index = 0;
			for (std::pair<Status_Effect_System::Status_Effect, Status_Effect_System::Status_Effect_Source> effect :
			     player.playerStatusEffectList)
			{
				if (effect.second == Status_Effect_System::Status_Effect_Source::ARMOR)
				{
					std::cout << index << std::endl;
					break;
				}
				index++;
			}
			player.playerStatusEffectList.erase(player.playerStatusEffectList.begin() + index);
		}
		if (player.GetArmorSet().extraEffect != Status_Effect_System::Status_Effect::NONE_EFFECT)
		{
			player.playerStatusEffectList.push_back({
				player.GetArmorSet().extraEffect, Status_Effect_System::Status_Effect_Source::ARMOR
			});
		}
	}

	std::cout << currEffect << std::endl << std::endl;
	return bonusHealth;
}

//fetch armor info from the json
Armor_System::Armor ArmorInformation(Armor_System::ARMOR_TYPE type, Armor_System::ARMOR_GRADE grade)
{
	Armor_System::Armor tmp;
	tmp.rarity = grade;
	tmp.type = type;

	if (grade == Armor_System::ARMOR_GRADE::NO_GRADE)
		tmp.boost = 0;
	else
	{
		int ID = grade * GL_Total + type;
		if (type == Armor_System::ARMOR_TYPE::FOOT || type == Armor_System::ARMOR_TYPE::LEGS)
			ID += 1;
		tmp.boost = fullInventoryList[ID].health;
	}

	return tmp;
}

//determine what kind of effects granted to player upon achieving 4 pieces of armor that is of the rarity / grade
int ArmorSetBonusInformation(int armorSetID, bool fullSetBonus, Status_Effect_System::Status_Effect& effect)
{
	if (fullSetBonus)
	{
		switch (armorSetID)
		{
		case Armor_System::ARMOR_GRADE::TIER_1:
			effect = Status_Effect_System::REGEN;
			return 80;
			break;
		case Armor_System::ARMOR_GRADE::TIER_2:
			effect = Status_Effect_System::BURNING;
			return 150;
			break;
		case Armor_System::ARMOR_GRADE::TIER_3:
			effect = Status_Effect_System::NONE_EFFECT;
			return 250;
			break;
		default:
			break;
		}
	}
	else
	{
		effect = Status_Effect_System::NONE_EFFECT;
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
