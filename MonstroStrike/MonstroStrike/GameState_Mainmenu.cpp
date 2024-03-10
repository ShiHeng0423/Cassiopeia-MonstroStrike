#include "GameState_Mainmenu.h"
#include "Utils.h"
#include "TransformMatrix.h"
#include "GameStateManager.h"
#include "main.h"

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
	AEGfxTexture* audioUp;
	AEGfxTexture* audioDown;


	Button backButton;
	Button confirmationButtion[2];

	AEGfxTexture* buttonTexture;
	AEGfxTexture* backgroundTexture;
	AEGfxTexture* boxBackground;

	AEGfxVertexList* pWhiteSquareMesh;
	AEGfxVertexList* pBlackSquareMesh;

	Sprite background;

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

void IncreaseSfxVolume()
{
	audioManager->IncreaseSFXVolume();
	f32 maths = 0 - 250 * (1.f - audioManager->GetSFXVolume() * 2.f);
	AEVec2Set(&optionSoundBar[1].pos, 0 - 250 * (0.5f - audioManager->GetSFXVolume()), -100); // bar 2
	AEVec2Set(&optionSoundBar[1].scale, 250 * audioManager->GetSFXVolume() * 2.f, 50);
}

void IncreaseBgmVolume()
{
	audioManager->IncreaseBGMVolume();
	AEVec2Set(&optionSoundBar[0].pos, 0 - 250 * (0.5f - audioManager->GetBGMVolume()), 0); // bar 1
	AEVec2Set(&optionSoundBar[0].scale, 250 * audioManager->GetBGMVolume() * 2.f, 50);
}

void DecreaseSfxVolume()
{
	audioManager->DecreaseSFXVolume();
	AEVec2Set(&optionSoundBar[1].pos, 0 - 250 * (0.5f - audioManager->GetSFXVolume()), -100); // bar 2
	AEVec2Set(&optionSoundBar[1].scale, 250 * audioManager->GetSFXVolume() * 2.f, 50);
}

void DecreaseBgmVolume()
{
	audioManager->DecreaseBGMVolume();
	AEVec2Set(&optionSoundBar[0].pos, 0 - 250 * (0.5f - audioManager->GetBGMVolume()), 0); // bar 1
	AEVec2Set(&optionSoundBar[0].scale, 250 * audioManager->GetBGMVolume() * 2.f, 50);
}

void Mainmenu_Load()
{
	buttonTexture = AEGfxTextureLoad("Assets/panel_brown.png");
	backgroundTexture = AEGfxTextureLoad("Assets/1.jpg");
	boxBackground = AEGfxTextureLoad("Assets/UI_Sprite/Transparent center/panel-transparent-center-015.png");
	audioUp = AEGfxTextureLoad("Assets/UI_Sprite/arrowBrown_right.png");
	audioDown = AEGfxTextureLoad("Assets/UI_Sprite/arrowBrown_left.png");

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

	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF000000, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFF000000, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh
	pBlackSquareMesh = AEGfxMeshEnd();

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

	AEVec2Set(&interactableButtonOption[0].pos, -200, 0);
	AEVec2Set(&interactableButtonOption[0].scale, 50, 50);
	interactableButtonOption[0].Ptr = DecreaseBgmVolume;
	interactableButtonOption[0].pTex = audioDown;

	AEVec2Set(&interactableButtonOption[1].pos, 200, 0);
	AEVec2Set(&interactableButtonOption[1].scale, 50, 50);
	interactableButtonOption[1].Ptr = IncreaseBgmVolume;
	interactableButtonOption[1].pTex = audioUp;

	AEVec2Set(&interactableButtonOption[2].pos, -200, -100);
	AEVec2Set(&interactableButtonOption[2].scale, 50, 50);
	interactableButtonOption[2].Ptr = DecreaseSfxVolume;
	interactableButtonOption[2].pTex = audioDown;

	AEVec2Set(&interactableButtonOption[3].pos, 200, -100);
	AEVec2Set(&interactableButtonOption[3].scale, 50, 50);
	interactableButtonOption[3].Ptr = IncreaseSfxVolume;
	interactableButtonOption[3].pTex = audioUp;

	AEVec2Set(&optionSoundBar[0].pos, 0, 0); // bar 1
	AEVec2Set(&optionSoundBar[0].scale, 250, 50);

	AEVec2Set(&optionSoundBar[1].pos, 0, -100); // bar 2
	AEVec2Set(&optionSoundBar[1].scale, 250, 50);

	AEVec2Set(&optionBackgroundBar[0].pos, 0, 0); // bar 1
	AEVec2Set(&optionBackgroundBar[0].scale, 250, 50);

	AEVec2Set(&optionBackgroundBar[1].pos, 0, -100); // bar 2
	AEVec2Set(&optionBackgroundBar[1].scale, 250, 50);

	optionbackground.pTex = boxBackground;
	optionbackground.scale.x = 1000.f;
	optionbackground.scale.y = 500.f;

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

	audioManager->PlayAudio(true, Audio_List::MainMenu_Song);
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
			if (AETestPointToRect(&mousePos, &backButton.pos, backButton.scale.x, backButton.scale.y))
				backButton.Ptr();
			break;
		case CurrentScene::OptionScene:
			for (size_t i = 0; i < sizeof(interactableButtonOption) / sizeof(interactableButtonOption[0]); i++)
			{
				if (AETestPointToRect(&mousePos, &interactableButtonOption[i].pos, interactableButtonOption[i].scale.x, interactableButtonOption[i].scale.y))
					interactableButtonOption[i].Ptr();
			}
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
		AEGfxGetPrintSize(fontID, pText, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText, -width / 2, -height / 2 + 0.22f, 0.5f, 1, 1, 1, 1);

		const char* pText1 = "Load";
		AEGfxGetPrintSize(fontID, pText1, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText1, -width / 2, -height / 2, 0.5f, 1, 1, 1, 1);

		const char* pText2 = "Credit";
		AEGfxGetPrintSize(fontID, pText2, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText2, -width / 2, -height / 2 - 0.22f, 0.5f, 1, 1, 1, 1);

		const char* pText3 = "Controls";
		AEGfxGetPrintSize(fontID, pText3, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText3, -width / 2, -height / 2 - 0.44f, 0.5f, 1, 1, 1, 1);

		const char* pText4 = "Options";
		AEGfxGetPrintSize(fontID, pText4, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText4, -width / 2, -height / 2 - 0.66f, 0.5f, 1, 1, 1, 1);

		const char* pText5 = "Quit";
		AEGfxGetPrintSize(fontID, pText5, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText5, -width / 2, -height / 2 - 0.88f, 0.5f, 1, 1, 1, 1);
		break;
	}
	case CurrentScene::CreditScene:
	case CurrentScene::ControlScene:
	case CurrentScene::OptionScene:
	{
		AEGfxTextureSet(optionbackground.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(optionbackground.pos.x, optionbackground.pos.y, 0.f, optionbackground.scale.x, optionbackground.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);


		for (size_t i = 0; i < sizeof(interactableButtonOption) / sizeof(interactableButtonOption[0]); i++)
		{
			AEGfxTextureSet(interactableButtonOption[i].pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(interactableButtonOption[i].pos.x, interactableButtonOption[i].pos.y, 0.f, interactableButtonOption[i].scale.x, interactableButtonOption[i].scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}

		AEGfxTextureSet(backButton.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(backButton.pos.x, backButton.pos.y, 0.f, backButton.scale.x, backButton.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		
		for (size_t i = 0; i < sizeof(optionBackgroundBar) / sizeof(optionBackgroundBar[0]); i++)
		{
			AEGfxTextureSet(interactableButtonMainMenu[i].pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(optionBackgroundBar[i].pos.x, optionBackgroundBar[i].pos.y, 0.f, optionBackgroundBar[i].scale.x, optionBackgroundBar[i].scale.y).m);
			AEGfxMeshDraw(pBlackSquareMesh, AE_GFX_MDM_TRIANGLES);
		}

		for (size_t i = 0; i < sizeof(optionSoundBar) / sizeof(optionSoundBar[0]); i++)
		{
			AEGfxTextureSet(optionSoundBar[i].pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(optionSoundBar[i].pos.x, optionSoundBar[i].pos.y, 0.f, optionSoundBar[i].scale.x, optionSoundBar[i].scale.y).m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}

		f32 width, height;

		const char* pText = "Back";
		AEGfxGetPrintSize(fontID, pText, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText, -width / 2 - 0.85f, -height / 2 - 0.9f, 0.5f, 1, 1, 1, 1);
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
		AEGfxGetPrintSize(fontID, pText, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText, -width / 2 - 0.31f, -height / 2, 0.5f, 1, 1, 1, 1);

		const char* pText1 = "No";
		AEGfxGetPrintSize(fontID, pText1, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText1, -width / 2 + 0.31f, -height / 2, 0.5f, 1, 1, 1, 1);
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
	AEGfxTextureUnload(boxBackground);
	AEGfxTextureUnload(audioUp);
	AEGfxTextureUnload(audioDown);
}

void BackMainMenu()
{
	currScene = CurrentScene::MainScene;
}

void GoNewGameLevel1()
{
	next = GameStates::GameLobby;
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