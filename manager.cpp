//======================================================
//
//�}�l�[�W��[manager.cpp]
//Author:�Ό��D�n
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

//�ÓI�����o�ϐ�
const int CManager::INT_ZERO = 0;										//int�^��0
const float CManager::FLT_ZERO = 0.0f;									//float�^��0
const D3DXVECTOR2 CManager::VEC2_ZERO = D3DXVECTOR2(0.0f, 0.0f);		//vec2�^��0
const D3DXVECTOR3 CManager::VEC3_ZERO = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//vec3�^��0

//�ÓI�����o�ϐ�
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
//�R���X�g���N�^
//=================================
CManager::CManager()
{
	
}

//=================================
//�f�X�g���N�^
//=================================
CManager::~CManager()
{}

//=================================
//������
//=================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//����
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputMouse = new CInputMouse;
	m_pRenderer = new CRenderer;
	m_pDebProc = new CDebugProc;
	m_pCamera = new CCamera;
	m_pTextureEditorPop = new CTexture;			//�I�u�W�F�N�g�p
	m_pTextureX = new CTexture;					//X�t�@�C���p
	m_pTextureSystem = new CTexture;			//�V�X�e���p
	m_pPlayer = new CPlayer;
	m_pGUIAddObj = CGUIAddObj::Create();
	m_pGUIChangeObj = CGUIChangeObj::Create();
	m_pVariableManager = new CVariableManager;

	//�����_���[������
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h������
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�}�E�X������
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�f�o�b�O������
	m_pDebProc->Init();

	//�J����������
	if (FAILED(m_pCamera->Init()))
	{
		return E_FAIL;
	}

	//���C�g������
	CLight::Create(D3DXVECTOR3(0.22f, -0.87f, 0.44f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CLight::Create(D3DXVECTOR3(-0.18f, 0.88f, -0.44f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CLight::Create(D3DXVECTOR3(0.89f, -0.11f, 0.44f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//�e�N�X�`��������
	if (FAILED(m_pTextureEditorPop->Load("data\\preload.txt")))
	{
		return E_FAIL;
	}

	//�V�X�e���ŕK�v�Ȃ��̂��������œǂݍ���
	m_pTextureSystem->Regist("data\\TEXTURE\\Tex_AddObj.png");
	m_pTextureSystem->Regist("data\\TEXTURE\\Tex_Camera.png");
	m_pTextureSystem->Regist("data\\TEXTURE\\Tex_Light.png");

	//�v���C���[������
	if (FAILED(m_pPlayer->Init()))
	{
		return E_FAIL;
	}

	//�ϐ���`
	m_pVariableManager->ReadUserDefData("data\\system.ini");

	//ImGUI�n
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

	//FPS�v���평����
	m_nFPS = 0;
	m_dwFrameCount = 0;

	//�ł���
	return S_OK;
}

//=================================
//�I��
//=================================
void CManager::Uninit(void)
{
	//�I�u�W�F�N�g�I��+�j��
	CObject::ReleaseAll();

	//���f���j��
	CXModel::UnloadAll();			//X���f��

	//ImGUI
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//�ϐ��}�l�j��
	if (m_pVariableManager != nullptr)
	{
		m_pVariableManager->ReleaseAll();
		delete m_pVariableManager;
		m_pVariableManager = nullptr;
	}

	//�v���C���[�j��
	if (m_pPlayer != nullptr)
	{//�v���C���[�I��
		m_pPlayer->Uninit();
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}

	//�e�N�X�`���j��
	if (m_pTextureSystem != nullptr)
	{//�e�N�X�`���I��
		m_pTextureSystem->Unload();
		delete m_pTextureSystem;
		m_pTextureSystem = nullptr;
	}
	if (m_pTextureX != nullptr)
	{//�e�N�X�`���I��
		m_pTextureX->Unload();
		delete m_pTextureX;
		m_pTextureX = nullptr;
	}
	if (m_pTextureEditorPop != nullptr)
	{//�e�N�X�`���I��
		m_pTextureEditorPop->Unload();
		delete m_pTextureEditorPop;
		m_pTextureEditorPop = nullptr;
	}

	//���C�g�j��
	CLight::ReleaseAll();

	//�J�����j��
	if (m_pCamera != nullptr)
	{//�J�����I��
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	//�f�o�b�O�j��
	if (m_pDebProc != nullptr)
	{//�f�o�b�O�I��
		m_pDebProc->Uninit();
		delete m_pDebProc;
		m_pDebProc = nullptr;
	}

	//�}�E�X�j��
	if (m_pInputMouse != nullptr)
	{//�}�E�X�I��
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = nullptr;
	}

	//�L�[�{�[�h�j��
	if (m_pInputKeyboard != nullptr)
	{//�L�[�{�[�h�I��
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	//�����_���[�j��
	if (m_pRenderer != nullptr)
	{//�����_���[�I��
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
}

//=================================
//�X�V
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

	//���̎��_�Ŏ��S�t���O�������Ă���I�u�W�F���E��
	CObject::Death();

	//FPS�v����̏���
	m_dwFrameCount++;

	//�f�o�b�O�\��
	m_pDebProc->Print("FPS:%d\n", m_nFPS);
	m_pDebProc->Print("[������@]\n");
	m_pDebProc->Print("�E�N���b�N+�ړ�:���_�ړ�\n");
	m_pDebProc->Print("WASD:�v���C���[�i�J�����j�ړ�\n");

	ImGui::EndFrame();
}

//=================================
//�`��
//=================================
void CManager::Draw(void)
{
	m_pRenderer->Draw();
}

//=================================
//FPS�v��
//=================================
void CManager::CheckFPS(DWORD dwCurrentTime, DWORD dwExecLastTime)
{
	m_nFPS = (m_dwFrameCount * 1000) / (dwCurrentTime - dwExecLastTime);
	m_dwFrameCount = 0;
}