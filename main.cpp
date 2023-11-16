//======================================================
//
//���C������[main.cpp]
//Author:�Ό��D�n
//
//======================================================
#include <Windows.h>
#include <string.h>
#include "main.h"
#include "manager.h"
#include "resource.h"
#include "xmodel.h"
#include "objloader.h"

//�������j�󌟏o
#include <crtdbg.h>

//�}�N����`
#define FPS_SPEED			(500)	//FPS�v������

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//========================
//���C���֐�
//========================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int CmdShow)
{
	//�������j�󌟏o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	CManager* pManager = nullptr;
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//�E�B���h�E�̗̈�i���������s�̒萔�j

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//�������T�C�Y
		CS_CLASSDC,							//�E�B���h�E�̃X�^�C��
		WindowProc,							//�E�B���h�E�v���V�[�W��
		0,									//�g�p���Ȃ�
		0,									//�g�p���Ȃ�
		hInstance,							//�C���X�^���X�n���h��
		LoadIcon(nullptr,IDI_APPLICATION),	//�^�X�N�o�[�̃A�C�R��
		LoadCursor(nullptr,IDC_ARROW),		//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),			//�N���C�A���g�̈�̔w�i�F
		"IDR_MENU1",						//���j���[�o�[
		CLASS_NAME,							//�N���X�̖��O
		LoadIcon(nullptr,IDI_APPLICATION)	//�A�v���P�[�V�����̃A�C�R��
	};

	HWND hWnd;	//�E�B���h�E�n���h��
	MSG msg;	//���b�Z�[�W�i�[

	//�����֌W
	DWORD dwCurrentTime;		//���ݎ���
	DWORD dwExecLastTime;		//�Ō�ɏ�����������
	DWORD dwFPSTime;			//�Ō��FPS�v����������

	//�E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�B���h�E�𐶐�
	//HMENU men = LoadMenu(hInstance, TEXT("IDR_MENU1"));
	hWnd = CreateWindowEx(
		0,										//�g���E�B���h�E�X�^�C��
		CLASS_NAME,								//�E�B���h�E�N���X�̖��O
		WINDOW_NAME,							//�E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,					//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,							//�E�B���h�E�̍���X���W
		CW_USEDEFAULT,							//�E�B���h�E�̍���Y���W
		(rect.right - rect.left),				//�E�B���h�E�̕�
		(rect.bottom - rect.top),				//�E�B���h�E�̍���
		nullptr,								//�e�E�B���h�E�̃n���h��
		nullptr,	//���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,								//�C���X�^���X�n���h��
		nullptr									//�E�B���h�E�쐬�f�[�^
	);

	//�}�l�[�W������
	pManager = new CManager;

	//�}�l�[�W��������
	if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//����\�ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;		//������
	dwExecLastTime = timeGetTime();
	dwFPSTime = timeGetTime();

	//�E�B���h�E�̕\��
	ShowWindow(hWnd, CmdShow);
	UpdateWindow(hWnd);

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();
			if (dwCurrentTime - dwFPSTime >= FPS_SPEED)
			{
				pManager->CheckFPS(dwCurrentTime, dwFPSTime);
				dwFPSTime = dwCurrentTime;
			}
			if ((dwCurrentTime - dwExecLastTime) >= PROC_SPEED)
			{//60����1�b�o��
				dwExecLastTime = dwCurrentTime;
				//�X�V����
				pManager->Update();

				//�`�揈��
				pManager->Draw();
			}
		}
	}

	//�}�l�[�W���j��
	if (pManager != nullptr)
	{
		//�I��
		pManager->Uninit();
		delete pManager;
		pManager = nullptr;
	}

	//�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//========================
//�E�B���h�E�v���V�[�W��
//========================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
	case WM_DESTROY:
		//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);	//Destroy���b�Z�[�W�𑗂�
			}
			break;
		}
		break;
	case WM_CLOSE:
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

		if (nID == IDYES)
		{
			DestroyWindow(hWnd);	//Destroy���b�Z�[�W�𑗂�
		}
		else
		{
			return 0;
		}
		break;
	case WM_COMMAND:
		static OPENFILENAME     ofn;
		static TCHAR            szPath[MAX_PATH];
		static TCHAR            szFile[MAX_PATH];

		if (szPath[0] == TEXT('\0')) {
			GetCurrentDirectory(MAX_PATH, szPath);
		}
		if (ofn.lStructSize == 0) {
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = FindWindow(nullptr, TEXT(WINDOW_NAME));
			ofn.lpstrInitialDir = szPath;       // �����t�H���_�ʒu
			ofn.lpstrFile = szFile;       // �I���t�@�C���i�[
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		}

		switch (LOWORD(wParam))
		{
		case ID_XREAD:
			ofn.lpstrFilter = TEXT("X�t�@�C��(*.x)\0*.x\0");
			ofn.lpstrTitle = TEXT("���f���t�@�C����I��");
			if (GetOpenFileName(&ofn)) {
				//���΃p�X��
				char aRelat[256];
				int nPathLen = strlen(_strdup(szPath));
				nPathLen++;	//�X���b�V��2��������
				strcpy(&aRelat[0], (_strdup(szFile) + nPathLen));

				CXModel::Load(&aRelat[0]);
			}
			break;
		case ID_MLLOAD:
			ofn.lpstrFilter = TEXT("���f�����X�g�t�@�C��(*.isml)\0*.isml\0");
			ofn.lpstrTitle = TEXT("���f�����X�g�t�@�C����ǂݍ���");
			if (GetOpenFileName(&ofn)) {
				//���΃p�X��
				char aRelat[256];
				int nPathLen = strlen(_strdup(szPath));
				nPathLen++;	//�X���b�V��2��������
				strcpy(&aRelat[0], (_strdup(szFile) + nPathLen));

				CXModel::LoadList(&aRelat[0]);
			}
			break;
		case ID_MLWRITE:
			ofn.lpstrFilter = TEXT("���f�����X�g�t�@�C��(*.isml)\0*.isml\0");
			ofn.lpstrTitle = TEXT("���f�����X�g�t�@�C����ۑ�");
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
			if (GetSaveFileName(&ofn)) {
				//���΃p�X��
				char aRelat[256];
				int nPathLen = strlen(_strdup(szPath));
				nPathLen++;	//�X���b�V��2��������
				strcpy(&aRelat[0], (_strdup(szFile) + nPathLen));
				//�t�@�C���p�X�Ɋg���q���Ȃ���Εt������
				char* pSearch = strstr(&aRelat[0], ".isml");
				if (pSearch == nullptr)
				{
					strcat(&aRelat[0], ".isml");
				}

				CXModel::SaveList(&aRelat[0]);
			}
			break;
		case ID_MDLOAD:
			ofn.lpstrFilter = TEXT("�}�b�v�f�[�^�t�@�C��(*.ismd)\0*.ismd\0");
			ofn.lpstrTitle = TEXT("�}�b�v�f�[�^�t�@�C����ǂݍ���");
			if (GetOpenFileName(&ofn)) {
				//���΃p�X��
				char aRelat[256];
				int nPathLen = strlen(_strdup(szPath));
				nPathLen++;	//�X���b�V��2��������
				strcpy(&aRelat[0], (_strdup(szFile) + nPathLen));

				CObjLoader::LoadData(&aRelat[0]);
			}
			break;
		case ID_MDWRITE:
			ofn.lpstrFilter = TEXT("�}�b�v�f�[�^�t�@�C��(*.ismd)\0*.ismd\0");
			ofn.lpstrTitle = TEXT("�}�b�v�f�[�^�t�@�C����ۑ�");
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
			if (GetSaveFileName(&ofn)) {
				//���΃p�X��
				char aRelat[256];
				int nPathLen = strlen(_strdup(szPath));
				nPathLen++;	//�X���b�V��2��������
				strcpy(&aRelat[0], (_strdup(szFile) + nPathLen));
				//�t�@�C���p�X�Ɋg���q���Ȃ���Εt������
				char* pSearch = strstr(&aRelat[0], ".ismd");
				if (pSearch == nullptr)
				{
					strcat(&aRelat[0], ".ismd");
				}

				CObjLoader::SaveData(&aRelat[0]);
			}
			break;
		case ID_TXTWRITE:
			ofn.lpstrFilter = TEXT("TXT�`���}�b�v�f�[�^�t�@�C��(*.txt)\0*.txt\0");
			ofn.lpstrTitle = TEXT("TXT�`���}�b�v�f�[�^�t�@�C����ۑ�");
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
			if (GetSaveFileName(&ofn)) {
				//���΃p�X��
				char aRelat[256];
				int nPathLen = strlen(_strdup(szPath));
				nPathLen++;	//�X���b�V��2��������
				strcpy(&aRelat[0], (_strdup(szFile) + nPathLen));
				//�t�@�C���p�X�Ɋg���q���Ȃ���Εt������
				char* pSearch = strstr(&aRelat[0], ".txt");
				if (pSearch == nullptr)
				{
					strcat(&aRelat[0], ".txt");
				}

				CObjLoader::SaveTXTData(&aRelat[0]);
			}
			break;
		case ID_EXIT:
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);	//Destroy���b�Z�[�W�𑗂�
			}
			break;
		case ID_CAMSET:
			break;
		case ID_LIGSET:
			break;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}