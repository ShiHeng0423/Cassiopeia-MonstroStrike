/*!************************************************************************
  \file					GameState_Mainmenu.cpp
  \project name			Monstrostrike
  \primary author		Teng Shi Heng (85%)
  \secondary authors	Johny Yong Jun Siang (10%, implemented rolling credits), Goh Jun Jie (5%, drew the background art)
  \brief				File containing definitions of functions related to the main menu room state.
						Such as loading, initializing, update, drawing etc... of the level. It also includes
						private functions for updating the credits when credit page is updated.

All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/
#include "GameState_Mainmenu.h"
#include "Utils.h"
#include "GameStateManager.h"
#include "Inventory.h"
#include "main.h"
namespace
{
	enum CurrentScene
	{
		MAIN_SCENE,
		CREDIT_SCENE,
		CONTROL_SCENE,
		OPTION_SCENE,
		QUIT_SCENE
	};

	enum Interactable
	{
		START,
		LOAD,
		CREDIT,
		CONTROLS,
		OPTIONS,
		QUIT
	};

	//Main menu buttons
	Button interactableButtonMainMenu[6];

	//Option button
	Sprite optionbackground;
	Sprite optionSoundBar[2];
	Sprite optionBackgroundBar[2];
	Button interactableButtonOption[4];
	AEGfxTexture* audioUp;
	AEGfxTexture* audioDown;
	AEGfxTexture* gameControlsImg;


	Button backButton;
	Button confirmationButtion[2];

	AEGfxTexture* buttonTexture;
	AEGfxTexture* backgroundTexture;
	AEGfxTexture* boxBackground;


	AEGfxVertexList* pWhiteSquareMesh;
	AEGfxVertexList* pBlackSquareMesh;

	Sprite background;
	Sprite gameControls;

	//menu buttons
	//Start Game Option - "Start Game" or "New Game" (if multiple files and saved data)
	//Load Game Option(for games with save files) - "Load Saved Game" or "Load Save File"
	//Quit Game Option - "Quit Game" (with confirmation)
	//Credits - "Credits"
	//How To Play - "How to Play" or "Controls"
	//Options Menu - "Options"


	s8 currScene;
	f32 printTimer = 0.0f;

	void CreditAnimationUpdate();
	void CreditAnimationEnter();

	struct CreditSlides {
		Object slidesObj;
		AEVec2 velocity;

		AEGfxTexture* creditsSlides;
	} credits[6];

	f32 creditTime = 0;
	f32 creditHeightUpdate = 0;
	f32 maxCreditTime = 20.f;

	Object gameTitle;

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
	AEVec2Set(&optionSoundBar[1].pos, 0 - 250 * (0.5f - audioManager->GetSFXVolume()), -100); // bar 2
	AEVec2Set(&optionSoundBar[1].scale, 250 * audioManager->GetSFXVolume() * 2.f, 50);
	optionSoundBar[1].UpdateTransformMatrix();
}

void IncreaseBgmVolume()
{
	audioManager->IncreaseBGMVolume();
	AEVec2Set(&optionSoundBar[0].pos, 0 - 250 * (0.5f - audioManager->GetBGMVolume()), 0); // bar 1
	AEVec2Set(&optionSoundBar[0].scale, 250 * audioManager->GetBGMVolume() * 2.f, 50);
	optionSoundBar[0].UpdateTransformMatrix();
}

void DecreaseSfxVolume()
{
	audioManager->DecreaseSFXVolume();
	AEVec2Set(&optionSoundBar[1].pos, 0 - 250 * (0.5f - audioManager->GetSFXVolume()), -100); // bar 2
	AEVec2Set(&optionSoundBar[1].scale, 250 * audioManager->GetSFXVolume() * 2.f, 50);
	optionSoundBar[1].UpdateTransformMatrix();
}

void DecreaseBgmVolume()
{
	audioManager->DecreaseBGMVolume();
	AEVec2Set(&optionSoundBar[0].pos, 0 - 250 * (0.5f - audioManager->GetBGMVolume()), 0); // bar 1
	AEVec2Set(&optionSoundBar[0].scale, 250 * audioManager->GetBGMVolume() * 2.f, 50);
	optionSoundBar[0].UpdateTransformMatrix();
}

void Mainmenu_Load()
{
	buttonTexture = AEGfxTextureLoad("Assets/panel_brown.png");
	backgroundTexture = AEGfxTextureLoad("Assets/1.jpg");
	boxBackground = AEGfxTextureLoad("Assets/panelInset_beige.png");
	audioUp = AEGfxTextureLoad("Assets/UI_Sprite/arrowBrown_right.png");
	audioDown = AEGfxTextureLoad("Assets/UI_Sprite/arrowBrown_left.png");
	gameControlsImg = AEGfxTextureLoad("Assets/Keyboard_Keys/Game Control.png");

	credits[0].creditsSlides = AEGfxTextureLoad("Assets/Credits/Credit_Opening.png");
	credits[1].creditsSlides = AEGfxTextureLoad("Assets/Credits/Credit_1.png");
	credits[2].creditsSlides = AEGfxTextureLoad("Assets/Credits/Credit_2.png");
	credits[3].creditsSlides = AEGfxTextureLoad("Assets/Credits/Credit_3.png");
	credits[4].creditsSlides = AEGfxTextureLoad("Assets/Credits/Credit_4.png");
	credits[5].creditsSlides = AEGfxTextureLoad("Assets/Credits/Credit_Ending.png");

	gameTitle.pTex = AEGfxTextureLoad("Assets/GameTitle.png");

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
		interactableButtonMainMenu[i].UpdateTransformMatrix();

		switch (i)
		{
		case Interactable::START:
			interactableButtonMainMenu[i].Ptr = GoNewGameLevel1;
			break;
		case Interactable::LOAD:
			interactableButtonMainMenu[i].Ptr = GoLoadSaveLevel;
			break;
		case Interactable::CREDIT:
			interactableButtonMainMenu[i].Ptr = GoCreditScene;
			break;
		case Interactable::CONTROLS:
			interactableButtonMainMenu[i].Ptr = GoControlScene;
			break;
		case Interactable::OPTIONS:
			interactableButtonMainMenu[i].Ptr = GoOptionScene;
			break;
		case Interactable::QUIT:
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
	interactableButtonOption[0].UpdateTransformMatrix();

	AEVec2Set(&interactableButtonOption[1].pos, 200, 0);
	AEVec2Set(&interactableButtonOption[1].scale, 50, 50);
	interactableButtonOption[1].Ptr = IncreaseBgmVolume;
	interactableButtonOption[1].pTex = audioUp;
	interactableButtonOption[1].UpdateTransformMatrix();

	AEVec2Set(&interactableButtonOption[2].pos, -200, -100);
	AEVec2Set(&interactableButtonOption[2].scale, 50, 50);
	interactableButtonOption[2].Ptr = DecreaseSfxVolume;
	interactableButtonOption[2].pTex = audioDown;
	interactableButtonOption[2].UpdateTransformMatrix();

	AEVec2Set(&interactableButtonOption[3].pos, 200, -100);
	AEVec2Set(&interactableButtonOption[3].scale, 50, 50);
	interactableButtonOption[3].Ptr = IncreaseSfxVolume;
	interactableButtonOption[3].pTex = audioUp;
	interactableButtonOption[3].UpdateTransformMatrix();

	AEVec2Set(&optionSoundBar[0].pos, 0, 0); // bar 1
	AEVec2Set(&optionSoundBar[0].scale, 250, 50);
	optionSoundBar[0].UpdateTransformMatrix();

	AEVec2Set(&optionSoundBar[1].pos, 0, -100); // bar 2
	AEVec2Set(&optionSoundBar[1].scale, 250, 50);
	optionSoundBar[1].UpdateTransformMatrix();

	AEVec2Set(&optionBackgroundBar[0].pos, 0, 0); // bar 1
	AEVec2Set(&optionBackgroundBar[0].scale, 250, 50);
	optionBackgroundBar[0].UpdateTransformMatrix();

	AEVec2Set(&optionBackgroundBar[1].pos, 0, -100); // bar 2
	AEVec2Set(&optionBackgroundBar[1].scale, 250, 50);
	optionBackgroundBar[1].UpdateTransformMatrix();

	optionbackground.pTex = boxBackground;
	optionbackground.scale.x = 1000.f;
	optionbackground.scale.y = 500.f;
	optionbackground.UpdateTransformMatrix();

	backButton.pTex = buttonTexture;
	AEVec2Set(&backButton.scale, 250.f, 80.f);
	AEVec2Set(&backButton.pos, -675.f, -410.f);
	backButton.Ptr = BackMainMenu;
	backButton.UpdateTransformMatrix();

	confirmationButtion[0].pTex = buttonTexture;
	AEVec2Set(&confirmationButtion[0].scale, 250.f, 80.f);
	AEVec2Set(&confirmationButtion[0].pos, 250.f, 0.f);
	confirmationButtion[0].Ptr = BackMainMenu;
	confirmationButtion[0].UpdateTransformMatrix();

	confirmationButtion[1].pTex = buttonTexture;
	AEVec2Set(&confirmationButtion[1].scale, 250.f, 80.f);
	AEVec2Set(&confirmationButtion[1].pos, -250.f, 0.f);
	confirmationButtion[1].Ptr = GoQuitGame;
	confirmationButtion[1].UpdateTransformMatrix();

	background.pTex = backgroundTexture;
	background.scale.x = 1600;
	background.scale.y = 900;
	background.UpdateTransformMatrix();

	gameControls.pTex = gameControlsImg;
	gameControls.scale.x = 600;
	gameControls.scale.y = 400;
	gameControls.UpdateTransformMatrix();

	currScene = CurrentScene::MAIN_SCENE;

	audioManager->PlayAudio(true, Audio_List::MAINMENU_SONG);

	gameTitle.scale.x = (f32)AEGfxGetWindowWidth() * 0.5f;
	gameTitle.scale.y = (f32)AEGfxGetWindowHeight() * 0.15f;

	gameTitle.pos.x = 0.f;
	gameTitle.pos.y = (f32)AEGfxGetWindowHeight() * 0.3f;

	gameTitle.UpdateTransformMatrix();
}

void Mainmenu_Update()
{
	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		s32 x, y;
		AEInputGetCursorPosition(&x, &y);
		AEVec2 mousePos{0, 0};
		mousePos.x = x - AEGfxGetWindowWidth() * 0.5f;
		mousePos.y = AEGfxGetWindowHeight() * 0.5f - y;

		switch (currScene)
		{
		case CurrentScene::MAIN_SCENE:
			for (size_t i = 0; i < sizeof(interactableButtonMainMenu) / sizeof(interactableButtonMainMenu[0]); i++)
			{
				if (AETestPointToRect(&mousePos, &interactableButtonMainMenu[i].pos,
				                      interactableButtonMainMenu[i].scale.x, interactableButtonMainMenu[i].scale.y))
					interactableButtonMainMenu[i].Ptr();
			}
			break;
		case CurrentScene::CREDIT_SCENE:
			if (AETestPointToRect(&mousePos, &backButton.pos, backButton.scale.x, backButton.scale.y))
				backButton.Ptr();
			break;
		case CurrentScene::CONTROL_SCENE:
			if (AETestPointToRect(&mousePos, &backButton.pos, backButton.scale.x, backButton.scale.y))
				backButton.Ptr();
			break;
		case CurrentScene::OPTION_SCENE:
			for (size_t i = 0; i < sizeof(interactableButtonOption) / sizeof(interactableButtonOption[0]); i++)
			{
				if (AETestPointToRect(&mousePos, &interactableButtonOption[i].pos, interactableButtonOption[i].scale.x,
				                      interactableButtonOption[i].scale.y))
					interactableButtonOption[i].Ptr();
			}
			if (AETestPointToRect(&mousePos, &backButton.pos, backButton.scale.x, backButton.scale.y))
				backButton.Ptr();
			break;
		case CurrentScene::QUIT_SCENE:
			if (AETestPointToRect(&mousePos, &confirmationButtion[0].pos, confirmationButtion[0].scale.x,
			                      confirmationButtion[0].scale.y))
				confirmationButtion[0].Ptr();
			if (AETestPointToRect(&mousePos, &confirmationButtion[1].pos, confirmationButtion[1].scale.x,
			                      confirmationButtion[1].scale.y))
				confirmationButtion[1].Ptr();
			break;
		default:
			break;
		}
	}

	if (currScene == CurrentScene::CREDIT_SCENE)
	{
		CreditAnimationUpdate();
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

	if (currScene != CurrentScene::CREDIT_SCENE)
	{
		AEGfxTextureSet(background.pTex, 0, 0);
		AEGfxSetTransform(background.transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(gameTitle.pTex, 0, 0);
		AEGfxSetTransform(gameTitle.transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
	}

	switch (currScene)
	{
	case CurrentScene::MAIN_SCENE:
	{
		for (size_t i = 0; i < sizeof(interactableButtonMainMenu) / sizeof(interactableButtonMainMenu[0]); i++)
		{
			AEGfxTextureSet(interactableButtonMainMenu[i].pTex, 0, 0);
			AEGfxSetTransform(interactableButtonMainMenu[i].transform.m);
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

		const char* pText5 = "Quit Game";
		AEGfxGetPrintSize(fontID, pText5, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText5, -width / 2, -height / 2 - 0.88f, 0.5f, 1, 1, 1, 1);
		break;
	}
	case CurrentScene::CREDIT_SCENE:
	{

		f32 width, height;

		for (size_t i = 0; i < (sizeof(credits) / sizeof(credits[0])); ++i)
		{
			AEGfxTextureSet(credits[i].creditsSlides, 0, 0);
			AEGfxSetTransform(credits[i].slidesObj.transform.m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}

		AEGfxTextureSet(backButton.pTex, 0, 0);
		AEGfxSetTransform(backButton.transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		const char* pText = "Back";
		AEGfxGetPrintSize(fontID, pText, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText, -width / 2 - 0.85f, -height / 2 - 0.9f, 0.5f, 1, 1, 1, 1);
		break;
	}
	case CurrentScene::CONTROL_SCENE:
	{
		AEGfxTextureSet(optionbackground.pTex, 0, 0);
		AEGfxSetTransform(optionbackground.transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(gameControls.pTex, 0, 0);
		AEGfxSetTransform(gameControls.transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(backButton.pTex, 0, 0);
		AEGfxSetTransform(backButton.transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		f32 width, height;

		const char* pText = "Back";
		AEGfxGetPrintSize(fontID, pText, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText, -width / 2 - 0.85f, -height / 2 - 0.9f, 0.5f, 1, 1, 1, 1);
		break;
	}
	case CurrentScene::OPTION_SCENE:
	{
		AEGfxTextureSet(optionbackground.pTex, 0, 0);
		AEGfxSetTransform(optionbackground.transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);


		for (size_t i = 0; i < sizeof(interactableButtonOption) / sizeof(interactableButtonOption[0]); i++)
		{
			AEGfxTextureSet(interactableButtonOption[i].pTex, 0, 0);
			AEGfxSetTransform(interactableButtonOption[i].transform.m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}
		AEGfxTextureSet(backButton.pTex, 0, 0);
		AEGfxSetTransform(backButton.transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		for (size_t i = 0; i < sizeof(optionBackgroundBar) / sizeof(optionBackgroundBar[0]); i++)
		{
			AEGfxTextureSet(interactableButtonMainMenu[i].pTex, 0, 0);
			AEGfxSetTransform(optionBackgroundBar[i].transform.m);
			AEGfxMeshDraw(pBlackSquareMesh, AE_GFX_MDM_TRIANGLES);
		}

		for (size_t i = 0; i < sizeof(optionSoundBar) / sizeof(optionSoundBar[0]); i++)
		{
			AEGfxTextureSet(optionSoundBar[i].pTex, 0, 0);
			AEGfxSetTransform(optionSoundBar[i].transform.m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		}

		f32 width, height;

		const char* pText5 = "Sound Setting";
		AEGfxGetPrintSize(fontID, pText5, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText5, -width / 2, -height / 2 + 0.3f, 0.5f, 1, 1, 1, 1);

		const char* pText = "BGM";
		AEGfxGetPrintSize(fontID, pText, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText, -width / 2 - 0.35f, -height / 2, 0.5f, 1, 1, 1, 1);

		const char* pText1 = "SFX";
		AEGfxGetPrintSize(fontID, pText1, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText1, -width / 2 - 0.35f, -height / 2 - 0.225f, 0.5f, 1, 1, 1, 1);

		const char* pText2 = "Back";
		AEGfxGetPrintSize(fontID, pText2, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText2, -width / 2 - 0.85f, -height / 2 - 0.9f, 0.5f, 1, 1, 1, 1);
		break;
	}
	case CurrentScene::QUIT_SCENE:
	{
		AEGfxTextureSet(optionbackground.pTex, 0, 0);
		AEGfxSetTransform(optionbackground.transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(confirmationButtion[0].pTex, 0, 0);
		AEGfxSetTransform(confirmationButtion[0].transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(confirmationButtion[1].pTex, 0, 0);
		AEGfxSetTransform(confirmationButtion[1].transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		f32 width, height;

		const char* pText5 = "Do you want to quit?";
		AEGfxGetPrintSize(fontID, pText5, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText5, -width / 2, -height / 2 + 0.3f, 0.5f, 1, 1, 1, 1);

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
	AEGfxMeshFree(pBlackSquareMesh);
	AEGfxTextureUnload(buttonTexture);
	AEGfxTextureUnload(backgroundTexture);
	AEGfxTextureUnload(boxBackground);
	AEGfxTextureUnload(audioUp);
	AEGfxTextureUnload(audioDown);
	AEGfxTextureUnload(gameControlsImg);
	AEGfxTextureUnload(gameTitle.pTex);


	//Unload all slides
	for (int i = 0; i < (sizeof(credits) / sizeof(credits[0])); i++)
	{
		AEGfxTextureUnload(credits[i].creditsSlides);
	}
}

void BackMainMenu()
{
	currScene = CurrentScene::MAIN_SCENE;
}

void GoNewGameLevel1()
{
	next = GameStates::GAME_LOBBY;
	Inventory::isNewAccount = true;

	gameManager->getDebugManager()->setDebugClose();
}

void GoLoadSaveLevel()
{
	//next = GAME_LOBBY;
	Inventory::isNewAccount = false;
	const char* player_filepath = "Assets/SaveFiles/player_inventory.json";
	Inventory::ReadJsonFile(player_filepath);

	next = Inventory::fileLoadedState;
	current = Inventory::fileLoadedState - 1;

	gameManager->getDebugManager()->setDebugClose();
}

void GoCreditScene()
{
	CreditAnimationEnter();

	currScene = CurrentScene::CREDIT_SCENE;
}

void GoControlScene()
{
	currScene = CurrentScene::CONTROL_SCENE;
}

void GoOptionScene()
{
	currScene = CurrentScene::OPTION_SCENE;
}

void GoConfirmQuitScene()
{
	currScene = CurrentScene::QUIT_SCENE;
}

void GoQuitGame()
{
	next = GameStates::QUIT;
}

namespace {
	void CreditAnimationUpdate()
	{
		creditTime += (f32)AEFrameRateControllerGetFrameTime();

		for (int i = 0; i < sizeof(credits) / sizeof(credits[0]); i++)
		{
			credits[i].slidesObj.pos.y += creditHeightUpdate * (f32)AEFrameRateControllerGetFrameTime();
			credits[i].slidesObj.UpdateTransformMatrix();
		}

		if (creditTime >= maxCreditTime)
		{
			BackMainMenu();
		}
	}

	void CreditAnimationEnter() //Initialise the slides position
	{
		creditTime = 0.f;

		f32 slideDisplayDuration = 2.f; //Total animation time will be number of slides * this value

		maxCreditTime = (sizeof(credits) / sizeof(credits[0]) - 1) * slideDisplayDuration;

		creditHeightUpdate = (f32)AEGfxGetWindowHeight() / slideDisplayDuration;

		for (int i = 0; i < sizeof(credits) / sizeof(credits[0]); i++)
		{
			credits[i].slidesObj.pos.x = 0.f; //All at center

			if (i == 0)
			{
				credits[i].slidesObj.pos.y = (f32)AEGfxGetWindowHeight() * -0.5f; //First Position at center y
			}
			else
			{

				credits[i].slidesObj.pos.y = credits[0].slidesObj.pos.y - i * (f32)AEGfxGetWindowHeight(); //Need to below each slides

			}
			credits[i].slidesObj.scale.x = (f32)AEGfxGetWindowWidth();
			credits[i].slidesObj.scale.y = (f32)AEGfxGetWindowHeight();
			credits[i].slidesObj.UpdateTransformMatrix();
		}
	}

}