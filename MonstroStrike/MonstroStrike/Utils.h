/*!************************************************************************
  \file					Utils.h
  \project name			Monstrostrike
  \primary author		Teng Shi heng (100%)
  \brief				This file contains declaration of structures and 
						functions for managing objects of the game such 
						as sprites, objects, and buttons, along with 
						functions for generating meshes and calculation of
						transformation matrices for general usage. 

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#pragma once
#include "AEEngine.h"
#include <string>

//Static Image
struct Sprite
{
	AEGfxTexture* pTex{ NULL };

	AEVec2 pos{ 0.f,0.f };
	AEVec2 scale{ 0.f,0.f };
	float rotate{ 0.f };

	AEMtx33 transform{};

	void UpdateTransformMatrix();
};

//Moving Image (E.g. Player, Enemy etc)
struct Object
{
	AEGfxTexture* pTex{ NULL };

	AEVec2 pos{ 0.f,0.f };
	AEVec2 scale{ 0.f,0.f };
	float rotate{ 0.f };

	AEVec2 speed{ 0.f, 0.f };

	AEMtx33 transform{};

	void UpdateTransformMatrix();
};

//Button UI
struct Button
{
	AEGfxTexture* pTex{ NULL };

	AEVec2 pos{ 0.f,0.f };
	AEVec2 scale{ 0.f,0.f };
	float rotate{ 0.f };

	AEVec2 speed{ 0.f, 0.f };

	AEMtx33 transform{};

	void (*Ptr)(void);
	void UpdateTransformMatrix();
};

//Button UI (with text)
struct ButtonUI
{
	AEGfxTexture* pTex{ NULL };

	AEVec2 pos{ 0.f,0.f };
	AEVec2 scale{ 0.f,0.f };
	float rotate{ 0.f };

	AEVec2 speed{ 0.f, 0.f };

	AEMtx33 transform{};

	std::string str{ "" };

	void (*Ptr)(void);
	void UpdateTransformMatrix();
};

AEGfxVertexList* GenerateSquareMesh(u32 meshColor);
AEGfxVertexList* GenerateLineMesh(u32 meshColor);
AEMtx33 ObjectTransformationMatrixSet(f32 transX, f32 transY, f32 rotation, f32 scaleX, f32 scaleY);
