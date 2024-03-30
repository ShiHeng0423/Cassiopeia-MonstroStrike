#pragma once
namespace Status_Effect_System
{
	enum Status_Effect {
		BURNING,
		REGEN,
		DRAINING,

		NONE_EFFECT
	};

	enum Status_Effect_Source {
		ARMOR,
		WEAPON,
		CONSUMABLES,
		ENEMY,

		NONE
	};
}