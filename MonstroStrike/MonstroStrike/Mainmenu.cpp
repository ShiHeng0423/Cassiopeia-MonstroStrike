#include "Mainmenu.h"
#include "Utils.h"
#include "TransformMatrix.h"
#include "GameStateManager.h"

namespace {
	Button menuButtonBackground;
	Button menuButton;
	AEGfxVertexList* pWhiteSquareMesh;
	s8 pFont;
}

void GoLevel1();

void Menu_Load()
{
	pFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 72);

	menuButtonBackground.img.pTex = AEGfxTextureLoad("Assets/panel_brown.png");
	AEVec2Set(&menuButtonBackground.img.scale, 250.f, 80.f);

	menuButton.img.pTex = AEGfxTextureLoad("Assets/panelInset_beige.png");
	AEVec2Set(&menuButton.img.scale, 220.f, 60.f);
	menuButton.Ptr = GoLevel1;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh
	pWhiteSquareMesh = AEGfxMeshEnd();


}

void Menu_Initialize()
{

}

void Menu_Update()
{
	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		s32 x, y;
		AEInputGetCursorPosition(&x, &y);
		AEVec2 mousePos;
		mousePos.x = x - AEGfxGetWindowWidth() * 0.5f;
		mousePos.y = AEGfxGetWindowHeight() * 0.5f - y;
		if (AETestPointToRect(&mousePos, &menuButton.pos, menuButton.img.scale.x, menuButton.img.scale.y))
			menuButton.Ptr();
	}
}

void Menu_Draw()
{
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(menuButtonBackground.img.pTex, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, 0.f, 0.f, menuButtonBackground.img.scale.x, menuButtonBackground.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, -100.f, 0.f, menuButtonBackground.img.scale.x, menuButtonBackground.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, -200.f, 0.f, menuButtonBackground.img.scale.x, menuButtonBackground.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, -300.f, 0.f, menuButtonBackground.img.scale.x, menuButtonBackground.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);


	AEGfxTextureSet(menuButton.img.pTex, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(0, 0, 0.f, menuButton.img.scale.x, menuButton.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, -100.f, 0.f, menuButton.img.scale.x, menuButton.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, -200.f, 0.f, menuButton.img.scale.x, menuButton.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, -300.f, 0.f, menuButton.img.scale.x, menuButton.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	const char* pText = "Start";
	f32 width, height;
	AEGfxGetPrintSize(pFont, pText, 0.5f, &width, &height);
	AEGfxPrint(pFont, pText, -width / 2, -height / 2, 0.5f, 1, 1, 1, 1);

	const char* pText1 = "Option";
	AEGfxGetPrintSize(pFont, pText1, 0.5f, &width, &height);
	AEGfxPrint(pFont, pText1, -width / 2, -height / 2 - 0.22f, 0.5f, 1, 1, 1, 1);

	const char* pText2 = "Credit";
	AEGfxGetPrintSize(pFont, pText2, 0.5f, &width, &height);
	AEGfxPrint(pFont, pText2, -width / 2, -height / 2 - 0.44f, 0.5f, 1, 1, 1, 1);

	const char* pText3 = "Exit";
	AEGfxGetPrintSize(pFont, pText3, 0.5f, &width, &height);
	AEGfxPrint(pFont, pText3, -width / 2, -height / 2 - 0.66f, 0.5f, 1, 1, 1, 1);
}

void Menu_Free()
{

}

void Menu_Unload()
{

}

void GoLevel1()
{
	next = GameStates::Area1;
}

//void Leave()
//{
//	if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
//	{
//		current = GameStates::Quit;
//	}
//}