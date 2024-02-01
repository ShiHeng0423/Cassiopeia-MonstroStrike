#include "TransformMatrix.h"

AEMtx33 ObjectTransformationMatrixSet(f32 transX, f32 transY, f32 rotation, f32 scaleX, f32 scaleY)
{
	// Create a scale matrix that scales by 500 x and y
	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, scaleX, scaleY);

	// Create a rotation matrix that rotates by 90 degrees
	// Note that PI in radians is 180 degrees.
	// Since 90 degrees is 180/2, 90 degrees in radians is PI/2
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, rotation);

	// Create a translation matrix that translates by
	// 200 in the x-axis and 100 in the y-axis
	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, transX, transY);

	// Concatenate the matrices into the 'transform' variable.
	// We concatenate in the order of translation * rotation * scale
	// i.e. this means we scale, then rotate, then translate.
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	return transform;
}