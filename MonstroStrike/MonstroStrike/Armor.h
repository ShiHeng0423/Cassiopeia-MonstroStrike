/*!************************************************************************
  \file					Armor.h
  \project name			Monstrostrike
  \primary author		Teng Shi heng (100%)
  \brief				File containing the namespace that encapsulates 
						functionality related to the armor system within 
						the game. 
						
						It defines enums for armor grades and types, 
						as well as structs for individual armor pieces 
						and complete armor sets. 
						
						Additionally, this file declares functions for retrieving
						armor information, managing armor sets, and updating
						armor effects for player object.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#pragma once
#include "AEEngine.h"
#include "Player.h"
#include "StatusEffect.h"

namespace Armor_System
{
	enum ARMOR_GRADE
	{
		TIER_1,
		TIER_2,
		TIER_3,

		NO_GRADE
	};

	enum ARMOR_TYPE
	{
		HEAD,
		BODY,
		LEGS,
		FOOT,

		NO_TYPE
	};

	struct Armor
	{
		int boost{0};
		ARMOR_GRADE rarity{ARMOR_GRADE::NO_GRADE};
		ARMOR_TYPE type{ARMOR_TYPE::NO_TYPE};
	};

	struct Armor_Set
	{
		Armor pieces[4];
		Status_Effect_System::Status_Effect extraEffect{Status_Effect_System::Status_Effect::NONE_EFFECT};
		f32 effectTimer{0.f};
	};
}


Armor_System::Armor ArmorInformation(Armor_System::ARMOR_TYPE type, Armor_System::ARMOR_GRADE grade);

int ArmorSetBonusInformation(int armorSetID, bool fullSetBonus, Status_Effect_System::Status_Effect& effect);

void Equip_Armor(class Player& player, Armor_System::ARMOR_TYPE newArmorType, Armor_System::ARMOR_GRADE newArmorGrade);

int Check_Set_Effect(class Player& player);

void Armor_Effect_Update(class Player& player);
