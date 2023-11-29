//======================================================
//
//メッシュフィールド処理[meshField.cpp]
//Author:石原颯馬
//
//======================================================
#include "meshField.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "manipulation.h"
#include <vector>

using namespace std;

//静的メンバ変数
CMeshField* CMeshField::m_pTop = nullptr;
CMeshField* CMeshField::m_pCur = nullptr;
int CMeshField::m_nNumAll = 0;

//=================================
//コンストラクタ
//=================================
CMeshField::CMeshField() : CObject(PRIORITY_04)
{
	//値クリア
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;
	m_fWidth = CManager::FLT_ZERO;
	m_fDepth = CManager::FLT_ZERO;
	m_nBlockWidth = CManager::INT_ZERO;
	m_nBlockDepth = CManager::INT_ZERO;
	m_texType = CTexture::TYPE_EDITORPOP;
	m_type = IManipulation::TYPE_MESHFIELD;

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
CMeshField::~CMeshField()
{
}

//=================================
//初期化
//=================================
HRESULT CMeshField::Init(void)
{
	//バッファ生成
	CreateBuff();

	//頂点設定
	SetVtxBuff();

	//インデックスバッファ設定
	SetIdxBuff();

	return S_OK;
}

//=================================
//終了
//=================================
void CMeshField::Uninit(void)
{
	//操作インターフェース破棄
	if (m_pManipObj != nullptr)
	{
		m_pManipObj->Uninit();
		m_pManipObj = nullptr;
	}

	//バッファ破棄
	ReleaseBuff();

	//自分自身破棄
	Release();
}

//=================================
//更新
//=================================
void CMeshField::Update(void)
{
}

//=================================
//描画
//=================================
void CMeshField::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxbuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャ設定
	CTexture* pTexture = nullptr;
	if (m_texType == CTexture::TYPE_EDITORPOP)
	{//エディタ表示用
		pTexture = CManager::GetTextureEditorPop();			//テクスチャオブジェクト取得
	}
	else if (m_texType == CTexture::TYPE_SYSTEM)
	{
		pTexture = CManager::GetTextureSystem();	//テクスチャオブジェクト取得
	}
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//ポリゴン描画（インデックスされたやつ）
	int nPolyNum = ((((m_nBlockWidth + 1) * m_nBlockDepth * 2) + (2 * (m_nBlockDepth - 1))) - 2);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (m_nBlockWidth + 1) * (m_nBlockDepth + 1), 0, nPolyNum);
}

//=================================
//生成
//=================================
CMeshField* CMeshField::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth,
	const int nBlockWidth, const int nBlockDepth)
{
	CMeshField* pMeshField = nullptr;

	if (pMeshField == nullptr)
	{
		//オブジェクトの生成
		pMeshField = new CMeshField;

		//初期化
		pMeshField->m_pos = pos;
		pMeshField->m_rot = rot;
		pMeshField->m_fWidth = fWidth;
		pMeshField->m_fDepth = fDepth;
		pMeshField->m_nBlockWidth = nBlockWidth;
		pMeshField->m_nBlockDepth = nBlockDepth;
		pMeshField->Init();

		//操作用オブジェクト生成
		pMeshField->m_pManipObj = CManipulationObj::Create(pMeshField);

		//当たり判定設定
		pMeshField->m_collision.SetVtx(D3DXVECTOR3(-fWidth * nBlockWidth, -1.0f, -fDepth * nBlockDepth), D3DXVECTOR3(fWidth * nBlockWidth, -1.0f, fDepth * nBlockDepth));

		return pMeshField;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//バッファ生成
//=================================
void CMeshField::CreateBuff(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスの取得

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * (m_nBlockWidth + 1) * (m_nBlockDepth + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxbuff,
		nullptr);

	//インデックスバッファの生成
	int nIdxNum = (((m_nBlockWidth + 1) * m_nBlockDepth * 2) + (2 * (m_nBlockDepth - 1)));
	pDevice->CreateIndexBuffer(sizeof(WORD) * nIdxNum,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr);
}

//=================================
//バッファ破棄
//=================================
void CMeshField::ReleaseBuff(void)
{
	//頂点バッファの破棄
	if (m_pVtxbuff != nullptr)
	{
		m_pVtxbuff->Release();
		m_pVtxbuff = nullptr;
	}

	//インデックスバッファ破棄
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}
}

//=================================
//頂点設定
//=================================
void CMeshField::SetVtxBuff(void)
{
	VERTEX_3D *pVtx;

	//バッファロック
	//頂点バッファのロックと頂点情報へのポインタを取得
	m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標+テクスチャ座標
	for (int nCount = 0; nCount < (m_nBlockWidth + 1) * (m_nBlockDepth + 1); nCount++)
	{
		//頂点座標（相対座標）
		pVtx[nCount].pos = D3DXVECTOR3(m_fWidth * (nCount % (m_nBlockWidth + 1)), 0.0f, -m_fDepth * (nCount / (m_nBlockWidth + 1)));

		//カラー
		pVtx[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[nCount].tex = D3DXVECTOR2(((float)(nCount % (m_nBlockWidth + 1))), ((float)(nCount / (m_nBlockWidth + 1))));
	}

	vector<D3DXVECTOR3>* pNor = new vector<D3DXVECTOR3>[(m_nBlockWidth + 1) * (m_nBlockDepth + 1)];

	for (int cntZ = 0; cntZ < m_nBlockDepth; cntZ++)
	{
		for (int cntX = 0; cntX < m_nBlockWidth; cntX++)
		{
			D3DXVECTOR3 nor0, nor1, nor2, nor3;
			D3DXVECTOR3 vec0, vec1;
			int nVtx0 = cntZ * (m_nBlockWidth + 1) + cntX;
			int nVtx1 = cntZ * (m_nBlockWidth + 1) + cntX + 1;
			int nVtx2 = (cntZ + 1) * (m_nBlockWidth + 1) + cntX;
			int nVtx3 = (cntZ + 1) * (m_nBlockWidth + 1) + cntX + 1;

			//1
			vec0 = pVtx[nVtx3].pos - pVtx[nVtx1].pos;
			vec1 = pVtx[nVtx0].pos - pVtx[nVtx1].pos;
			D3DXVec3Cross(&nor1, &vec0, &vec1);
			D3DXVec3Normalize(&nor1, &nor1);

			//2
			vec0 = pVtx[nVtx0].pos - pVtx[nVtx2].pos;
			vec1 = pVtx[nVtx3].pos - pVtx[nVtx2].pos;
			D3DXVec3Cross(&nor2, &vec0, &vec1);
			D3DXVec3Normalize(&nor2, &nor2);

			//0
			nor0 = (nor1 + nor2);
			D3DXVec3Normalize(&nor0, &nor0);
			//3
			nor3 = (nor1 + nor2);
			D3DXVec3Normalize(&nor3, &nor3);

			pNor[nVtx0].push_back(nor0);
			pNor[nVtx1].push_back(nor1);
			pNor[nVtx2].push_back(nor2);
			pNor[nVtx3].push_back(nor3);
		}
	}

	for (int nCount = 0; nCount < (m_nBlockWidth + 1) * (m_nBlockDepth + 1); nCount++, pVtx++)
	{
		D3DXVECTOR3 nor = CManager::VEC3_ZERO;
		//全法線を足す
		for (int cntNor = 0; cntNor < pNor[nCount].size(); cntNor++)
		{
			nor += pNor[nCount].at(cntNor);
		}
		D3DXVec3Normalize(&nor, &nor);

		//法線ベクトル
		pVtx->nor = nor;
	}

	delete[] pNor;

	//頂点バッファをアンロック
	m_pVtxbuff->Unlock();
}

//=================================
//頂点数変更（による頂点・インデックス再設定）
//=================================
void CMeshField::SetVtxNum(void)
{
	//バッファ破棄・再生成
	ReleaseBuff();
	CreateBuff();

	//頂点・インデックス設定
	SetVtxBuff();
	SetIdxBuff();
}

//=================================
//インデックス設定
//=================================
void CMeshField::SetIdxBuff(void)
{
	WORD *pIdx;	//インデックス情報へのポインタ
	int nCntWidth;	//インデックスカウンタ

	//インデックスバッファ設定
	//バッファロック
	m_pIdxBuff->Lock(0, 0, (void **)&pIdx, 0);

	for (nCntWidth = 0; nCntWidth < m_nBlockDepth - 1; nCntWidth++)
	{
		//グネグネパート
		pIdx = SetIdxSwaingField(nCntWidth, pIdx);

		//チョン打ちするパート
		//最後のインデックス情報を次のにも入れる
		*pIdx = m_nBlockWidth + (m_nBlockWidth + 1) * nCntWidth;
		pIdx++;

		//その次のに次のループで最初に入る数字を入れる
		*pIdx = (m_nBlockWidth + 1) + (m_nBlockWidth + 1) * (nCntWidth + 1);
		pIdx++;
	}

	//最後のグネグネパート
	SetIdxSwaingField(nCntWidth, pIdx);

	//バッファアンロック
	m_pIdxBuff->Unlock();
}

//========================
//グネグネインデックス設定処理
//========================
WORD* CMeshField::SetIdxSwaingField(int nCntWidth, WORD *pIdx)
{
	//グネグネパート
	for (int nCountHeight = 0; nCountHeight < (m_nBlockWidth + 1) * 2; nCountHeight++, pIdx++)
	{
		*pIdx = (m_nBlockWidth + 1) * ((nCountHeight % 2) ^ 0x1) + nCountHeight / 2 + (m_nBlockWidth + 1) * nCntWidth;
	}
	return pIdx;
}

//========================
//除外処理
//========================
void CMeshField::Exclusion(void)
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

float CMeshField::GetHeight(D3DXVECTOR3 posNew)
{
	//頂点バッファのロックと頂点情報へのポインタを取得
	VERTEX_3D *pVtx;	//設定用ポインタ
	m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0);

	D3DXVECTOR3 dust;

	for (int cntZ = 0; cntZ < m_nBlockDepth; cntZ++)
	{
		for (int cntX = 0; cntX < m_nBlockWidth; cntX++)
		{
			int nVtx0 = cntZ * (m_nBlockDepth + 1) + cntX;
			int nVtx1 = cntZ * (m_nBlockDepth + 1) + cntX + 1;
			int nVtx2 = (cntZ + 1) * (m_nBlockDepth + 1) + cntX;
			int nVtx3 = (cntZ + 1) * (m_nBlockDepth + 1) + cntX + 1;

			if (D3DXVec3Cross(&dust, &(pVtx[nVtx3].pos - pVtx[nVtx2].pos), &(posNew - (pVtx[nVtx2].pos + m_pos)))->y <= 0.0f &&
				D3DXVec3Cross(&dust, &(pVtx[nVtx0].pos - pVtx[nVtx3].pos), &(posNew - (pVtx[nVtx3].pos + m_pos)))->y <= 0.0f &&
				D3DXVec3Cross(&dust, &(pVtx[nVtx2].pos - pVtx[nVtx0].pos), &(posNew - (pVtx[nVtx0].pos + m_pos)))->y <= 0.0f)
			{
				D3DXVECTOR3 vec1, vec2, nor;
				vec1 = pVtx[nVtx0].pos - pVtx[nVtx2].pos;
				vec2 = pVtx[nVtx3].pos - pVtx[nVtx2].pos;
				D3DXVec3Cross(&nor, &vec1, &vec2);
				D3DXVec3Normalize(&nor, &nor);
				if (nor.y != 0.0f)
				{
					return (-((posNew.x - (pVtx[nVtx2].pos.x + m_pos.x))*nor.x + (posNew.z - (pVtx[nVtx2].pos.z + m_pos.z))*nor.z) / nor.y) + pVtx[nVtx2].pos.y + m_pos.y;
				}
			}

			if (D3DXVec3Cross(&dust, &(pVtx[nVtx0].pos - pVtx[nVtx1].pos), &(posNew - (pVtx[nVtx1].pos + m_pos)))->y <= 0.0f &&
				D3DXVec3Cross(&dust, &(pVtx[nVtx3].pos - pVtx[nVtx0].pos), &(posNew - (pVtx[nVtx0].pos + m_pos)))->y <= 0.0f &&
				D3DXVec3Cross(&dust, &(pVtx[nVtx1].pos - pVtx[nVtx3].pos), &(posNew - (pVtx[nVtx3].pos + m_pos)))->y <= 0.0f)
			{
				D3DXVECTOR3 vec1, vec2, nor;
				vec1 = pVtx[nVtx3].pos - pVtx[nVtx1].pos;
				vec2 = pVtx[nVtx0].pos - pVtx[nVtx1].pos;
				D3DXVec3Cross(&nor, &vec1, &vec2);
				D3DXVec3Normalize(&nor, &nor);
				if (nor.y != 0.0f)
				{
					return (-((posNew.x - (pVtx[nVtx1].pos.x + m_pos.x))*nor.x + (posNew.z - (pVtx[nVtx1].pos.z + m_pos.z))*nor.z) / nor.y) + pVtx[nVtx1].pos.y + m_pos.y;
				}
			}
		}
	}

	//頂点バッファをアンロック
	m_pVtxbuff->Unlock();

	return 0.0f;
}