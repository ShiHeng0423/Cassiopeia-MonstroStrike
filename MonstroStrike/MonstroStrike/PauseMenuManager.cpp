#include "PauseMenuManager.h"
#include "GameStateManager.h"
#include "Utils.h"
#include "main.h"

//private namespace for this file
namespace {

	AEGfxTexture* pauseMenuBackgroundTex;
	AEGfxTexture* buttonFrame;
	AEGfxTexture* gameControlsImg;

	Button pauseMenuButtons[4];
	Button quitToMainmenu[2];
	Button backButton;

	Sprite pauseMenuBackground;
	Sprite gameControls;



	AEGfxVertexList* pWhiteSquareMesh;
}

int currScene = 0;

void ResumeGame() { currScene = CurrentScene::MAIN_SCENE; }
void ReturnLobby() { next = GameStates::GAME_LOBBY; }
void OpenControls() { currScene = CurrentScene::CONTROL_SCENE; }
void QuitMainmenu() { next = GameStates::SPLASHSCREEN; }
void QuitConfirmation() { currScene = CurrentScene::QUIT_SCENE; }
void BackPauseMenu() { currScene = CurrentScene::PAUSE_SCENE; }

PauseMenu_Manager::PauseMenu_Manager()
{
	pauseMenuBackgroundTex = AEGfxTextureLoad("Assets/panelInset_beige.png");
	buttonFrame = AEGfxTextureLoad("Assets/panel_brown.png");
	gameControlsImg = AEGfxTextureLoad("Assets/Keyboard_Keys/Game Control.png");

	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // top-left: blue

	pWhiteSquareMesh = AEGfxMeshEnd();
}

PauseMenu_Manager::~PauseMenu_Manager()
{
	AEGfxTextureUnload(pauseMenuBackgroundTex);
	AEGfxTextureUnload(buttonFrame);
	AEGfxTextureUnload(gameControlsImg);
	AEGfxMeshFree(pWhiteSquareMesh);
}

void PauseMenu_Manager::Init(Camera* cam)
{
	for (size_t i = 0; i < sizeof(pauseMenuButtons) / sizeof(pauseMenuButtons[0]); i++)
	{
		pauseMenuButtons[i].pTex = buttonFrame;
		AEVec2Set(&pauseMenuButtons[i].scale, 400.f, 80.f);
		AEVec2Set(&pauseMenuButtons[i].pos, cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y - 100.f * i + 100.f);

		switch (i)
		{
		case Interactable::RESUME:
			pauseMenuButtons[i].Ptr = ResumeGame;
			break;
		case Interactable::RETURN:
			pauseMenuButtons[i].Ptr = ReturnLobby;
			break;
		case Interactable::CONTROLS:
			pauseMenuButtons[i].Ptr = OpenControls;
			break;
		case Interactable::GO_MAINMENU:
			pauseMenuButtons[i].Ptr = QuitConfirmation;
			break;
		default:
			break;
		}
	}

	quitToMainmenu[0].pTex = buttonFrame;
	AEVec2Set(&quitToMainmenu[0].scale, 250.f, 80.f);
	AEVec2Set(&quitToMainmenu[0].pos, 250.f + cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y);
	quitToMainmenu[0].Ptr = BackPauseMenu;

	quitToMainmenu[1].pTex = buttonFrame;
	AEVec2Set(&quitToMainmenu[1].scale, 250.f, 80.f);
	AEVec2Set(&quitToMainmenu[1].pos, -250.f + cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y);
	quitToMainmenu[1].Ptr = QuitMainmenu;

	backButton.pTex = buttonFrame;
	AEVec2Set(&backButton.scale, 250.f, 80.f);
	AEVec2Set(&backButton.pos, 250.f + cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y);
	backButton.Ptr = BackPauseMenu;

	currScene = CurrentScene::MAIN_SCENE;

	pauseMenuBackground.pTex = pauseMenuBackgroundTex;
	pauseMenuBackground.scale.x = 1000.f;
	pauseMenuBackground.scale.y = 500.f;
	pauseMenuBackground.pos = cam->GetCameraWorldPoint();

	gameControls.pTex = gameControlsImg;
	gameControls.scale.x = 600;
	gameControls.scale.y = 400;
	gameControls.pos = cam->GetCameraWorldPoint();
}

void PauseMenu_Manager::Update(Camera* cam)
{
	if (AEInputCheckTriggered(AEVK_ESCAPE))
	{
		f32 x, y;
		AEGfxGetCamPosition(&x, &y);
		if (currScene == CurrentScene::MAIN_SCENE)
		{
			//rmb to freeze game update
			currScene = CurrentScene::PAUSE_SCENE;

			for (size_t i = 0; i < sizeof(pauseMenuButtons) / sizeof(pauseMenuButtons[0]); i++)
			{
				AEVec2Set(&pauseMenuButtons[i].pos, x, y - 100.f * i + 100.f);
				pauseMenuButtons[i].UpdateTransformMatrix();
			}

			AEVec2Set(&quitToMainmenu[0].pos, 250.f + x, y);
			quitToMainmenu[0].UpdateTransformMatrix();

			AEVec2Set(&quitToMainmenu[1].pos, -250.f + x, y);
			quitToMainmenu[1].UpdateTransformMatrix();

			AEVec2Set(&backButton.pos, 280.f + x, -180.f + y);
			backButton.UpdateTransformMatrix();

			AEVec2Set(&pauseMenuBackground.pos, x, y);
			pauseMenuBackground.UpdateTransformMatrix();

			AEVec2Set(&gameControls.pos, x, y);
			gameControls.UpdateTransformMatrix();
		}
		else
		{
			//unfreeze the game
			currScene = CurrentScene::MAIN_SCENE;
		}
	}

	if (currScene == CurrentScene::PAUSE_SCENE || currScene == CurrentScene::CONTROL_SCENE || currScene == CurrentScene::QUIT_SCENE)
	{
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			s32 cx, cy;
			AEInputGetCursorPosition(&cx, &cy);
			AEVec2 mousePos{ 0,0 };
			mousePos.x = cx - AEGfxGetWindowWidth() * 0.5f;
			mousePos.y = AEGfxGetWindowHeight() * 0.5f - cy;
			f32 x, y;
			AEGfxGetCamPosition(&x, &y);
			switch (currScene)
			{
			case CurrentScene::PAUSE_SCENE:
			{
				for (size_t i = 0; i < sizeof(pauseMenuButtons) / sizeof(pauseMenuButtons[0]); i++)
				{
					AEVec2 translateOrigin = pauseMenuButtons[i].pos;
					translateOrigin.x -= x;
					translateOrigin.y -= y;
					if (AETestPointToRect(&mousePos, &translateOrigin, pauseMenuButtons[i].scale.x, pauseMenuButtons[i].scale.y))
						pauseMenuButtons[i].Ptr();
				}
				break;
			}
			case CurrentScene::CONTROL_SCENE:
			{
				AEVec2 translateOrigin = backButton.pos;
				translateOrigin.x -= x;
				translateOrigin.y -= y;
				if (AETestPointToRect(&mousePos, &translateOrigin, backButton.scale.x, backButton.scale.y))
					backButton.Ptr();
				break;
			}
			case CurrentScene::QUIT_SCENE:
			{
				AEVec2 translateOrigin = quitToMainmenu[0].pos;
				translateOrigin.x -= x;
				translateOrigin.y -= y;
				if (AETestPointToRect(&mousePos, &translateOrigin, quitToMainmenu[0].scale.x, quitToMainmenu[0].scale.y))
					quitToMainmenu[0].Ptr();
				translateOrigin = quitToMainmenu[1].pos;
				AEGfxGetCamPosition(&x, &y);
				translateOrigin.x -= x;
				translateOrigin.y -= y;
				if (AETestPointToRect(&mousePos, &translateOrigin, quitToMainmenu[1].scale.x, quitToMainmenu[1].scale.y))
					quitToMainmenu[1].Ptr();
				break;
			}
			default:
				break;
			}
		}
	}
}

void PauseMenu_Manager::Render()
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	f32 width, height;


	switch (currScene)
	{
	case CurrentScene::PAUSE_SCENE:
	{
		AEGfxTextureSet(pauseMenuBackgroundTex, 0, 0);
		AEGfxSetTransform(pauseMenuBackground.transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		for (size_t i = 0; i < sizeof(pauseMenuButtons) / sizeof(pauseMenuButtons[0]); i++)
		{
			AEGfxTextureSet(pauseMenuButtons[i].pTex, 0, 0);
			AEGfxSetTransform(pauseMenuButtons[i].transform.m);
			AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		}

		const char* pText = "Resume";
		AEGfxGetPrintSize(fontID, pText, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText, -width / 2, -height / 2 + 0.22f, 0.5f, 1, 1, 1, 1);

		const char* pText1 = "Back To Village";
		AEGfxGetPrintSize(fontID, pText1, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText1, -width / 2, -height / 2, 0.5f, 1, 1, 1, 1);

		const char* pText2 = "Controls";
		AEGfxGetPrintSize(fontID, pText2, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText2, -width / 2, -height / 2 - 0.22f, 0.5f, 1, 1, 1, 1);

		const char* pText3 = "Quit to Main menu";
		AEGfxGetPrintSize(fontID, pText3, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText3, -width / 2, -height / 2 - 0.44f, 0.5f, 1, 1, 1, 1);
		break;
	}
	case CurrentScene::CONTROL_SCENE:
	{
		AEGfxTextureSet(pauseMenuBackgroundTex, 0, 0);
		AEGfxSetTransform(pauseMenuBackground.transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(backButton.pTex, 0, 0);
		AEGfxSetTransform(backButton.transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(gameControls.pTex, 0, 0);
		AEGfxSetTransform(gameControls.transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		f32 width, height;

		const char* pText = "Back";
		AEGfxGetPrintSize(fontID, pText, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText, -width / 2 + 0.35f, -height / 2 -0.4f, 0.5f, 1, 1, 1, 1);
		break;
	}
	case CurrentScene::QUIT_SCENE:
	{
		AEGfxTextureSet(pauseMenuBackground.pTex, 0, 0);
		AEGfxSetTransform(pauseMenuBackground.transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(quitToMainmenu[0].pTex, 0, 0);
		AEGfxSetTransform(quitToMainmenu[0].transform.m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(quitToMainmenu[1].pTex, 0, 0);
		AEGfxSetTransform(quitToMainmenu[1].transform.m);
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