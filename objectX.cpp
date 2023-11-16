//======================================================
//
//オブジェクト（X）処理[object3D.cpp]
//Author:石原颯馬
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "objectX.h"
#include "xmodel.h"
#include <assert.h>

//静的メンバ変数
CObjectX* CObjectX::m_pTop = nullptr;
CObjectX* CObjectX::m_pCur = nullptr;
int CObjectX::m_nNumAll = 0;

//=================================
//コンストラクタ（デフォルト）
//=================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	//クリア
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;

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
	m_bExclusion = false;		//生きてる
	m_pModel = nullptr;

	m_nNumAll++;
}

//=================================
//デストラクタ
//=================================
CObjectX::~CObjectX()
{
}

//========================
//初期化処理
//========================
HRESULT CObjectX::Init(void)
{
	return S_OK;
}

//========================
//終了処理
//========================
void CObjectX::Uninit(void)
{
	m_bExclusion = true;		//除外予定

	//自分自身破棄
	Release();
}

//========================
//更新処理
//========================
void CObjectX::Update(void)
{
	
}

//========================
//描画処理
//========================
void CObjectX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得
	CTexture* pTexture = CManager::GetTextureX();						//テクスチャオブジェクト取得
	D3DXMATRIX mtxRot, mtxTrans, mtxTexture;							//計算用
	D3DMATERIAL9 matDef;												//現在のマテリアル保存用
	D3DXMATERIAL *pMat;													//マテリアルデータへのポインタ

	//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	//モデル取得
	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&mtxWorld);

	//拡縮を反映
	//D3DXMatrixScaling(&mtxScall, FENCE_SCALE, FENCE_SCALE, FENCE_SCALE);
	//D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxScall);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//位置反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//マテリアルデータへのポインタ取得
	pMat = (D3DXMATERIAL*)m_pModel->GetBufMat()->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModel->GetNumMat(); nCntMat++)
	{
		//マテリアル設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャ設定
		pDevice->SetTexture(0, pTexture->GetAddress(m_pModel->GetIdxTexture()[nCntMat]));

		//モデル描画
		m_pModel->GetMesh()->DrawSubset(nCntMat);
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//生成処理
//========================
CObjectX* CObjectX::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel)
{
	CObjectX* pObjX = nullptr;

	if (pObjX == nullptr)
	{
		//オブジェクトの生成
		pObjX = new CObjectX;

		//初期化
		pObjX->Init();
		pObjX->m_pos = pos;
		pObjX->m_rot = rot;
		pObjX->m_pModel = pModel;

		//追加変数設定
		CVariableManager* pVariableManager = CManager::GetVariableManager();
		int nVariableNum = pVariableManager->GetDefinedNum();
		for (int cnt = 0; cnt < nVariableNum; cnt++)
		{
			char* pName = pVariableManager->GetDefinedVariable()[cnt]->pName;
			pObjX->m_apVariable[cnt] = pVariableManager->Declaration(pName);
		}

		return pObjX;
	}
	else
	{
		return nullptr;
	}
}

//========================
//オブジェクト単位除外処理
//========================
void CObjectX::Delete(CXModel * pTarget)
{
	CObjectX* pObject = m_pTop;	//先頭を入れる

	while (pObject != nullptr)
	{//最後尾まで回し続ける
		CObjectX* pObjectNext = pObject->m_pNext;	//次のオブジェ保存

		if (pObject->GetModel() == pTarget)
		{
			pObject->Uninit();	//除外
		}

		pObject = pObjectNext;	//次を入れる
	}
}

//========================
//除外処理
//========================
void CObjectX::Exclusion(void)
{
	CObjectX* pObject = m_pTop;	//先頭を入れる

	while (pObject != nullptr)
	{//最後尾まで回し続ける
		CObjectX* pObjectNext = pObject->m_pNext;	//次のオブジェ保存

		if (pObject->m_bExclusion == true)
		{//死亡フラグが立ってる
			if (pObject->m_pPrev != nullptr)
			{//前にオブジェがいる
				pObject->m_pPrev->m_pNext = pObject->m_pNext;	//前のオブジェの次のオブジェは自分の次のオブジェ
			}
			if (pObject->m_pNext != nullptr)
			{
				pObject->m_pNext->m_pPrev = pObject->m_pPrev;	//次のオブジェの前のオブジェは自分の前のオブジェ
			}

			if (m_pCur == pObject)
			{//最後尾でした
				m_pCur = pObject->m_pPrev;	//最後尾を自分の前のオブジェにする
			}
			if (m_pTop == pObject)
			{
				m_pTop = pObject->m_pNext;	//先頭を自分の次のオブジェにする
			}

			//成仏
			m_nNumAll--;	//総数減らす
		}

		pObject = pObjectNext;	//次を入れる
	}
}