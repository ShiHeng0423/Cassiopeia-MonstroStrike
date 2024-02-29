#include "GameState_SplashScreen.h"
#include "AEEngine.h"
#include "GameStateManager.h"

namespace {
	AEGfxVertexList* pSquareMesh;
	AEGfxTexture* digipenLogo;

	AEMtx33 transform;

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

	digipenLogo = AEGfxTextureLoad("Assets/Digipen_Logo/DigiPen_Singapore_WEB_RED.png");
}

void SplashScreen_Initialize()
{
	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, 800.f, 400.f);

	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, 0.f);

	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, 0.f, 0.f);

	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	timer = 3.0f;
	alpha = 0.f;
}

void SplashScreen_Update()
{
	timer -= (f32)AEFrameRateControllerGetFrameTime();
	if (timer > 0.f)
		alpha += 1.f / ((f32)AEFrameRateControllerGetFrameRate() * 2.f);
	else
		next = MainMenu;

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

	AEGfxTextureSet(digipenLogo, 0, 0);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(pSquareMesh, AE_GFX_MDM_TRIANGLES);
}

void SplashScreen_Free()
{

}

void SplashScreen_Unload()
{
	AEGfxMeshFree(pSquareMesh);
	AEGfxTextureUnload(digipenLogo);
}