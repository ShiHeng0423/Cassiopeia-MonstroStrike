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