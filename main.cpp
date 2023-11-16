//======================================================
//
//メイン処理[main.cpp]
//Author:石原颯馬
//
//======================================================
#include <Windows.h>
#include <string.h>
#include "main.h"
#include "manager.h"
#include "resource.h"
#include "xmodel.h"
#include "objloader.h"

//メモリ破壊検出
#include <crtdbg.h>

//マクロ定義
#define FPS_SPEED			(500)	//FPS計測時間

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//========================
//メイン関数
//========================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int CmdShow)
{
	//メモリ破壊検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	CManager* pManager = nullptr;
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//ウィンドウの領域（書き換え不可の定数）

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//メモリサイズ
		CS_CLASSDC,							//ウィンドウのスタイル
		WindowProc,							//ウィンドウプロシージャ
		0,									//使用しない
		0,									//使用しない
		hInstance,							//インスタンスハンドル
		LoadIcon(nullptr,IDI_APPLICATION),	//タスクバーのアイコン
		LoadCursor(nullptr,IDC_ARROW),		//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			//クライアント領域の背景色
		"IDR_MENU1",						//メニューバー
		CLASS_NAME,							//クラスの名前
		LoadIcon(nullptr,IDI_APPLICATION)	//アプリケーションのアイコン
	};

	HWND hWnd;	//ウィンドウハンドル
	MSG msg;	//メッセージ格納

	//時刻関係
	DWORD dwCurrentTime;		//現在時刻
	DWORD dwExecLastTime;		//最後に処理した時刻
	DWORD dwFPSTime;			//最後にFPS計測した時刻

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定サイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウを生成
	//HMENU men = LoadMenu(hInstance, TEXT("IDR_MENU1"));
	hWnd = CreateWindowEx(
		0,										//拡張ウィンドウスタイル
		CLASS_NAME,								//ウィンドウクラスの名前
		WINDOW_NAME,							//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,					//ウィンドウスタイル
		CW_USEDEFAULT,							//ウィンドウの左上X座標
		CW_USEDEFAULT,							//ウィンドウの左上Y座標
		(rect.right - rect.left),				//ウィンドウの幅
		(rect.bottom - rect.top),				//ウィンドウの高さ
		nullptr,								//親ウィンドウのハンドル
		nullptr,	//メニューハンドルまたは子ウィンドウID
		hInstance,								//インスタンスハンドル
		nullptr									//ウィンドウ作成データ
	);

	//マネージャ生成
	pManager = new CManager;

	//マネージャ初期化
	if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//分解能設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;		//初期化
	dwExecLastTime = timeGetTime();
	dwFPSTime = timeGetTime();

	//ウィンドウの表示
	ShowWindow(hWnd, CmdShow);
	UpdateWindow(hWnd);

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();
			if (dwCurrentTime - dwFPSTime >= FPS_SPEED)
			{
				pManager->CheckFPS(dwCurrentTime, dwFPSTime);
				dwFPSTime = dwCurrentTime;
			}
			if ((dwCurrentTime - dwExecLastTime) >= PROC_SPEED)
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime;
				//更新処理
				pManager->Update();

				//描画処理
				pManager->Draw();
			}
		}
	}

	//マネージャ破棄
	if (pManager != nullptr)
	{
		//終了
		pManager->Uninit();
		delete pManager;
		pManager = nullptr;
	}

	//クラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//========================
//ウィンドウプロシージャ
//========================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
	case WM_DESTROY:
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);	//Destroyメッセージを送る
			}
			break;
		}
		break;
	case WM_CLOSE:
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{
			DestroyWindow(hWnd);	//Destroyメッセージを送る
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
			ofn.lpstrInitialDir = szPath;       // 初期フォルダ位置
			ofn.lpstrFile = szFile;       // 選択ファイル格納
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		}

		switch (LOWORD(wParam))
		{
		case ID_XREAD:
			ofn.lpstrFilter = TEXT("Xファイル(*.x)\0*.x\0");
			ofn.lpstrTitle = TEXT("モデルファイルを選択");
			if (GetOpenFileName(&ofn)) {
				//相対パス化
				char aRelat[256];
				int nPathLen = strlen(_strdup(szPath));
				nPathLen++;	//スラッシュ2つ分も消す
				strcpy(&aRelat[0], (_strdup(szFile) + nPathLen));

				CXModel::Load(&aRelat[0]);
			}
			break;
		case ID_MLLOAD:
			ofn.lpstrFilter = TEXT("モデルリストファイル(*.isml)\0*.isml\0");
			ofn.lpstrTitle = TEXT("モデルリストファイルを読み込み");
			if (GetOpenFileName(&ofn)) {
				//相対パス化
				char aRelat[256];
				int nPathLen = strlen(_strdup(szPath));
				nPathLen++;	//スラッシュ2つ分も消す
				strcpy(&aRelat[0], (_strdup(szFile) + nPathLen));

				CXModel::LoadList(&aRelat[0]);
			}
			break;
		case ID_MLWRITE:
			ofn.lpstrFilter = TEXT("モデルリストファイル(*.isml)\0*.isml\0");
			ofn.lpstrTitle = TEXT("モデルリストファイルを保存");
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
			if (GetSaveFileName(&ofn)) {
				//相対パス化
				char aRelat[256];
				int nPathLen = strlen(_strdup(szPath));
				nPathLen++;	//スラッシュ2つ分も消す
				strcpy(&aRelat[0], (_strdup(szFile) + nPathLen));
				//ファイルパスに拡張子がなければ付け足す
				char* pSearch = strstr(&aRelat[0], ".isml");
				if (pSearch == nullptr)
				{
					strcat(&aRelat[0], ".isml");
				}

				CXModel::SaveList(&aRelat[0]);
			}
			break;
		case ID_MDLOAD:
			ofn.lpstrFilter = TEXT("マップデータファイル(*.ismd)\0*.ismd\0");
			ofn.lpstrTitle = TEXT("マップデータファイルを読み込み");
			if (GetOpenFileName(&ofn)) {
				//相対パス化
				char aRelat[256];
				int nPathLen = strlen(_strdup(szPath));
				nPathLen++;	//スラッシュ2つ分も消す
				strcpy(&aRelat[0], (_strdup(szFile) + nPathLen));

				CObjLoader::LoadData(&aRelat[0]);
			}
			break;
		case ID_MDWRITE:
			ofn.lpstrFilter = TEXT("マップデータファイル(*.ismd)\0*.ismd\0");
			ofn.lpstrTitle = TEXT("マップデータファイルを保存");
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
			if (GetSaveFileName(&ofn)) {
				//相対パス化
				char aRelat[256];
				int nPathLen = strlen(_strdup(szPath));
				nPathLen++;	//スラッシュ2つ分も消す
				strcpy(&aRelat[0], (_strdup(szFile) + nPathLen));
				//ファイルパスに拡張子がなければ付け足す
				char* pSearch = strstr(&aRelat[0], ".ismd");
				if (pSearch == nullptr)
				{
					strcat(&aRelat[0], ".ismd");
				}

				CObjLoader::SaveData(&aRelat[0]);
			}
			break;
		case ID_TXTWRITE:
			ofn.lpstrFilter = TEXT("TXT形式マップデータファイル(*.txt)\0*.txt\0");
			ofn.lpstrTitle = TEXT("TXT形式マップデータファイルを保存");
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
			if (GetSaveFileName(&ofn)) {
				//相対パス化
				char aRelat[256];
				int nPathLen = strlen(_strdup(szPath));
				nPathLen++;	//スラッシュ2つ分も消す
				strcpy(&aRelat[0], (_strdup(szFile) + nPathLen));
				//ファイルパスに拡張子がなければ付け足す
				char* pSearch = strstr(&aRelat[0], ".txt");
				if (pSearch == nullptr)
				{
					strcat(&aRelat[0], ".txt");
				}

				CObjLoader::SaveTXTData(&aRelat[0]);
			}
			break;
		case ID_EXIT:
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);	//Destroyメッセージを送る
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