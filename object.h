//======================================================
//
//オブジェクト処理のヘッダ[object.h]
//Author:石原颯馬
//
//======================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "main.h"

//優先順位参考表
typedef enum
{
	PRIORITY_BG = 0,		//BG
	PRIORITY_UNDEF01,
	PRIORITY_UNDEF02,
	PRIORITY_DEFAULT,		//デフォルト
	PRIORITY_UNDEF04,
	PRIORITY_UI,			//UI
	PRIORITY_PAUSE,			//ポーズ
	PRIORITY_FADE,			//フェード
	PRIORITY_MAX			//最大優先順位（これで足りるらしい）
} PRIORITY;

//オブジェクトクラス
class CObject
{
public:
	//コンストラクタ・デストラクタ
	CObject(int nPriority = PRIORITY_DEFAULT);
	virtual ~CObject();

	//基本処理
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	//全オブジェクト系
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);

	//取得・設定
	static CObject* GetTop(const int nPriority) { return m_apTop[nPriority]; }
	CObject* GetNext(void) { return m_pNext; }

	//純粋仮想取得
	virtual D3DXVECTOR3 GetPos(void) = 0;
	virtual D3DXVECTOR3 GetRot(void) = 0;
	virtual float GetWidth(void) = 0;
	virtual float GetHeight(void) = 0;
	virtual float GetDepth(void) = 0;

	//死亡フラグが立っているオブジェを殺す
	static void Death(void);

protected:
	void Release(void);

private:
	static CObject* m_apTop[PRIORITY_MAX];	//先頭オブジェクト
	static CObject* m_apCur[PRIORITY_MAX];	//最後尾オブジェクト
	CObject* m_pNext;		//次のオブジェクト
	CObject* m_pPrev;		//前のオブジェクト
	static int m_nNumAll;	//総数
	int m_nPriority;		//優先順位
	bool m_bDeath;			//死亡フラグ
};

#endif // !_OBJECT_H_
