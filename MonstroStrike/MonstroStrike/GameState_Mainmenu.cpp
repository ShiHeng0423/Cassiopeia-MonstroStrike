#include "GameState_Mainmenu.h"
#include "Utils.h"
#include "GameStateManager.h"
#include "main.h"
#include "TextPrinting.h"

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
	std::vector<PrintedCharacter> printedCharVec;
	f32 printTimer = 0.0f;
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
	boxBackground = AEGfxTextureLoad("Assets/UI_Sprite/Transparent center/panel-transparent-center-015.png");
	audioUp = AEGfxTextureLoad("Assets/UI_Sprite/arrowBrown_right.png");
	audioDown = AEGfxTextureLoad("Assets/UI_Sprite/arrowBrown_left.png");
	gameControlsImg = AEGfxTextureLoad("Assets/Keyboard_Keys/Game Control.png");

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
		case CurrentScene::CONTROL_SCENE:
			if (AETestPointToRect(&mousePos, &backButton.pos, backButton.scale.x, backButton.scale.y))
				backButton.Ptr();
			break;
		case CurrentScene::OPTION_SCENE:
			for (size_t i = 0; i < sizeof(interactableButtonOption) / sizeof(interactableButtonOption[0]); i++)
			{
				if (AETestPointToRect(&mousePos, &interactableButtonOption[i].pos, interactableButtonOption[i].scale.x, interactableButtonOption[i].scale.y))
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
	AEGfxSetTransform(background.transform.m);
	AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

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
		//const char* testText = "Did you know? Mejiro Mcqueen is my first UD horse? HAHAHAHAHHAHAHAHAHAHAHAHAHAHAHAHAHAH";
		//PrintTextOverTime(testText, 0.01f, -1.f, 0.f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, pFont, printedCharVec, & printTimer);
		////AEGfxPrint(pFont, pText, -width / 2, -height / 2 + 0.22f, 0.5f, 1, 1, 1, 1);

		auto pText = "Start";
		AEGfxGetPrintSize(fontID, pText, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText, -width / 2, -height / 2 + 0.22f, 0.5f, 1, 1, 1, 1);

		auto pText1 = "Load";
		AEGfxGetPrintSize(fontID, pText1, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText1, -width / 2, -height / 2, 0.5f, 1, 1, 1, 1);

		auto pText2 = "Credit";
		AEGfxGetPrintSize(fontID, pText2, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText2, -width / 2, -height / 2 - 0.22f, 0.5f, 1, 1, 1, 1);

		auto pText3 = "Controls";
		AEGfxGetPrintSize(fontID, pText3, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText3, -width / 2, -height / 2 - 0.44f, 0.5f, 1, 1, 1, 1);

		auto pText4 = "Options";
		AEGfxGetPrintSize(fontID, pText4, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText4, -width / 2, -height / 2 - 0.66f, 0.5f, 1, 1, 1, 1);

		auto pText5 = "Quit";
		AEGfxGetPrintSize(fontID, pText5, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText5, -width / 2, -height / 2 - 0.88f, 0.5f, 1, 1, 1, 1);
		break;
	}
	case CurrentScene::CREDIT_SCENE:
	{
		AEGfxTextureSet(optionbackground.pTex, 0, 0);
		AEGfxSetTransform(optionbackground.transform.m);
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

		auto pText = "Back";
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

		auto pText = "BGM";
		AEGfxGetPrintSize(fontID, pText, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText, -width / 2 - 0.35f, -height / 2, 0.5f, 1, 1, 1, 1);

		auto pText1 = "SFX";
		AEGfxGetPrintSize(fontID, pText1, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText1, -width / 2 - 0.35f, -height / 2 - 0.225f, 0.5f, 1, 1, 1, 1);

		auto pText2 = "Back";
		AEGfxGetPrintSize(fontID, pText2, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText2, -width / 2 - 0.85f, -height / 2 - 0.9f, 0.5f, 1, 1, 1, 1);
		break;

	}
	case CurrentScene::QUIT_SCENE:
	{
		AEGfxTextureSet(confirmationButtion[0].pTex, 0, 0);
		AEGfxSetTransform(confirmationButtion[0].transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(confirmationButtion[1].pTex, 0, 0);
		AEGfxSetTransform(confirmationButtion[1].transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		f32 width, height;

		auto pText = "Yes";
		AEGfxGetPrintSize(fontID, pText, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText, -width / 2 - 0.31f, -height / 2, 0.5f, 1, 1, 1, 1);

		auto pText1 = "No";
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
}

void BackMainMenu()
{
	currScene = CurrentScene::MAIN_SCENE;
}

void GoNewGameLevel1()
{
	next = GameStates::GAME_LOBBY;
}

void GoLoadSaveLevel()
{
}

void GoCreditScene()
{
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
