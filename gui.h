//======================================================
//
//GUIオブジェクトのヘッダ[gui.h]
//Author:石原颯馬
//
//======================================================
#ifndef _GUI_H_
#define _GUI_H_

#include "object.h"

//前方宣言
class CXModel;

//オブジェクト追加GUIクラス
class CGUIAddObj : public CObject
{
public:
	//コンストラクタ・デストラクタ
	CGUIAddObj();
	~CGUIAddObj();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGUIAddObj* Create(void);

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return CManager::FLT_ZERO; }
	float GetHeight(void) { return CManager::FLT_ZERO; }
	float GetDepth(void) { return CManager::FLT_ZERO; }
	int GetModelIdx(void) { return -1; }
private:
	CXModel* m_SelectObj;
	D3DXVECTOR3 m_pos, m_rot;
	bool m_bDragged;
};

//オブジェクト変更GUIクラス
class CGUIChangeObj : public CObject
{
public:
	//コンストラクタ・デストラクタ
	CGUIChangeObj();
	~CGUIChangeObj();
	
	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGUIChangeObj* Create(void);

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return CManager::FLT_ZERO; }
	float GetHeight(void) { return CManager::FLT_ZERO; }
	float GetDepth(void) { return CManager::FLT_ZERO; }
	int GetModelIdx(void) { return -1; }
private:
	D3DXVECTOR3 m_pos, m_rot;
	bool m_bDragged;
};

#endif // !_GUI_H_
