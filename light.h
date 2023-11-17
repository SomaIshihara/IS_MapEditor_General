//==========================================
//
//ライトプログラムのヘッダ[light.h]
//Author:石原颯馬
//
//==========================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

//マクロ
#define MAX_LIGHT	(3)	//ライト数

//クラス化したものがこちらです
class CLight
{
public:
	//コンストラクタ・デストラクタ
	CLight();
	~CLight();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//設定
	void SetDiffuse(const D3DXCOLOR col);
	void SetDirection(const D3DXVECTOR3 dir);
	void SetEnable(const bool bEnable);

	//破棄
	static void ReleaseAll(void);

	//取得
	D3DXCOLOR GetDiffuse(void) { return m_light.Diffuse; }
	D3DXVECTOR3 GetDirection(void) { return m_light.Direction; }

	//生成
	static CLight* Create(const D3DXVECTOR3 dir, const D3DXCOLOR col);

private:
	//リスト
	static CLight* m_pTop;	//先頭オブジェクト
	static CLight* m_pCur;	//最後尾オブジェクト
	CLight* m_pNext;			//次のオブジェクト
	CLight* m_pPrev;			//前のオブジェクト
	int m_nID;					//ID
	static int m_nNumAll;		//総数

	D3DLIGHT9 m_light;	//ライト構造体
};

#endif // !_LIGHT_H_
