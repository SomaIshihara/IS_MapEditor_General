//======================================================
//
//エディタ操作用オブジェクト処理[manipulation.cpp]
//Author:石原颯馬
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "manipulation.h"
#include "xmodel.h"
#include "interface.h"
#include <assert.h>

//静的メンバ変数
CManipulationObj* CManipulationObj::m_pTop = nullptr;
CManipulationObj* CManipulationObj::m_pCur = nullptr;
int CManipulationObj::m_nNumAll = 0;

//=================================
//コンストラクタ（デフォルト）
//=================================
CManipulationObj::CManipulationObj(int nPriority) : CObject(nPriority)
{
	if (m_pCur == nullptr)
	{//最後尾がいない（すなわち先頭もいない）
		m_pTop = this;		//俺が先頭
		m_pPrev = nullptr;		//前後誰もいない
		m_pNext = nullptr;
	}
	else
	{//最後尾がいる
		m_pPrev = m_pCur;		//最後尾が自分の前のオブジェ
		m_pCur->m_pNext = this;	//最後尾の次のオブジェが自分
		m_pNext = nullptr;			//自分の次のオブジェはいない
	}
	m_pCur = this;				//俺が最後尾

	m_nNumAll++;
}

//=================================
//デストラクタ
//=================================
CManipulationObj::~CManipulationObj()
{
}

//========================
//初期化処理
//========================
HRESULT CManipulationObj::Init(void)
{
	return S_OK;
}

//========================
//終了処理
//========================
void CManipulationObj::Uninit(void)
{
	//自分自身破棄
	Release();
}

//========================
//更新処理
//========================
void CManipulationObj::Update(void)
{
	
}

//========================
//描画処理
//========================
void CManipulationObj::Draw(void)
{
	
}

//========================
//生成処理
//========================
CManipulationObj* CManipulationObj::Create(IManipulation* face)
{
	CManipulationObj* pManip = nullptr;

	if (pManip == nullptr)
	{
		//オブジェクトの生成
		pManip = new CManipulationObj;

		//初期化
		pManip->Init();
		pManip->m_interface = face;

		return pManip;
	}
	else
	{
		return nullptr;
	}
}

//========================
//除外処理
//========================
void CManipulationObj::Exclusion(void)
{
	if (m_pPrev != nullptr)
	{//前にオブジェがいる
		m_pPrev->m_pNext = m_pNext;	//前のオブジェの次のオブジェは自分の次のオブジェ
	}
	if (m_pNext != nullptr)
	{
		m_pNext->m_pPrev = m_pPrev;	//次のオブジェの前のオブジェは自分の前のオブジェ
	}

	if (m_pCur == this)
	{//最後尾でした
		m_pCur = m_pPrev;	//最後尾を自分の前のオブジェにする
	}
	if (m_pTop == this)
	{
		m_pTop = m_pNext;	//先頭を自分の次のオブジェにする
	}

	//成仏
	m_nNumAll--;	//総数減らす
}