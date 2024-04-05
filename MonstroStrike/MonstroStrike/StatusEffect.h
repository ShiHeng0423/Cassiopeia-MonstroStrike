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