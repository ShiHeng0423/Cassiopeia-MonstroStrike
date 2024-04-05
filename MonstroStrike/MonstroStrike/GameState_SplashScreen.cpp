/*!************************************************************************
  \file					GameState_SplashScreen.cpp
  \project name			Monstrostrike
  \primary author		Teng Shi heng (100%)
  \brief				File containing the definitions of functions declared
						GameState_SplashScreen.h files.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#include "GameState_SplashScreen.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "main.h"
#include "Utils.h"
#include <string>
namespace {
	AEGfxVertexList* pSquareMesh;
	AEGfxTexture* digipenLogoSprite;
	AEGfxTexture* teamNameSprite;
	AEGfxTexture* copyrightSprite;

	Sprite digipenLogo;
	Sprite teamName;
	Sprite copyright;

	f32 timer;
	f32 alpha;
	
	bool logoChange;
}

void SplashScreen_Load()
{
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // top-left: blue

	pSquareMesh = AEGfxMeshEnd();

	digipenLogoSprite = AEGfxTextureLoad("Assets/Digipen_Logo/DigiPen_Singapore_WEB_WHITE.png");
	copyrightSprite = AEGfxTextureLoad("Assets/Digipen_Logo/Copyright.png");
	teamNameSprite = AEGfxTextureLoad("Assets/Digipen_Logo/Team_Name_WEB_WHITE.png");
}

void SplashScreen_Initialize()
{
	
	digipenLogo.pTex = digipenLogoSprite;
	AEVec2Set(&digipenLogo.scale, 800.f, 400.f);
	digipenLogo.UpdateTransformMatrix();

	teamName.pTex = teamNameSprite;
	AEVec2Set(&teamName.scale, 800.f, 400.f);
	teamName.UpdateTransformMatrix();

	copyright.pTex = copyrightSprite;
	AEVec2Set(&copyright.pos, 0.f, AEGfxGetWinMinY() * 0.9f);
	AEVec2Set(&copyright.scale, 600.f, 50.f);
	copyright.UpdateTransformMatrix();

	timer = 6.0f;
	alpha = 0.f;
	logoChange = false;
}

void SplashScreen_Update()
{
	timer -= (f32)AEFrameRateControllerGetFrameTime();
	if (timer > 3.f) {
		alpha = (6.0f - timer) / 3.0f;
		alpha = AEClamp(alpha, 0.f, 1.0f);
		digipenLogo.pTex = digipenLogoSprite;
		teamName.pTex = NULL; 
	}
	else if (timer > 0.0f && timer < 3.f) {
		if (!logoChange) {
			alpha = 0.f; 
			logoChange = true;
		}
		alpha += (1.f / ((f32)AEFrameRateControllerGetFrameRate() * 2.f));
		digipenLogo.pTex = NULL;
		teamName.pTex = teamNameSprite;
	}
	else {
		alpha -= (1.f / ((f32)AEFrameRateControllerGetFrameRate() * 2.f));
		alpha = AEClamp(alpha, 0.f, 1.0f);
		next = MAINMENU;
	}

}

void SplashScreen_Draw()
{
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(alpha);

	if (!logoChange) {
		AEGfxTextureSet(digipenLogo.pTex, 0, 0);
		AEGfxSetTransform(digipenLogo.transform.m);
		AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);
	}
	else {
		AEGfxTextureSet(teamName.pTex, 0, 0);
		AEGfxSetTransform(teamName.transform.m);
		AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);
	}

	AEGfxTextureSet(copyright.pTex, 0, 0);
	AEGfxSetTransform(copyright.transform.m);
	AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);
}

void SplashScreen_Free()
{

}

void SplashScreen_Unload()
{
	AEGfxMeshFree(pSquareMesh);
	AEGfxTextureUnload(digipenLogoSprite);
	AEGfxTextureUnload(teamNameSprite);
	AEGfxTextureUnload(copyrightSprite);
}