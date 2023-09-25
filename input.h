//==========================================================================================
//
//入力プログラムのヘッダ[input.h]
//Author:石原颯馬（振動部:平澤詩苑）
//
//==========================================================================================
//Ver.3.0.1（アクションシューティングマスター後）
//使用可能な入力:キーボード・マウス（カーソル位置・移動量・クリック）・ゲームパッド（1~4台）
//==========================================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//==================================================
//						入力本体
//==================================================
class CInput
{
public:
	//リピート時間
	typedef struct
	{
		DWORD currentTime;	//現在の時間
		DWORD execLastTime;	//最後にtrueにした時間
	} RepeateTime;

	//静的const
	static const int REPEATE_TIME;

	//コンストラクタ・デストラクタ
	CInput();
	virtual ~CInput();

	//基本処理
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
protected:
	static LPDIRECTINPUT8 m_pInput;	//オブジェクト
	LPDIRECTINPUTDEVICE8 m_pDevice;	//デバイス
private:
	static int m_nNumAll;			//使用コントローラ数
};

//==================================================
//						キーボード
//==================================================
class CInputKeyboard : public CInput
{
public:
	//キーボード構造体
	typedef struct
	{
		BYTE state;		//状態（保存用およびプレス情報）
		BYTE trigger;	//トリガー情報
		BYTE release;	//リリース情報
		BYTE repeate;	//リピート情報
		RepeateTime repeateTime;	//リピート時間
	} Keyboard;

	//静的const
	static const int NUM_KEY_MAX;

	//コンストラクタ・デストラクタ
	CInputKeyboard();
	~CInputKeyboard();

	//基本処理
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	//取得
	bool GetPress(int nKey) { return (m_aKeyState[nKey].state & 0x80) ? true : false; }
	bool GetTrigger(int nKey) { return (m_aKeyState[nKey].trigger & 0x80) ? true : false; }
	bool GetRelease(int nKey) { return (m_aKeyState[nKey].release & 0x80) ? true : false; }
	bool GetRepeate(int nKey) { return (m_aKeyState[nKey].repeate & 0x80) ? true : false; }

private:
	Keyboard m_aKeyState[256];	//キーボードすべての状態（トリガー等含む）
};

//==================================================
//						マウス
//==================================================
class CInputMouse : public CInput
{
public:
	//静的const
	static const int CLICK_LEFT;
	static const int CLICK_RIGHT;
	static const int BUTTON_MAX;

	//マウスのボタン部
	typedef struct
	{
		BYTE trigger;		//ボタンのトリガー情報
		BYTE release;		//ボタンのリリース情報
		BYTE repeate;		//リピート情報
		RepeateTime repeateTime;	//リピート時間
	} MouseButton;

	//マウス構造体
	typedef struct
	{
		MouseButton* mb;			//マウスボタン
		DIMOUSESTATE state;					//状態およびプレス情報
		D3DXVECTOR3 pos;					//位置
		D3DXVECTOR3 move;					//移動量
	} Mouse;

	//コンストラクタ・デストラクタ
	CInputMouse();
	~CInputMouse();

	//基本処理
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	//クリック座標をワールド座標に変換
	D3DXVECTOR3 ConvertClickPosToWorld(float fZ);

	//取得
	bool GetPress(int nButton) { return m_mouse.state.rgbButtons[nButton] & 0x80 ? true : false; }
	bool GetTrigger(int nButton) { return m_mouse.mb[nButton].trigger & 0x80 ? true : false; }
	bool GetRelease(int nButton) { return m_mouse.mb[nButton].release & 0x80 ? true : false; }
	bool GetRepeate(int nButton) { return m_mouse.mb[nButton].repeate & 0x80 ? true : false; }
	D3DXVECTOR3 GetPos(void) { return m_mouse.pos; }
	D3DXVECTOR3 GetMove(void) { return m_mouse.move; }
	int GetWheel(void) { return m_mouse.state.lZ; }

private:
	Mouse m_mouse;	//マウスすべて
};

//==================================================
//					 ゲームパッド
//==================================================
class CInputGamePad : public CInput
{
public:
	//静的const
	static const int MAX_USE_GAMEPAD;
	static const int STICK_MAX;
	static const int STICK_DEADZONE;
	static const int BUTTON_NUM;
	static const int VIBE_POWER_MIN;
	static const int VIBE_POWER_LEVEL_01;
	static const int VIBE_POWER_LEVEL_02;
	static const int VIBE_POWER_LEVEL_03;
	static const int VIBE_POWER_LEVEL_04;
	static const int VIBE_POWER_LEVEL_05;
	static const int VIBE_FADE;

	//ゲームパッドの振動状態
	typedef enum
	{
		VIBE_STATE_00_STOP,	//停止
		VIBE_STATE_01_LOOP,	//ずっと同じ振動
		VIBE_STATE_02_FADE,	//徐々に落ち着いていく
		VIBE_STATE_MAX
	}VIBE_STATE;

	//ゲームパッド構造体
	typedef struct
	{
		XINPUT_STATE state;			//状態全部（およびプレス情報）
		WORD trigger;				//トリガー情報
		WORD release;				//リリース情報
		WORD repeate;				//リピート情報
		RepeateTime repeateTime;	//リピート時間

		VIBE_STATE Vibr_State;		//振動の状態
		WORD wVibrPower;			//振動の強さ
		int nVibrTime;				//振動させる時間
	} GamePad;

	//コンストラクタ・デストラクタ
	CInputGamePad();
	~CInputGamePad();

	//基本処理
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	//振動処理
	void UpdateVibration(void);
	void StopVibration(void);
	void SetPadVibration(WORD wPower, int nVibeTime, VIBE_STATE State);

	//取得
	bool GetPress(int nButton) { return m_gamePad.state.Gamepad.wButtons & nButton ? true : false; }
	bool GetTrigger(int nButton) { return m_gamePad.trigger & nButton ? true : false; }
	bool GetRelease(int nButton) { return m_gamePad.release & nButton ? true : false; }
	bool GetRepeate(int nButton) { return m_gamePad.repeate & nButton ? true : false; }
	SHORT GetLStickX(void) { return AdjustStick(m_gamePad.state.Gamepad.sThumbLX); }
	SHORT GetLStickY(void) { return AdjustStick(m_gamePad.state.Gamepad.sThumbLY); }
	SHORT GetRStickX(void) { return AdjustStick(m_gamePad.state.Gamepad.sThumbRX); }
	SHORT GetRStickY(void) { return AdjustStick(m_gamePad.state.Gamepad.sThumbRY); }
private:
	SHORT AdjustStick(SHORT stick);
	static int m_nNumAll;//ゲームパッド数
	GamePad m_gamePad;	//ゲームパッドすべて
	int m_nID;			//ゲームパッドID
};

#endif // !_INPUT_H_
