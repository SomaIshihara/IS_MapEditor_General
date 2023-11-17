//==========================================
//
//ライト関係プログラム[light.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"

//静的メンバ変数
CLight* CLight::m_pTop = nullptr;
CLight* CLight::m_pCur = nullptr;
int CLight::m_nNumAll = 0;

//========================
//コンストラクタ
//========================
CLight::CLight()
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
	m_nID = m_nNumAll;
	m_nNumAll++;

	//ライト情報クリア
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));
}

//========================
//デストラクタ
//========================
CLight::~CLight()
{

}

//========================
//初期化処理
//========================
HRESULT CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスの取得
	D3DXVECTOR3 vecDir;	//設定用

	//ライト情報クリア
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));

	//ライト種類設定
	m_light.Type = D3DLIGHT_DIRECTIONAL;

	//ライト拡散光設定
	m_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトの方向
	vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	D3DXVec3Normalize(&vecDir, &vecDir);

	m_light.Direction = vecDir;

	//ライト設定
	pDevice->SetLight(m_nID, &m_light);

	//ライト有効化
	pDevice->LightEnable(m_nID, TRUE);

	//できた
	return S_OK;
}

//========================
//終了処理
//========================
void CLight::Uninit(void)
{
	//無
}

//========================
//更新処理
//========================
void CLight::Update(void)
{
	//無
}

//========================
//色変更処理
//========================
void CLight::SetDiffuse(const D3DXCOLOR col)
{
	m_light.Diffuse = col;
	SetEnable(true);
}

//========================
//向き変更処理
//========================
void CLight::SetDirection(const D3DXVECTOR3 dir)
{
	m_light.Direction = dir;
	SetEnable(true);
}

//========================
//有効・無効処理
//========================
void CLight::SetEnable(const bool bEnable)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスの取得
	if (bEnable == true)
	{
		//ライト設定
		pDevice->SetLight(m_nID, &m_light);

		//ライト有効化
		pDevice->LightEnable(m_nID, TRUE);
	}
	else
	{
		//ライト無効化
		pDevice->LightEnable(m_nID, FALSE);
	}
}

//========================
//すべて破棄
//========================
void CLight::ReleaseAll(void)
{
	CLight* pObject = m_pTop;	//先頭を入れる

	while (pObject != nullptr)
	{//最後尾まで回し続ける
		CLight* pObjectNext = pObject->m_pNext;	//次のオブジェ保存

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
		pObject->Uninit();
		delete pObject;

		pObject = pObjectNext;	//次を入れる
	}
}

//========================
//生成
//========================
CLight* CLight::Create(const D3DXVECTOR3 dir, const D3DXCOLOR col)
{
	CLight* pLight = nullptr;

	if (pLight == nullptr)
	{
		//オブジェクトの生成
		pLight = new CLight;

		//初期化
		pLight->Init();
		pLight->SetDirection(dir);
		pLight->SetDiffuse(col);

		return pLight;
	}
	else
	{
		return nullptr;
	}
}
