#include "GameState_Mainmenu.h"
#include "Utils.h"
#include "TransformMatrix.h"
#include "GameStateManager.h"

namespace {

	enum Interactable {
		Start,
		Load,
		Credit,
		Controls,
		Options,
		Quit
	};

	Button interactableButton[6];

	AEGfxTexture* buttonTexture;
	AEGfxVertexList* pWhiteSquareMesh;
	s8 pFont;

	//menu buttons
	//Start Game Option - "Start Game" or "New Game" (if multiple files and saved data)
	//Load Game Option(for games with save files) - "Load Saved Game" or "Load Save File"
	//Quit Game Option - "Quit Game" (with confirmation)
	//Credits - "Credits"
	//How To Play - "How to Play" or "Controls"
	//Options Menu - "Options"

	//Return to Main Menu Option - "Return to Main Menu" or "Exit to Main Menu"
	// //Restart Game Option - "Restart Game", "Return to Overworld", "New Game", "Exit Level"
	//How To Play - "How to Play" or "Controls"
	//Resume Game Option - "Resume Game"
}

void GoLevel1();

void Mainmenu_Load()
{
	pFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 72);
	buttonTexture = AEGfxTextureLoad("Assets/panel_brown.png");

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

void Mainmenu_Initialize()
{
	for (size_t i = 0; i < sizeof(interactableButton) / sizeof(interactableButton[0]); i++)
	{
		interactableButton[i].pTex = buttonTexture;
		AEVec2Set(&interactableButton[i].scale, 250.f, 80.f);

		switch (i)
		{
		case Interactable::Start:
			interactableButton[i].Ptr = GoLevel1;
			break;
		case Interactable::Load:
			break;
		case Interactable::Credit:
			break;
		case Interactable::Controls:
			break;
		case Interactable::Options:
			break;
		case Interactable::Quit:
			break;
		default:
			break;
		}
	}
}

void Mainmenu_Update()
{
	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		s32 x, y;
		AEInputGetCursorPosition(&x, &y);
		AEVec2 mousePos{ 0,0 };
		mousePos.x = x - AEGfxGetWindowWidth() * 0.5f;
		mousePos.y = AEGfxGetWindowHeight() * 0.5f - y;
		//if (AETestPointToRect(&mousePos, &menuButton.pos, menuButton.scale.x, menuButton.scale.y))
		//	menuButton.Ptr();
	}
}

void Mainmenu_Draw()
{
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	for (size_t i = 0; i < sizeof(interactableButton) / sizeof(interactableButton[0]); i++)
	{
		AEGfxTextureSet(interactableButton[i].pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, 100.f - 100.f * i, 0.f, interactableButton[i].scale.x, interactableButton[i].scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	}

	f32 width, height;

	const char* pText = "Start";
	AEGfxGetPrintSize(pFont, pText, 0.5f, &width, &height);
	AEGfxPrint(pFont, pText, -width / 2, -height / 2 + 0.22, 0.5f, 1, 1, 1, 1);

	const char* pText1 = "Load";
	AEGfxGetPrintSize(pFont, pText1, 0.5f, &width, &height);
	AEGfxPrint(pFont, pText1, -width / 2, -height / 2 , 0.5f, 1, 1, 1, 1);

	const char* pText2 = "Credit";
	AEGfxGetPrintSize(pFont, pText2, 0.5f, &width, &height);
	AEGfxPrint(pFont, pText2, -width / 2, -height / 2 - 0.22f, 0.5f, 1, 1, 1, 1);

	const char* pText3 = "Controls";
	AEGfxGetPrintSize(pFont, pText3, 0.5f, &width, &height);
	AEGfxPrint(pFont, pText3, -width / 2, -height / 2 - 0.44f, 0.5f, 1, 1, 1, 1);

	const char* pText4 = "Options";
	AEGfxGetPrintSize(pFont, pText4, 0.5f, &width, &height);
	AEGfxPrint(pFont, pText4, -width / 2, -height / 2 - 0.66f, 0.5f, 1, 1, 1, 1);

	const char* pText5 = "Quit";
	AEGfxGetPrintSize(pFont, pText5, 0.5f, &width, &height);
	AEGfxPrint(pFont, pText5, -width / 2, -height / 2 - 0.88f, 0.5f, 1, 1, 1, 1);
}

void Mainmenu_Free()
{

}

void Mainmenu_Unload()
{
	AEGfxMeshFree(pWhiteSquareMesh);
	AEGfxTextureUnload(buttonTexture);
	AEGfxDestroyFont(pFont);
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