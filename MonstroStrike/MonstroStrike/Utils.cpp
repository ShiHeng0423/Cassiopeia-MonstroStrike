/*!************************************************************************
  \file					Utils.cpp
  \project name			Monstrostrike
  \primary author		Teng Shi heng (100%)
  \brief				File containing the definitions of functions declared
						Utils.h files.

All content � 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#include "AEEngine.h"
#include "Utils.h"

void Sprite::UpdateTransformMatrix()
{
	AEMtx33 scaleMatrix = { 0 };
	AEMtx33Scale(&scaleMatrix, scale.x, scale.y);

	AEMtx33 rotateMatrix = { 0 };
	AEMtx33Rot(&rotateMatrix, rotate);

	AEMtx33 translateMatrix = { 0 };
	AEMtx33Trans(&translateMatrix, pos.x, pos.y);

	AEMtx33Concat(&transform, &rotateMatrix, &scaleMatrix);
	AEMtx33Concat(&transform, &translateMatrix, &transform);
}

void Object::UpdateTransformMatrix()
{
	AEMtx33 scaleMatrix = { 0 };
	AEMtx33Scale(&scaleMatrix, scale.x, scale.y);

	AEMtx33 rotateMatrix = { 0 };
	AEMtx33Rot(&rotateMatrix, rotate);

	AEMtx33 translateMatrix = { 0 };
	AEMtx33Trans(&translateMatrix, pos.x, pos.y);

	AEMtx33Concat(&transform, &rotateMatrix, &scaleMatrix);
	AEMtx33Concat(&transform, &translateMatrix, &transform);
}

void Button::UpdateTransformMatrix()
{
	AEMtx33 scaleMatrix = { 0 };
	AEMtx33Scale(&scaleMatrix, scale.x, scale.y);

	AEMtx33 rotateMatrix = { 0 };
	AEMtx33Rot(&rotateMatrix, rotate);

	AEMtx33 translateMatrix = { 0 };
	AEMtx33Trans(&translateMatrix, pos.x, pos.y);

	AEMtx33Concat(&transform, &rotateMatrix, &scaleMatrix);
	AEMtx33Concat(&transform, &translateMatrix, &transform);
}

void ButtonUI::UpdateTransformMatrix()
{
	AEMtx33 scaleMatrix = { 0 };
	AEMtx33Scale(&scaleMatrix, scale.x, scale.y);

	AEMtx33 rotateMatrix = { 0 };
	AEMtx33Rot(&rotateMatrix, rotate);

	AEMtx33 translateMatrix = { 0 };
	AEMtx33Trans(&translateMatrix, pos.x, pos.y);

	AEMtx33Concat(&transform, &rotateMatrix, &scaleMatrix);
	AEMtx33Concat(&transform, &translateMatrix, &transform);
}

AEGfxVertexList* GenerateSquareMesh(u32 MeshColor)
{
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, MeshColor, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, MeshColor, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, MeshColor, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, MeshColor, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, MeshColor, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, MeshColor, 0.0f, 0.0f);  // top-left: blue

	return AEGfxMeshEnd();
}

AEGfxVertexList* GenerateLineMesh(u32 MeshColor)
{
	AEGfxMeshStart();

	AEGfxVertexAdd(-0.5f, 0.f, MeshColor, 1.0f, 1.0f);
	AEGfxVertexAdd(0.5f, 0.f, MeshColor, 1.0f, 1.0f);

	return AEGfxMeshEnd();
}

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