#include "Mainmenu.h"
#include "Utils.h"
#include "TransformMatrix.h"

Button menuButtonBackground;
Button menuButton;
AEGfxVertexList* pWhiteSquareMesh;
void Menu_Load()
{
	menuButtonBackground.img.pTex = AEGfxTextureLoad("Assets/panel_brown.png");
	AEVec2Set(&menuButtonBackground.img.scale,250.f,80.f);

	menuButton.img.pTex = AEGfxTextureLoad("Assets/panelInset_beige.png");
	AEVec2Set(&menuButton.img.scale, 220.f, 60.f);

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
	AEGfxSetTransform(ObjectTransformationMatrixSet(-600.f, 0.f, 0.f, menuButtonBackground.img.scale.x, menuButtonBackground.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, 0.f, 0.f, menuButtonBackground.img.scale.x, menuButtonBackground.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, 0.f, 0.f, menuButtonBackground.img.scale.x, menuButtonBackground.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, 0.f, 0.f, menuButtonBackground.img.scale.x, menuButtonBackground.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);


	AEGfxTextureSet(menuButton.img.pTex, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(0, 0, 0.f, menuButton.img.scale.x, menuButton.img.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
}

void Menu_Free()
{

}

void Menu_Unload()
{

}
