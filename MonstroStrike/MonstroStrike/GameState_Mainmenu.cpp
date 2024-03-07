#include "GameState_Mainmenu.h"
#include "Utils.h"
#include "TransformMatrix.h"
#include "GameStateManager.h"

namespace {

	enum CurrentScene {
		MainScene,
		CreditScene,
		ControlScene,
		OptionScene,
		QuitScene
	};

	enum Interactable {
		Start,
		Load,
		Credit,
		Controls,
		Options,
		Quit
	};

	//Main menu buttons
	Button interactableButtonMainMenu[6];

	//Option button
	Sprite_V2 optionbackground;
	Sprite_V2 optionSoundBar[2];
	Sprite_V2 optionBackgroundBar[2];
	Button interactableButtonOption[4];


	Button backButton;
	Button confirmationButtion[2];

	AEGfxTexture* buttonTexture;
	AEGfxTexture* backgroundTexture;

	AEGfxVertexList* pWhiteSquareMesh;

	Sprite background;

	s8 pFont;

	//menu buttons
	//Start Game Option - "Start Game" or "New Game" (if multiple files and saved data)
	//Load Game Option(for games with save files) - "Load Saved Game" or "Load Save File"
	//Quit Game Option - "Quit Game" (with confirmation)
	//Credits - "Credits"
	//How To Play - "How to Play" or "Controls"
	//Options Menu - "Options"

	s8 currScene;
}

void GoNewGameLevel1();
void GoLoadSaveLevel();
void GoCreditScene();
void GoControlScene();
void GoOptionScene();
void GoConfirmQuitScene();
void GoQuitGame();

void BackMainMenu();

//void IncreaseSfxVolume();
//void IncreaseBgmVolume();
//void DecreaseSfxVolume();
//void DecreaseBgmVolume();

void Mainmenu_Load()
{
	pFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 72);
	buttonTexture = AEGfxTextureLoad("Assets/panel_brown.png");
	backgroundTexture = AEGfxTextureLoad("Assets/1.jpg");

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
	for (size_t i = 0; i < sizeof(interactableButtonMainMenu) / sizeof(interactableButtonMainMenu[0]); i++)
	{
		interactableButtonMainMenu[i].pTex = buttonTexture;
		AEVec2Set(&interactableButtonMainMenu[i].scale, 250.f, 80.f);
		AEVec2Set(&interactableButtonMainMenu[i].pos, 0.f, 100.f - 100.f * i);

		switch (i)
		{
		case Interactable::Start:
			interactableButtonMainMenu[i].Ptr = GoNewGameLevel1;
			break;
		case Interactable::Load:
			interactableButtonMainMenu[i].Ptr = GoLoadSaveLevel;
			break;
		case Interactable::Credit:
			interactableButtonMainMenu[i].Ptr = GoCreditScene;
			break;
		case Interactable::Controls:
			interactableButtonMainMenu[i].Ptr = GoControlScene;
			break;
		case Interactable::Options:
			interactableButtonMainMenu[i].Ptr = GoOptionScene;
			break;
		case Interactable::Quit:
			interactableButtonMainMenu[i].Ptr = GoConfirmQuitScene;
			break;
		default:
			break;
		}
	}

	AEVec2Set(&interactableButtonOption[0].pos, 0, 0);
	AEVec2Set(&interactableButtonOption[0].scale, 50, 50);
	//interactableButtonOption[0].Ptr = DecreaseBgmVolume;
	//interactableButtonOption[0].pTex = DecreaseBgmVolume;

	AEVec2Set(&interactableButtonOption[1].pos, 200, 0);
	AEVec2Set(&interactableButtonOption[1].scale, 50, 50);
	//interactableButtonOption[1].Ptr = IncreaseBgmVolume;
	//interactableButtonOption[1].pTex = DecreaseBgmVolume;

	AEVec2Set(&interactableButtonOption[2].pos, 0, -100);
	AEVec2Set(&interactableButtonOption[2].scale, 50, 50);
	//interactableButtonOption[0].Ptr = DecreaseSfxVolume;

	AEVec2Set(&interactableButtonOption[3].pos, 200, -100);
	AEVec2Set(&interactableButtonOption[3].scale, 50, 50);
	//interactableButtonOption[0].Ptr = IncreaseSfxVolume;

	AEVec2Set(&optionSoundBar[0].pos, 100, 0); // bar 1
	AEVec2Set(&optionSoundBar[0].scale, 100, 50);

	AEVec2Set(&optionSoundBar[1].pos, 100, -100); // bar 2
	AEVec2Set(&optionSoundBar[1].scale, 100, 50);

	AEVec2Set(&optionBackgroundBar[0].pos, 100, 0); // bar 1
	AEVec2Set(&optionBackgroundBar[0].scale, 100, 50);

	AEVec2Set(&optionBackgroundBar[1].pos, 100, -100); // bar 2
	AEVec2Set(&optionBackgroundBar[1].scale, 100, 50);

	optionbackground;

	backButton.pTex = buttonTexture;
	AEVec2Set(&backButton.scale, 250.f, 80.f);
	AEVec2Set(&backButton.pos, -675.f, -410.f);
	backButton.Ptr = BackMainMenu;

	confirmationButtion[0].pTex = buttonTexture;
	AEVec2Set(&confirmationButtion[0].scale, 250.f, 80.f);
	AEVec2Set(&confirmationButtion[0].pos, 250.f, 0.f);
	confirmationButtion[0].Ptr = BackMainMenu;

	confirmationButtion[1].pTex = buttonTexture;
	AEVec2Set(&confirmationButtion[1].scale, 250.f, 80.f);
	AEVec2Set(&confirmationButtion[1].pos, -250.f, 0.f);
	confirmationButtion[1].Ptr = GoQuitGame;

	background.pTex = backgroundTexture;
	background.scale.x = 1600;
	background.scale.y = 900;

	currScene = CurrentScene::MainScene;
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

		switch (currScene)
		{
		case CurrentScene::MainScene:
			for (size_t i = 0; i < sizeof(interactableButtonMainMenu) / sizeof(interactableButtonMainMenu[0]); i++)
			{
				if (AETestPointToRect(&mousePos, &interactableButtonMainMenu[i].pos, interactableButtonMainMenu[i].scale.x, interactableButtonMainMenu[i].scale.y))
					interactableButtonMainMenu[i].Ptr();
			}
			break;
		case CurrentScene::CreditScene:
		case CurrentScene::ControlScene:
		case CurrentScene::OptionScene:
			if (AETestPointToRect(&mousePos, &backButton.pos, backButton.scale.x, backButton.scale.y))
				backButton.Ptr();
			break;
		case CurrentScene::QuitScene:
			if (AETestPointToRect(&mousePos, &confirmationButtion[0].pos, confirmationButtion[0].scale.x, confirmationButtion[0].scale.y))
				confirmationButtion[0].Ptr();
			if (AETestPointToRect(&mousePos, &confirmationButtion[1].pos, confirmationButtion[1].scale.x, confirmationButtion[1].scale.y))
				confirmationButtion[1].Ptr();
			break;
		default:
			break;
		}
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

	AEGfxTextureSet(background.pTex, 0, 0);
	AEGfxSetTransform(ObjectTransformationMatrixSet(0.f, 0.f, 0.f, background.scale.x, background.scale.y).m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

	switch (currScene)
	{
	case CurrentScene::MainScene:
	{
		for (size_t i = 0; i < sizeof(interactableButtonMainMenu) / sizeof(interactableButtonMainMenu[0]); i++)
		{
			AEGfxTextureSet(interactableButtonMainMenu[i].pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(interactableButtonMainMenu[i].pos.x, interactableButtonMainMenu[i].pos.y, 0.f, interactableButtonMainMenu[i].scale.x, interactableButtonMainMenu[i].scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}

		f32 width, height;

		const char* pText = "Start";
		AEGfxGetPrintSize(pFont, pText, 0.5f, &width, &height);
		AEGfxPrint(pFont, pText, -width / 2, -height / 2 + 0.22f, 0.5f, 1, 1, 1, 1);

		const char* pText1 = "Load";
		AEGfxGetPrintSize(pFont, pText1, 0.5f, &width, &height);
		AEGfxPrint(pFont, pText1, -width / 2, -height / 2, 0.5f, 1, 1, 1, 1);

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
		break;
	}
	case CurrentScene::CreditScene:
	case CurrentScene::ControlScene:
	case CurrentScene::OptionScene:
	{
		AEGfxTextureSet(backButton.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(backButton.pos.x, backButton.pos.y, 0.f, backButton.scale.x, backButton.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		f32 width, height;

		const char* pText = "Back";
		AEGfxGetPrintSize(pFont, pText, 0.5f, &width, &height);
		AEGfxPrint(pFont, pText, -width / 2 - 0.85f, -height / 2 - 0.9f, 0.5f, 1, 1, 1, 1);
		break;
	}
	case CurrentScene::QuitScene:
	{
		AEGfxTextureSet(confirmationButtion[0].pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(confirmationButtion[0].pos.x, confirmationButtion[0].pos.y, 0.f, confirmationButtion[0].scale.x, confirmationButtion[0].scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(confirmationButtion[1].pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(confirmationButtion[1].pos.x, confirmationButtion[1].pos.y, 0.f, confirmationButtion[1].scale.x, confirmationButtion[1].scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		f32 width, height;

		const char* pText = "Yes";
		AEGfxGetPrintSize(pFont, pText, 0.5f, &width, &height);
		AEGfxPrint(pFont, pText, -width / 2 - 0.31f, -height / 2, 0.5f, 1, 1, 1, 1);

		const char* pText1 = "No";
		AEGfxGetPrintSize(pFont, pText1, 0.5f, &width, &height);
		AEGfxPrint(pFont, pText1, -width / 2 + 0.31f, -height / 2, 0.5f, 1, 1, 1, 1);
		break;
	}
	default:
		break;
	}


}

void Mainmenu_Free()
{

}

void Mainmenu_Unload()
{
	AEGfxMeshFree(pWhiteSquareMesh);
	AEGfxTextureUnload(buttonTexture);
	AEGfxTextureUnload(backgroundTexture);
	AEGfxDestroyFont(pFont);
}

void BackMainMenu()
{
	currScene = CurrentScene::MainScene;
}

void GoNewGameLevel1()
{
	next = GameStates::Area1;
}

void GoLoadSaveLevel()
{

}

void GoCreditScene()
{
	currScene = CurrentScene::CreditScene;
}

void GoControlScene()
{
	currScene = CurrentScene::ControlScene;
}

void GoOptionScene()
{
	currScene = CurrentScene::OptionScene;
}

void GoConfirmQuitScene()
{
	currScene = CurrentScene::QuitScene;
}

void GoQuitGame()
{
	next = GameStates::Quit;
}