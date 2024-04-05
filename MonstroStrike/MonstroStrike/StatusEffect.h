/*!************************************************************************
  \file					StatusEffect.h
  \project name			Monstrostrike
  \primary author		Teng Shi heng (100%)
  \brief				This file contains the declaration a namespace
						that encapsulates the status effect system for a 
						game or application.
						
						It defines two enums: Status_Effect and 
						Status_Effect_Source which represents what type of
						status effects its being applied and who is the 
						originate source that inflicts the status effect.


All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#pragma once
namespace Status_Effect_System
{
	enum Status_Effect {
		BURNING,
		REGEN,
		LIFE_STEAL,
		POISON,
		SLOW,

		NONE_EFFECT
	};

	enum Status_Effect_Source {
		ARMOR,
		WEAPON,
		CONSUMABLES,
		ENEMY,
		TRAPS,

		NONE
	};
}