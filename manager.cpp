//======================================================
//
//マネージャ[manager.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "object.h"
#include "player.h"
#include "objectX.h"
#include "meshField.h"
#include "meshsky.h"
#include "gui.h"
#include "xmodel.h"
#include "objectBillboard.h"
#include "userdef.h"

//静的メンバ変数
const int CManager::INT_ZERO = 0;										//int型の0
const float CManager::FLT_ZERO = 0.0f;									//float型の0
const D3DXVECTOR2 CManager::VEC2_ZERO = D3DXVECTOR2(0.0f, 0.0f);		//vec2型の0
const D3DXVECTOR3 CManager::VEC3_ZERO = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//vec3型の0

//静的メンバ変数
CRenderer* CManager::m_pRenderer = nullptr;
CInputKeyboard* CManager::m_pInputKeyboard = nullptr;
CInputMouse* CManager::m_pInputMouse = nullptr;
CDebugProc* CManager::m_pDebProc = nullptr;
CCamera* CManager::m_pCamera = nullptr;
CTexture* CManager::m_pTextureEditorPop = nullptr;
CTexture* CManager::m_pTextureX = nullptr;
CTexture* CManager::m_pTextureSystem = nullptr;
CPlayer* CManager::m_pPlayer = nullptr;
CGUIAddObj* CManager::m_pGUIAddObj = nullptr;
CGUIChangeObj* CManager::m_pGUIChangeObj = nullptr;
int CManager::m_nFPS = 0;
DWORD CManager::m_dwFrameCount = 0;
CMeshField* CManager::m_pMeshField = nullptr;
CVariableManager* CManager::m_pVariableManager = nullptr;

//=================================
//コンストラクタ
//=================================
CManager::CManager()
{
	
}

//=================================
//デストラクタ
//=================================
CManager::~CManager()
{}

//=================================
//初期化
//=================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//生成
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputMouse = new CInputMouse;
	m_pRenderer = new CRenderer;
	m_pDebProc = new CDebugProc;
	m_pCamera = new CCamera;
	m_pTextureEditorPop = new CTexture;			//オブジェクト用
	m_pTextureX = new CTexture;					//Xファイル用
	m_pTextureSystem = new CTexture;			//システム用
	m_pPlayer = new CPlayer;
	m_pGUIAddObj = CGUIAddObj::Create();
	m_pGUIChangeObj = CGUIChangeObj::Create();
	m_pVariableManager = new CVariableManager;

	//レンダラー初期化
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{
		return E_FAIL;
	}

	//キーボード初期化
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//マウス初期化
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//デバッグ初期化
	m_pDebProc->Init();

	//カメラ初期化
	if (FAILED(m_pCamera->Init()))
	{
		return E_FAIL;
	}

	//ライト初期化
	CLight::Create(D3DXVECTOR3(0.22f, -0.87f, 0.44f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CLight::Create(D3DXVECTOR3(-0.18f, 0.88f, -0.44f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CLight::Create(D3DXVECTOR3(0.89f, -0.11f, 0.44f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//テクスチャ初期化
	if (FAILED(m_pTextureEditorPop->Load("data\\preload.txt")))
	{
		return E_FAIL;
	}

	//システムで必要なものをこっちで読み込み
	m_pTextureSystem->Regist("data\\TEXTURE\\Tex_AddObj.png");
	m_pTextureSystem->Regist("data\\TEXTURE\\Tex_Camera.png");
	m_pTextureSystem->Regist("data\\TEXTURE\\Tex_Light.png");

	//プレイヤー初期化
	if (FAILED(m_pPlayer->Init()))
	{
		return E_FAIL;
	}

	//変数定義
	m_pVariableManager->ReadUserDefData("data\\system.ini");

	//ImGUI系
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.IniFilename = nullptr;
	//ImGui::StyleColorsLight();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(m_pRenderer->GetDevice());// GetGlyphRangesJapanese

	m_pMeshField = CMeshField::Create(D3DXVECTOR3(-1280.0f, 0.0f, 1280.0f), CManager::VEC3_ZERO, 64.0f, 64.0f, 40, 40);
	m_pMeshField->BindTexture(m_pTextureSystem->Regist("data\\TEXTURE\\line.png"));
	m_pMeshField->SetTexType(CTexture::TYPE_SYSTEM);
	m_pMeshField->GetCollision()->SetVtx(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMeshField->Exclusion();

	CMeshSky* pMeshSky = CMeshSky::Create(CManager::VEC3_ZERO, CManager::VEC3_ZERO, 10000.0f, 8, 8);
	pMeshSky->BindTexture(m_pTextureSystem->Regist("data\\TEXTURE\\sky000.png"));
	pMeshSky->SetTexType(CTexture::TYPE_SYSTEM);

	//FPS計測器初期化
	m_nFPS = 0;
	m_dwFrameCount = 0;

	//できた
	return S_OK;
}

//=================================
//終了
//=================================
void CManager::Uninit(void)
{
	//オブジェクト終了+破棄
	CObject::ReleaseAll();

	//モデル破棄
	CXModel::UnloadAll();			//Xモデル

	//ImGUI
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//変数マネ破棄
	if (m_pVariableManager != nullptr)
	{
		m_pVariableManager->ReleaseAll();
		delete m_pVariableManager;
		m_pVariableManager = nullptr;
	}

	//プレイヤー破棄
	if (m_pPlayer != nullptr)
	{//プレイヤー終了
		m_pPlayer->Uninit();
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}

	//テクスチャ破棄
	if (m_pTextureSystem != nullptr)
	{//テクスチャ終了
		m_pTextureSystem->Unload();
		delete m_pTextureSystem;
		m_pTextureSystem = nullptr;
	}
	if (m_pTextureX != nullptr)
	{//テクスチャ終了
		m_pTextureX->Unload();
		delete m_pTextureX;
		m_pTextureX = nullptr;
	}
	if (m_pTextureEditorPop != nullptr)
	{//テクスチャ終了
		m_pTextureEditorPop->Unload();
		delete m_pTextureEditorPop;
		m_pTextureEditorPop = nullptr;
	}

	//ライト破棄
	CLight::ReleaseAll();

	//カメラ破棄
	if (m_pCamera != nullptr)
	{//カメラ終了
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	//デバッグ破棄
	if (m_pDebProc != nullptr)
	{//デバッグ終了
		m_pDebProc->Uninit();
		delete m_pDebProc;
		m_pDebProc = nullptr;
	}

	//マウス破棄
	if (m_pInputMouse != nullptr)
	{//マウス終了
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = nullptr;
	}

	//キーボード破棄
	if (m_pInputKeyboard != nullptr)
	{//キーボード終了
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	//レンダラー破棄
	if (m_pRenderer != nullptr)
	{//レンダラー終了
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
}

//=================================
//更新
//=================================
void CManager::Update(void)
{
	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	m_pInputKeyboard->Update();
	m_pInputMouse->Update();
	m_pCamera->Update();
	m_pPlayer->Update();
	m_pRenderer->Update();

	//この時点で死亡フラグが立っているオブジェを殺す
	CObject::Death();

	//FPS計測器の処理
	m_dwFrameCount++;

	//デバッグ表示
	m_pDebProc->Print("FPS:%d\n", m_nFPS);
	m_pDebProc->Print("[操作方法]\n");
	m_pDebProc->Print("右クリック+移動:視点移動\n");
	m_pDebProc->Print("WASD:プレイヤー（カメラ）移動\n");

	ImGui::EndFrame();
}

//=================================
//描画
//=================================
void CManager::Draw(void)
{
	m_pRenderer->Draw();
}

//=================================
//FPS計測
//=================================
void CManager::CheckFPS(DWORD dwCurrentTime, DWORD dwExecLastTime)
{
	m_nFPS = (m_dwFrameCount * 1000) / (dwCurrentTime - dwExecLastTime);
	m_dwFrameCount = 0;
}