//======================================================
//
//マネージャのヘッダ[manager.h]
//Author:石原颯馬
//
//======================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "main.h"

//前方宣言
class CInputKeyboard;
class CInputMouse;
class CRenderer;
class CPlayer;
class CDebugProc;
class CCamera;
class CTexture;
class CMeshField;
class CGUIAddObj;
class CGUIChangeObj;
class CVariableManager;

class CManager
{
public:
	//静的const
	static const int CManager::INT_ZERO;
	static const float CManager::FLT_ZERO;
	static const D3DXVECTOR2 CManager::VEC2_ZERO;
	static const D3DXVECTOR3 CManager::VEC3_ZERO;

	//コンストラクタ・デストラクタ
	CManager();
	~CManager();

	//基本処理
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	//FPS計測
	void CheckFPS(DWORD dwCurrentTime, DWORD dwExecLastTime);

	//取得
	static CInputKeyboard* GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CInputMouse* GetInputMouse(void) { return m_pInputMouse; }
	static CRenderer* GetRenderer(void) { return m_pRenderer; }
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CDebugProc* GetDebProc(void) { return m_pDebProc; }
	static CCamera* GetCamera(void) { return m_pCamera; }
	static CTexture* GetTextureEditorPop(void) { return m_pTextureEditorPop; }
	static CTexture* GetTextureX(void) { return m_pTextureX; }
	static CTexture* GetTextureSystem(void) { return m_pTextureSystem; }
	static CMeshField* GetMeshField(void) { return m_pMeshField; }
	static CVariableManager* GetVariableManager(void) { return m_pVariableManager; }

private:
	//変数
	static CInputKeyboard* m_pInputKeyboard;	//キーボード
	static CInputMouse* m_pInputMouse;			//マウス
	static CRenderer* m_pRenderer;				//レンダラー
	static CDebugProc* m_pDebProc;				//デバッグ
	static CPlayer* m_pPlayer;					//プレイヤー
	static CCamera* m_pCamera;					//カメラ
	static CTexture* m_pTextureEditorPop;		//テクスチャ
	static CTexture* m_pTextureX;				//Xファイル用テクスチャ
	static CTexture* m_pTextureSystem;			//システム用テクスチャ
	static CMeshField* m_pMeshField;			//メッシュフィールド
	static CGUIAddObj* m_pGUIAddObj;			//オブジェクト追加GUI
	static CGUIChangeObj* m_pGUIChangeObj;		//オブジェクト変更GUI
	static CVariableManager* m_pVariableManager;//変数マネ

	static int m_nFPS;							//FPS
	static DWORD m_dwFrameCount;				//回したフレーム数
};

#endif