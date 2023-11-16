//======================================================
//
//テクスチャ保存クラスのヘッダ[texture.h]
//Author:石原颯馬
//
//======================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"
#define MAX_TEXTURE	(128)	//テクスチャ数

//テクスチャ保存クラス
class CTexture
{
public:
	enum TYPE
	{
		TYPE_EDITORPOP = 0,	//エディタに表示するもの
		TYPE_X,				//Xファイルの読み込み分
		TYPE_SYSTEM,		//エディタのシステム用
	};

	//静的const
	static const int CODE_LENGTH;	//仮置きに必要

	//パターンテクスチャ構造体定義
	typedef struct
	{
		LPDIRECT3DTEXTURE9 pTexture;	//テクスチャポインタ
		int nPatternWidth;				//パターン幅
		int nPatternHeight;				//パターン高さ
	} PatternTexture;
	//コンストラクタ・デストラクタ
	CTexture();
	~CTexture();

	//全読み込み・破棄
	HRESULT Load(const char* pPath);	//あらかじめ読み込みたいテクスチャ読み込み
	void Unload(void);					//追加した分も含めてすべて破棄

	int Regist(const char* pPath, const int nPatWidth = 1, const int nPatHeight = 1);	//追加する分の読み込み
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);											//テクスチャ取得
	int GetPatWidth(int nIdx);					//パターン幅取得
	int GetPatHeight(int nIdx);					//パターン高さ取得
	char* GetPath(int nIdx);
	int GetNumAll(void) { return m_nNumAll; }

private:
	HRESULT LoadTexture(const char* pPath, const int nPatWidth, const int nPatHeight);
	PatternTexture m_apTexture[MAX_TEXTURE];		//テクスチャポインタ
	char* m_aFilepath[MAX_TEXTURE];					//テクスチャパス
	int m_nNumAll;									//テクスチャ総数
};

#endif // !_TEXTURE_H_
