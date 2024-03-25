#include "GameState_SplashScreen.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "main.h"
#include "Utils.h"
#include <string>
namespace {
	AEGfxVertexList* pSquareMesh;
	AEGfxTexture* digipenLogoSprite;
	AEGfxTexture* copyrightSprite;

	Sprite digipenLogo;
	Sprite copyright;

	f32 timer;
	f32 alpha;
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
}

void SplashScreen_Initialize()
{
	digipenLogo.pTex = digipenLogoSprite;
	AEVec2Set(&digipenLogo.scale, 800.f, 400.f);
	digipenLogo.UpdateTransformMatrix();

	copyright.pTex = copyrightSprite;
	AEVec2Set(&copyright.scale, 600.f, 50.f);
	copyright.UpdateTransformMatrix();

	timer = 3.0f;
	alpha = 0.f;
}

void SplashScreen_Update()
{
	timer -= (f32)AEFrameRateControllerGetFrameTime();
	if (timer > 0.f)
		alpha += 1.f / ((f32)AEFrameRateControllerGetFrameRate() * 2.f);
	else
		next = MAINMENU;

	alpha = AEClamp(alpha, 0.f, 1.0f);
}

void SplashScreen_Draw()
{
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(alpha);

	AEGfxTextureSet(digipenLogo.pTex, 0, 0);
	AEGfxSetTransform(digipenLogo.transform.m);
	AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);

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
	AEGfxTextureUnload(copyrightSprite);
}