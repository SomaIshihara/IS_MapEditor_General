//======================================================
//
//プレイヤー処理のヘッダ[player.h]
//Author:石原颯馬
//
//======================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//前方宣言
class CObjectX;

//プレイヤークラス
class CPlayer
{
public:
	//静的const
	static const float MOVE_SPEED;
	static const float ROTATE_SPEED;

	//コンストラクタ・デストラクタ
	CPlayer();
	~CPlayer();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//選択解除
	void UnsetSelObj(void) { m_pObject = nullptr; }

	//取得処理
	CObjectX* GetSelObj(void) { return m_pObject; }

private:
	//関数
	void Move(void);		//カメラ移動処理
	void Rotate(void);		//カメラ回転処理
	void Select(void);		//オブジェクト選択

	D3DXVECTOR3 m_cursorPos;	//クリック前のカーソル位置
	CObjectX* m_pObject;		//選択しているオブジェ
};

#endif