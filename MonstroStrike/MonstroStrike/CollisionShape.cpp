#include "CollisionShape.h"
#include <AEMath.h>
#include <iostream>
#include <algorithm>

bool AABBvsAABB(AABB firstBox, AABB secondBox)
{
	const f32 epsilon = 0.1f;
	//Return false if there are no intersection found separated along axis
	if (firstBox.maximum.x + epsilon < secondBox.minimum.x || firstBox.minimum.x > secondBox.maximum.x + epsilon)
	{
		return false;
	}
	if (firstBox.maximum.y + epsilon < secondBox.minimum.y || firstBox.minimum.y > secondBox.maximum.y + epsilon)
	{
		return false;
	}

	//Means at least one overlapping axis
	return true;
}

bool CirclevsCircle(Circle first, Circle second)
{
	float totalRadius = first.radius + second.radius;

	totalRadius *= totalRadius;
	
	return totalRadius < (pow(first.position.x + second.position.x, 2) + pow(first.position.y + second.position.y, 2));
}