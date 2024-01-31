#include "Physics.h"

AEVec2 AABBNormalize(AABB firstBox, AABB secondBox) //AABB only
{
	AEVec2 result = { 0.f, 0.f };

	result.x = roundf((firstBox.minimum.x - secondBox.minimum.x) / (secondBox.maximum.x - secondBox.minimum.x));
	result.y = roundf((firstBox.minimum.y - secondBox.minimum.y) / (secondBox.maximum.y - secondBox.minimum.y));

	return result;
}

f32 DotProduct(const AEVec2& first, const AEVec2& second)
{
	return first.x * second.x + first.y * second.y;
}

void ApplyGravity(AEVec2 *velocity, f32 mass)
{
	std::cout << "Velocity: " << velocity->y << std::endl;
	std::cout << "Mass: " << mass << std::endl;

	AEVec2 gravityForce;
	AEVec2 gravity = { 0.f, -9.81f }; //The acceleration aka gravity
	AEVec2Scale(&gravityForce, &gravity, mass); //Force = Mass x Acceleration
	AEVec2Scale(velocity, velocity, 0.98f); //Dampen, maybe add terminal velocity if needed to
	velocity->y += gravityForce.y * AEFrameRateControllerGetFrameTime();
}