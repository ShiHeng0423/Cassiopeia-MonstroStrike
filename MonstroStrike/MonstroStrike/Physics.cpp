#include "Physics.h"

AEVec2 NormalizeValue(AABB firstBox, AABB secondBox) //AABB only
{
	AEVec2 result = { 0.f, 0.f };

	f32 overlapX = fminf(firstBox.maximum.x, secondBox.maximum.x) - fmaxf(firstBox.minimum.x, secondBox.minimum.x);
	f32 overlapY = fminf(firstBox.maximum.y, secondBox.maximum.y) - fmaxf(firstBox.minimum.y, secondBox.minimum.y);

	// Determine the axis of least overlap
	if (overlapX < overlapY) {
		// Colliding along the X-axis
		result = { (firstBox.minimum.x + firstBox.maximum.x) < (secondBox.minimum.x + secondBox.maximum.x) ? -1.0f : 1.0f, 0.0f };
	}
	else {
		// Colliding along the Y-axis
		result = { 0.0f, (firstBox.minimum.y + firstBox.maximum.y) < (secondBox.minimum.y + secondBox.maximum.y) ? -1.0f : 1.0f };
	}

	return result;
}

f32 DotProduct(const AEVec2& first, const AEVec2& second)
{
	return first.x * second.x + first.y * second.y;
}