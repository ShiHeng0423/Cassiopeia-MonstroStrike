
//#include "Player.h"
//#include "AEEngine.h"
//#include "Physics.h"
//#include "Armor_add.h"
//#include "Weapon.h"
//#include <iostream>
//#include <chrono>
//#include <queue>
//
////code will come here once per each trigger
////but you want this part of code to be called til the attack is done.
////then you need to make separate function and call it on here.
////
////Only happens in 1 frame
//
//using AnimationFunction = void(*)(Player& player, f32 elapsedTime);
//constexpr f32 comboWindowDuration = 1.0f;
//
//void *TriggerAttack(Player& player)
//{
//	player.attackTime -= AEFrameRateControllerGetFrameTime() * 3.f; //Constant here is speed scaling
//	f32 attackProgress = 1.0f - (player.attackTime / comboWindowDuration);
//	UpdateWeaponHitBoxTrig(&player, player.isFacingRight, &player.equippedWeapon, attackProgress);
//}
