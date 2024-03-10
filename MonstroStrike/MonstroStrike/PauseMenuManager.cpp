#include "PauseMenuManager.h"
#include "GameStateManager.h"
#include "TransformMatrix.h"
#include "Utils.h"
#include "main.h"

//private namespace for this file
namespace {

	AEGfxTexture* PauseMenuBackground;
	AEGfxTexture* ButtonFrame;

	Button PauseMenuButtons[4];
	Button QuitToMainmenu[2];
	Button BackButton;

	Sprite_V2 pauseMenuBackground;



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
	PauseMenuBackground = AEGfxTextureLoad("Assets/UI_Sprite/Transparent center/panel-transparent-center-015.png");
	ButtonFrame = AEGfxTextureLoad("Assets/UI_Sprite/Border/panel-border-015.png");

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
	AEGfxTextureUnload(PauseMenuBackground);
	AEGfxTextureUnload(ButtonFrame);
	AEGfxMeshFree(pWhiteSquareMesh);
}

void PauseMenu_Manager::Init(Camera* cam)
{
	for (size_t i = 0; i < sizeof(PauseMenuButtons) / sizeof(PauseMenuButtons[0]); i++)
	{
		PauseMenuButtons[i].pTex = ButtonFrame;
		AEVec2Set(&PauseMenuButtons[i].scale, 250.f, 80.f);
		AEVec2Set(&PauseMenuButtons[i].pos, cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y - 100.f * i + 100.f);

		switch (i)
		{
		case Interactable::RESUME:
			PauseMenuButtons[i].Ptr = ResumeGame;
			break;
		case Interactable::RETURN:
			PauseMenuButtons[i].Ptr = ReturnLobby;
			break;
		case Interactable::CONTROLS:
			PauseMenuButtons[i].Ptr = OpenControls;
			break;
		case Interactable::GO_MAINMENU:
			PauseMenuButtons[i].Ptr = QuitConfirmation;
			break;
		default:
			break;
		}
	}

	QuitToMainmenu[0].pTex = ButtonFrame;
	AEVec2Set(&QuitToMainmenu[0].scale, 250.f, 80.f);
	AEVec2Set(&QuitToMainmenu[0].pos, 250.f + cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y);
	QuitToMainmenu[0].Ptr = BackPauseMenu;

	QuitToMainmenu[1].pTex = ButtonFrame;
	AEVec2Set(&QuitToMainmenu[1].scale, 250.f, 80.f);
	AEVec2Set(&QuitToMainmenu[1].pos, -250.f + cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y);
	QuitToMainmenu[1].Ptr = QuitMainmenu;

	BackButton.pTex = ButtonFrame;
	AEVec2Set(&BackButton.scale, 250.f, 80.f);
	AEVec2Set(&BackButton.pos, 250.f + cam->GetCameraWorldPoint().x, cam->GetCameraWorldPoint().y);
	BackButton.Ptr = BackPauseMenu;

	currScene = CurrentScene::MAIN_SCENE;

	pauseMenuBackground.pTex = PauseMenuBackground;
	pauseMenuBackground.scale.x = 1000.f;
	pauseMenuBackground.scale.y = 500.f;
	pauseMenuBackground.pos = cam->GetCameraWorldPoint();
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

			for (size_t i = 0; i < sizeof(PauseMenuButtons) / sizeof(PauseMenuButtons[0]); i++)
				AEVec2Set(&PauseMenuButtons[i].pos, x, y - 100.f * i + 100.f);

			AEVec2Set(&QuitToMainmenu[0].pos, 250.f + x, y);
			AEVec2Set(&QuitToMainmenu[1].pos, -250.f + x, y);

			AEVec2Set(&BackButton.pos, 280.f + x, -180.f + y);

			AEVec2Set(&pauseMenuBackground.pos, x, y);
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
				for (size_t i = 0; i < sizeof(PauseMenuButtons) / sizeof(PauseMenuButtons[0]); i++)
				{
					AEVec2 translateOrigin = PauseMenuButtons[i].pos;
					translateOrigin.x -= x;
					translateOrigin.y -= y;
					if (AETestPointToRect(&mousePos, &translateOrigin, PauseMenuButtons[i].scale.x, PauseMenuButtons[i].scale.y))
						PauseMenuButtons[i].Ptr();
				}
				break;
			}
			case CurrentScene::CONTROL_SCENE:
			{
				AEVec2 translateOrigin = BackButton.pos;
				translateOrigin.x -= x;
				translateOrigin.y -= y;
				if (AETestPointToRect(&mousePos, &translateOrigin, BackButton.scale.x, BackButton.scale.y))
					BackButton.Ptr();
				break;
			}
			case CurrentScene::QUIT_SCENE:
			{
				AEVec2 translateOrigin = QuitToMainmenu[0].pos;
				translateOrigin.x -= x;
				translateOrigin.y -= y;
				if (AETestPointToRect(&mousePos, &translateOrigin, QuitToMainmenu[0].scale.x, QuitToMainmenu[0].scale.y))
					QuitToMainmenu[0].Ptr();
				translateOrigin = QuitToMainmenu[1].pos;
				AEGfxGetCamPosition(&x, &y);
				translateOrigin.x -= x;
				translateOrigin.y -= y;
				if (AETestPointToRect(&mousePos, &translateOrigin, QuitToMainmenu[1].scale.x, QuitToMainmenu[1].scale.y))
					QuitToMainmenu[1].Ptr();
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
		AEGfxTextureSet(PauseMenuBackground, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(pauseMenuBackground.pos.x, pauseMenuBackground.pos.y, 0.f, pauseMenuBackground.scale.x, pauseMenuBackground.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);
		for (size_t i = 0; i < sizeof(PauseMenuButtons) / sizeof(PauseMenuButtons[0]); i++)
		{
			AEGfxTextureSet(PauseMenuButtons[i].pTex, 0, 0);
			AEGfxSetTransform(ObjectTransformationMatrixSet(PauseMenuButtons[i].pos.x, PauseMenuButtons[i].pos.y, 0.f, PauseMenuButtons[i].scale.x, PauseMenuButtons[i].scale.y).m);
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
		AEGfxTextureSet(PauseMenuBackground, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(pauseMenuBackground.pos.x, pauseMenuBackground.pos.y, 0.f, pauseMenuBackground.scale.x, pauseMenuBackground.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(BackButton.pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(BackButton.pos.x, BackButton.pos.y, 0.f, BackButton.scale.x, BackButton.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		f32 width, height;

		const char* pText = "Back";
		AEGfxGetPrintSize(fontID, pText, 0.5f, &width, &height);
		AEGfxPrint(fontID, pText, -width / 2 + 0.35f, -height / 2 -0.4f, 0.5f, 1, 1, 1, 1);
		break;
	}
	case CurrentScene::QUIT_SCENE:
	{
		AEGfxTextureSet(PauseMenuBackground, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(pauseMenuBackground.pos.x, pauseMenuBackground.pos.y, 0.f, pauseMenuBackground.scale.x, pauseMenuBackground.scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(QuitToMainmenu[0].pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(QuitToMainmenu[0].pos.x, QuitToMainmenu[0].pos.y, 0.f, QuitToMainmenu[0].scale.x, QuitToMainmenu[0].scale.y).m);
		AEGfxMeshDraw(pWhiteSquareMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(QuitToMainmenu[1].pTex, 0, 0);
		AEGfxSetTransform(ObjectTransformationMatrixSet(QuitToMainmenu[1].pos.x, QuitToMainmenu[1].pos.y, 0.f, QuitToMainmenu[1].scale.x, QuitToMainmenu[1].scale.y).m);
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