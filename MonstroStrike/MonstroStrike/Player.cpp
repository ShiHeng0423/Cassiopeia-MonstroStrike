#include "Player.h"
#include "AEEngine.h"
#include "Physics.h"
#include "Weapon.h"
#include "TriggerAttack.h"
#include <iostream>
#include <chrono>
#include <queue>
#include <functional>  // for std::function
#include "MapTransition.h"
#include "ParticleSystem.h"

#define CAM_X_BOUNDARY (250.f)
#define CAM_FOLLOW_UP_SPEED_X (0.05f)
// Define a clock type for high-resolution time measurement
using Clock = std::chrono::high_resolution_clock;

// Store the time point of the last input
auto lastInputTime = Clock::now();
auto currentTime = Clock::now();
auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastInputTime).count() / 1000.0; // Convert to seconds

constexpr f32 comboWindowDuration = 1.0f;
constexpr f32 PRESS_THRESHOLD = 0.5f;
//Attack hold and atack release

bool undealtTriggerInput = false; // identifier
bool isReleased = true;
bool if_first_input = false;

using AnimationFunction = std::function<void(Player&, float)>;

auto triggeredTime = Clock::now();
auto releasedTime = Clock::now();
auto comboTime = Clock::now();

#pragma region AnimationQueue

#pragma endregion

static f32 particleEmissionRate = 0.f;
Player::Player(const char* filename, AEVec2 scale, AEVec2 location, AEVec2 speed, bool isFacingRight)
{
	obj.pTex = AEGfxTextureLoad(filename);
	obj.speed = speed;

	AEVec2Set(&obj.pos, location.x, location.y);
	AEVec2Set(&velocity, 0.f, 0.f); //Begin with no velocity
	AEVec2Set(&obj.scale, scale.x, scale.y);
	AEVec2Set(&expectedLocation, 0.f, 0.f);

	isFacingRight = isFacingRight;
	lookAheadMutliplier = 50.f;
	onFloor = true; //Set as false first, will be set as true when ground detected
	isFalling = false;
	mass = 60.f;

	//Initializing collision box starting position
	collisionBox.minimum.x = obj.pos.x - obj.scale.x * 0.5f;
	collisionBox.minimum.y = obj.pos.y - obj.scale.y * 0.5f;
	collisionBox.maximum.x = obj.pos.x + obj.scale.x * 0.5f;
	collisionBox.maximum.y = obj.pos.y + obj.scale.y * 0.5f;

	AEVec2Set(&boxArms.maximum, 0.f, 0.f);
	AEVec2Set(&boxHeadFeet.maximum, 0.f, 0.f);
	AEVec2Set(&collisionNormal, 0.f, 0.f);

	equippedWeapon = createWeapon("Broad-Sword", location.x, location.y);
	AEVec2Set(&equippedWeapon.scale, 20.f, 20.f);
	attackTime = 1.f;
	isAttacking = false;
	comboTrig = 0;
	comboTime = 0.0f;
	comboState = 0;
	//std::cout << "Player has been equipped with a " << equippedWeapon.name << std::endl;

	burningEffect = false;
	isConversation = false;


	//Player Stats
	maxHealth = 100.f;
	currHealth = maxHealth;
	attack = 100.f;
	defence = 50.f;

	weaponExtraEffect = Status_Effect_System::NONE_WEAPON_EFFECT;
	armorExtraEffect = Status_Effect_System::NONE_ARMOR_EFFECT;
}

Player::~Player()
{
	//AEGfxTextureUnload(obj.pTex);
}

void Player::Update(bool isInventoryOpen)
{
	//if (AEInputCheckTriggered(AEVK_B))
	//{
	//	equippedWeapon = createWeapon("Short-Sword", expectedLocation.x, expectedLocation.y);
	//	std::cout << "Now equipped with a " << equippedWeapon.name << std::endl;

	//}
	//if (AEInputCheckTriggered(AEVK_N))
	//{
	//	equippedWeapon = createWeapon("Broad-Sword", expectedLocation.x, expectedLocation.y);
	//	std::cout << "Now equipped with a " << equippedWeapon.name << std::endl;
	//}
	//if (AEInputCheckTriggered(AEVK_M))
	//{
	//	equippedWeapon = createWeapon("GreatSword", expectedLocation.x, expectedLocation.y);
	//	std::cout << "Now equipped with a " << equippedWeapon.name << std::endl;
	//}


	if (isFalling)
	{
		std::cout << "FELL\n";
	}

	if (AEInputCheckCurr(AEVK_D) && !isInventoryOpen)
	{
		velocity.x += obj.speed.x * (f32)AEFrameRateControllerGetFrameTime();
		isFacingRight = true;

		if (particleEmissionRate > 0.1f)
		{
			particleEmissionRate = 0.f;
			ParticleEmit(1, obj.pos.x, obj.pos.y,
				obj.scale.x * 0.25f, obj.scale.y * 0.25f, 0.f, PARTICLE_TRAILING, this);
		}
		else
		{
			particleEmissionRate += (f32)AEFrameRateControllerGetFrameTime();
		}

	}
	else if (AEInputCheckCurr(AEVK_A) && !isInventoryOpen)
	{
		velocity.x -= obj.speed.x * (f32)AEFrameRateControllerGetFrameTime();
		isFacingRight = false;
		if (particleEmissionRate > 0.1f)
		{
			particleEmissionRate = 0.f;
			ParticleEmit(1, obj.pos.x, obj.pos.y,
				obj.scale.x * 0.25f, obj.scale.y * 0.25f, 0.f, PARTICLE_TRAILING, this);
		}
		else
		{
			particleEmissionRate += (f32)AEFrameRateControllerGetFrameTime();
		}

	}

	// Apply velocity constraints
	velocity.x = AEClamp(velocity.x, -5.f, 5.f);


	// Calculate the desired location
	AEVec2 desiredLocation{ velocity.x * lookAheadMutliplier , 0.f };
	AEVec2Add(&expectedLocation, &obj.pos, &desiredLocation);

	//For friction
	if (onFloor) //Means confirm on floor
	{
		velocity.x *= 0.85f; //Friction application
	}

	//For jumping
	if (AEInputCheckTriggered(VK_SPACE) && onFloor && !isInventoryOpen)
	{
		onFloor = false;
		velocity.y = 700.f;		
		ParticleEmit(10, obj.pos.x, obj.pos.y,
		obj.scale.x * 0.25f, obj.scale.y * 0.25f, 0.f, PARTICLE_JUMP, this);
	}

	ApplyGravity(&velocity, mass, &onFloor, &gravityForce, &isFalling); //Velocity passed in must be modifiable, mass can be adjusted if needed to
	//
	//std::cout << "Player on floor: " << onFloor << std::endl;
	//std::cout << "Player vel y: " << fabsf(velocity.y) << std::endl;
	//std::cout << "Player gravity force: " << gravityForce << std::endl;

	//Player position update

	prevPos = obj.pos;
	prevcollisionBox = collisionBox;
	obj.pos.y += velocity.y * (f32)AEFrameRateControllerGetFrameTime();
	obj.pos.x += velocity.x;

	//Resetting main AABB box...
	collisionBox.minimum.x = obj.pos.x - obj.scale.x * 0.5f;
	collisionBox.minimum.y = obj.pos.y - obj.scale.y * 0.5f;
	collisionBox.maximum.x = obj.pos.x + obj.scale.x * 0.5f;
	collisionBox.maximum.y = obj.pos.y + obj.scale.y * 0.5f;

	//Vertical
	f32 verticalOffset = obj.scale.y * 0.025f;

	// Vertical box
	boxHeadFeet = collisionBox;
	boxHeadFeet.minimum.y -= verticalOffset;
	boxHeadFeet.maximum.y += verticalOffset;

	f32 horizontalOffset = obj.scale.x * 0.025f;

	// Horizontal box
	boxArms = collisionBox;
	boxArms.minimum.x -= horizontalOffset;
	boxArms.maximum.x += horizontalOffset;

	//std::cout << "Collision Normal Y: " << collisionNormal.y << std::endl;
	//Update player weapon hit box

	/*Weapon hit box update only*/

	if (wp.rarity < Weapon_System::NO_GRADE)
	{
		if (AEInputCheckTriggered(AEVK_LBUTTON) && !isInventoryOpen)
		{
			triggeredTime = Clock::now();
			undealtTriggerInput = true;
			isReleased = false;
			if_first_input = true;

		}
		if (AEInputCheckReleased(AEVK_LBUTTON) && !isInventoryOpen)
		{
			isReleased = true;
		}
		//reset
		if (!undealtTriggerInput)
		{
			::elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - ::comboTime).count() / 1000.0; // Convert to seconds
			if (::elapsedTime > comboWindowDuration)
			{
				isAttacking = false;
				equippedWeapon.weaponHIT = false;
				comboTime = 0.0f; // Reset combo time
				comboState = 0;   // Reset combo state

			}

		}
		if (undealtTriggerInput)
		{
			auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - triggeredTime).count() /
				1000.0; // Convert to seconds
			if (elapsedTime >= PRESS_THRESHOLD && !isReleased)
			{
				if (comboState == 2) //held
				{
					f32 attackProgress = 1.0f - (attackTime / comboWindowDuration);
					UpdateWeaponHitBoxHeld(this, isFacingRight, &equippedWeapon, attackProgress);
					comboState = 0;
					isAttacking = true;
				}
				::comboTime = Clock::now();
				undealtTriggerInput = false;

			}
			if (elapsedTime < PRESS_THRESHOLD && isReleased) //Trigger (Here is flag for initialization)
			{
				f32 attackProgress = 1.0f - (attackTime / comboWindowDuration);
				UpdateWeaponHitBoxTrig(this, isFacingRight, &equippedWeapon, attackProgress);
				isAttacking = true;

				if (comboState < 2)
				{
					comboState++;
					comboTime += (float)elapsedTime;
				}

				else
				{
					comboState = 0;
					comboTime = 0.0f;
				}
				::comboTime = Clock::now();
				undealtTriggerInput = false;


			}
		}
	}

	Armor_Effect_Update(*this);

	if (currHealth <= 0.f)
	{
		OnPlayerDeath();
	}
}

void OnPlayerDeath() {
	//Return to lobby
	if (!transitionalImageOBJ.active)
	{
		transitionalImageOBJ.PlayMapTransition(TRANSITION_UP, GAME_LOBBY);
	}

}